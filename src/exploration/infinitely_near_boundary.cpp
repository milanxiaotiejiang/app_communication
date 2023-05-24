//
// Created by Looper on 2023/2/2.
//

#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include "exploration/infinitely_near_boundary.h"
#include "glog/logging.h"
#include "exploration/line.h"
#include "exploration/A_star_pathplanner.h"
#include "exploration/cv_extend.h"
#include "exploration/douglas/DouglasPeucker.h"
#include "exploration/douglas/Point2D.h"

#define random(a, b) (rand() % (b - a) + a)

static bool DISPLAY_TRAJECTORY = false;

void InfinitelyNearBoundary::getExplorationPath(const cv::Mat &original_map,
                                                const cv::Mat &room_map,
                                                std::vector<geometry_msgs::Pose2D> &pose_path,
                                                std::vector<cv::Point> &point_path,
                                                std::vector<std::vector<geometry_msgs::Pose2D>> &complex_pose_path,
                                                float map_resolution,
                                                const cv::Point &starting_position,
                                                const cv::Point2d &map_origin,
                                                const double robot_radius,
                                                const int number_extension,
                                                const int distance_from_obstacles,
                                                const int multiple_contour_spacing,
                                                const int random_number_generation_ratio,
                                                const int boundary_min_area,
                                                const double path_eps) {

    double grid_spacing_in_meter = robot_radius * std::sqrt(2);//0.565685 网格正方形的边长
    double grid_spacing_in_pixel = grid_spacing_in_meter / map_resolution;
    int half_grid_spacing_as_int = (int) std::floor(0.5 * grid_spacing_in_pixel);

    cv::Mat principle_map;
    path_planner.downsampleMap(original_map.clone(), principle_map, 1, robot_radius, map_resolution);
    cv::Point reachablePoint(starting_position.x, starting_position.y);

    if (DISPLAY_TRAJECTORY) {
        cv::circle(principle_map, reachablePoint, 2, cv::Scalar(180), CV_FILLED);
        cv::imshow("principle_map", principle_map);
        cv::waitKey();
    }

    int num_it = (int) grid_spacing_in_pixel * 2;
    int origin_x = reachablePoint.x;
    int origin_y = reachablePoint.y;

    if (principle_map.at<unsigned char>(reachablePoint.y, reachablePoint.x) != 255) {
        LOG(INFO) << "InfinitelyNearBoundary : Find available points near the base station";

        for (int row = -num_it; row <= num_it; row++) {

            if (principle_map.at<unsigned char>(reachablePoint.y, reachablePoint.x) == 255)
                break;

            for (int col = -num_it; col <= num_it; col++) {
                if (principle_map.at<unsigned char>(origin_y + row, origin_x + col) == 255) {
                    reachablePoint.x = origin_x + col;
                    reachablePoint.y = origin_y + row;
                    break;
                }
            }
        }

    } else {
        LOG(INFO) << "InfinitelyNearBoundary : The location of the base station can ensure the arrival ...";
    }

    if (principle_map.at<unsigned char>(reachablePoint.y, reachablePoint.x) != 255)
        return;

    std::vector<std::vector<cv::Point2f>> middle_complex_path;
    for (int r = 0; r < number_extension; ++r) {

        int scale_in_pixel = (int) std::floor(half_grid_spacing_as_int +//机器人半径
                                              distance_from_obstacles +//与障碍物的间距
                                              grid_spacing_in_pixel * r +//多轮廓
                                              multiple_contour_spacing * r);
        LOG(INFO) << "(infinitely near boundary) 边界距离 scale_in_pixel: " << scale_in_pixel << " px";

        auto borderMat = room_map.clone();
        explorationErode(borderMat, borderMat, scale_in_pixel);

        if (DISPLAY_TRAJECTORY) {
            cv::imshow("m " + std::to_string(r) + " " + std::to_string(scale_in_pixel), borderMat);
            cv::waitKey();
        }

        std::vector<std::vector<cv::Point>> borderContours;
        cv::findContours(borderMat, borderContours, CV_RETR_CCOMP, CV_CHAIN_APPROX_NONE);

        for (const auto &borderContour: borderContours) {

            if (borderContour.empty())
                continue;

            cv::Mat room_mat = cv::Mat::zeros(room_map.rows, room_map.cols, CV_8UC1);
            cv::drawContours(room_mat, std::vector<std::vector<cv::Point> >(1, borderContour), -1, cv::Scalar(255),
                             CV_FILLED);

            if (DISPLAY_TRAJECTORY) {
                cv::imshow("m " + std::to_string(r) + " " + std::to_string(scale_in_pixel), room_mat);
                cv::waitKey();
            }

            int area_px = 0;
            for (int v = 0; v < room_mat.rows; ++v)
                for (int u = 0; u < room_mat.cols; ++u)
                    if (room_mat.at<uchar>(v, u) >= 250)
                        area_px++;
            auto area = area_px * map_resolution * map_resolution;
            if (area < boundary_min_area) {
//                LOG(INFO) << "InfinitelyNearBoundary : Discard small obstacles , area =" << area << " ...";
                continue;
            }

            bool isEligible = false;
            int accessibleCount = 0;

            int maxTraversal = borderContour.size() / random_number_generation_ratio;

            for (int i = 0; i < maxTraversal; i++) {
                auto random = rand() % borderContour.size();
                auto randomPoint = borderContour[random];
                double length = path_planner.planPath(original_map, reachablePoint, randomPoint,
                                                      1, robot_radius, map_resolution);
//                LOG(INFO) << "InfinitelyNearBoundary : r = " << std::to_string(r) << " , p = "
//                          << std::to_string(scale_in_pixel) << " , point : (" << randomPoint.x << ", " << randomPoint.y
//                          << ")" << "   " << length;
                if (length < 1e90) {
                    accessibleCount++;
                }
                if (accessibleCount >= maxTraversal * 0.5) {
                    isEligible = true;
                    break;
                }
            }

            if (isEligible) {
                std::vector<cv::Point2f> complex;
                for (const auto &point: borderContour) {
                    complex.push_back(point);
                }
                middle_complex_path.push_back(complex);
            } else {
                LOG(INFO) << "InfinitelyNearBoundary : maxTraversal =" << maxTraversal
                          << " , accessibleCount = " << accessibleCount;
            }

        }
    }

    if (middle_complex_path.empty()) {
        LOG(ERROR) << "Warning: there are no accessible points in this room.";
        return;
    }

    int path_eps_distance = static_cast<int>(std::floor(path_eps));
    for (auto &middle_complex: middle_complex_path) {
        // OpenCv
        std::vector<cv::Point> list;
        cv::approxPolyDP(middle_complex, list, 1.0, false);
        std::vector<Point2D> points;
        for (const auto &item: list) {
            points.emplace_back(item.x, item.y);
        }
        // 自实现
//        std::list<Point2D> line;
//        for (const auto &point: middle_complex) {
//            line.emplace_back(point.x, point.y);
//        }
//        DouglasPuecker2D<Point2D, Point2DAccessor> dp2d(line);
//        dp2d.simplify(1.0F);
//        std::list<Point2D> &list = dp2d.getLine();
//
//        std::vector<Point2D> points(list.begin(), list.end());

        const std::vector<Point2D> &neededPoints = splitPointsIfNeeded(points, path_eps_distance);

        std::vector<geometry_msgs::Pose2D> complex_poses = transformPointPathToPosePath(neededPoints);

        geometry_msgs::Pose2D head_pose;
        std::vector<geometry_msgs::Pose2D> complex_pose;
        for (int i = 0; i < complex_poses.size(); i++) {
            auto &pose = complex_poses[i];
            geometry_msgs::Pose2D current_pose;
            current_pose.x = (((room_map.cols - pose.x) * map_resolution) + map_origin.x);
            current_pose.y = (((room_map.rows - pose.y) * map_resolution) + map_origin.y);
            current_pose.theta = pose.theta;
            complex_pose.push_back(current_pose);
            // pose_path return
            pose_path.push_back(current_pose);
            if (i == 0) {
                head_pose.x = current_pose.x;
                head_pose.y = current_pose.y;
                head_pose.theta = current_pose.theta;
            } else if (i == complex_poses.size() - 1) {
                pose_path.push_back(head_pose);
            }
        }
        complex_pose_path.push_back(complex_pose);
    }

}

std::vector<geometry_msgs::Pose2D>
InfinitelyNearBoundary::transformPointPathToPosePath(const std::vector<Point2D> &point_path) {
    std::vector<geometry_msgs::Pose2D> pose_path;
    if (point_path.size() == 1) {
        geometry_msgs::Pose2D current_pose;
        current_pose.x = point_path[0].x;
        current_pose.y = point_path[0].y;
        current_pose.theta = 0.;
        pose_path.push_back(current_pose);
    } else {

        geometry_msgs::Pose2D zero_pose;
        zero_pose.x = point_path[0].x;
        zero_pose.y = point_path[0].y;
        zero_pose.theta = 0.;
        pose_path.push_back(zero_pose);

        for (size_t point_index = 1; point_index < point_path.size(); ++point_index) {
            const Point2D &current_point = point_path[point_index];

            geometry_msgs::Pose2D current_pose;
            current_pose.x = current_point.x;
            current_pose.y = current_point.y;
            current_pose.theta = 0.;
            Point2D vector(0, 0);
            if (point_index > 0) {
                vector = current_point - point_path[point_index - 1];
            } else if (point_path.size() >= 2) {
                vector = point_path[point_index + 1] - current_point;
            }
            if (vector.x != 0 || vector.y != 0) {
                current_pose.theta = std::atan2(vector.y, vector.x);
                pose_path.push_back(current_pose);
            }
        }
    }
    return pose_path;
}

std::vector<Point2D> InfinitelyNearBoundary::splitPoints(const Point2D &p1, const Point2D &p2, double distance) {
    std::vector<Point2D> split;

    double dx = p2.x - p1.x;
    double dy = p2.y - p1.y;
    double dist = std::sqrt(dx * dx + dy * dy);
    int numPoints = std::ceil(dist / distance);

    for (int i = 0; i <= numPoints; ++i) {
        double t = static_cast<double>(i) / numPoints;
        double x = p1.x + t * dx;
        double y = p1.y + t * dy;
        split.emplace_back(x, y);
    }

    return split;
}

std::vector<Point2D> InfinitelyNearBoundary::splitPointsIfNeeded(const std::vector<Point2D> &points, double distance) {
    std::vector<Point2D> results;

    for (size_t i = 0; i < points.size() - 1; ++i) {
        const Point2D &currentPoint = points[i];
        const Point2D &nextPoint = points[i + 1];

        double dx = nextPoint.x - currentPoint.x;
        double dy = nextPoint.y - currentPoint.y;
        double dist = std::sqrt(dx * dx + dy * dy);

        if (dist > distance) {
            std::vector<Point2D> interpolatedPoints = splitPoints(currentPoint, nextPoint, distance);
            results.insert(results.end(), interpolatedPoints.begin(), interpolatedPoints.end());
        } else {
            results.push_back(currentPoint);
        }
    }

    results.push_back(points.back());

    return results;
}