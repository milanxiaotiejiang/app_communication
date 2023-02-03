//
// Created by Looper on 2023/2/2.
//

#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include "exploration/infinitely_near_boundary.h"
#include "glog/logging.h"
#include "exploration/line.h"
#include "exploration/A_star_pathplanner.h"

#define random(a, b) (rand() % (b - a) + a)

static bool DISPLAY_TRAJECTORY = false;

void InfinitelyNearBoundary::getExplorationPath(const cv::Mat &original_map, const cv::Mat &room_map,
                                                std::vector<geometry_msgs::Pose2D> &pose_path,
                                                std::vector<cv::Point> &point_path,
                                                const float map_resolution,
                                                const cv::Point &starting_position,
                                                const cv::Point2d &map_origin,
                                                double robot_radius,
                                                int number_extension,
                                                int distance_from_obstacles,
                                                int multiple_contour_spacing,
                                                int random_number_generation_ratio,
                                                int boundary_min_area) {

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


    std::vector<cv::Point2f> middle_point_path;
    for (int r = 0; r < number_extension; ++r) {

        int scale_in_pixel = half_grid_spacing_as_int +//机器人半径
                             distance_from_obstacles +//与障碍物的间距
                             half_grid_spacing_as_int * 2 * r +//多轮廓
                             multiple_contour_spacing * r;
        LOG(INFO) << "(infinitely near boundary) scale_in_pixel: " << scale_in_pixel;

        auto borderMat = room_map.clone();
        cv::erode(borderMat, borderMat, cv::Mat(), cv::Point(1, 1), scale_in_pixel);

        if (DISPLAY_TRAJECTORY) {
            cv::imshow("m " + std::to_string(r) + " " + std::to_string(scale_in_pixel), borderMat);
            cv::waitKey();
        }

        std::vector<std::vector<cv::Point>> borderContours;
        cv::findContours(borderMat, borderContours, CV_RETR_CCOMP, CV_CHAIN_APPROX_NONE);

        for (const auto &vector: borderContours) {

            if (vector.empty())
                continue;

            cv::Mat room_mat = cv::Mat::zeros(room_map.rows, room_map.cols, CV_8UC1);
            cv::drawContours(room_mat, std::vector<std::vector<cv::Point> >(1, vector), -1, cv::Scalar(255), CV_FILLED);

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
                LOG(INFO) << "InfinitelyNearBoundary : Discard small obstacles , area =" << area << " ...";
                continue;
            }

            bool isEligible = false;
            int accessibleCount = 0;

            int maxTraversal = vector.size() / random_number_generation_ratio;

            for (int i = 0; i < maxTraversal; i++) {
                auto random = rand() % vector.size();
                auto randomPoint = vector[random];
                double length = path_planner.planPath(original_map, reachablePoint, randomPoint,
                                                      1, robot_radius, map_resolution);
//                LOG(INFO) << "InfinitelyNearBoundary : r = " << std::to_string(r) << " , p = "
//                          << std::to_string(scale_in_pixel) << " , point : (" << randomPoint.x << ", " << randomPoint.y
//                          << ")" << "   " << length;
                if (length < 1e90) {
                    accessibleCount++;
                }
                if (accessibleCount > maxTraversal * 0.5) {
                    isEligible = true;
                    break;
                }
            }

            if (isEligible) {
                for (const auto &point: vector) {
                    middle_point_path.push_back(point);
                }
                middle_point_path.push_back(vector.front());
            } else {
                LOG(INFO) << "InfinitelyNearBoundary : maxTraversal =" << maxTraversal << " , accessibleCount = "
                          << accessibleCount;
            }

        }
    }

    if (middle_point_path.empty()) {
        LOG(ERROR) << "Warning: there are no accessible points in this room.";
        return;
    }

    if (DISPLAY_TRAJECTORY) {
        auto show_map = original_map.clone();
        for (const auto &item: middle_point_path) {
            cv::circle(show_map, item, 1, cv::Scalar(64), CV_FILLED);
        }
        cv::imshow("show_map", show_map);
        cv::waitKey();
    }

    std::vector<geometry_msgs::Pose2D> fov_poses;
    transformPointPathToPosePath(middle_point_path, fov_poses);

    for (std::vector<geometry_msgs::Pose2D>::iterator pose = fov_poses.begin(); pose != fov_poses.end(); ++pose) {
        geometry_msgs::Pose2D current_pose;
        current_pose.x = (((room_map.cols - pose->x) * map_resolution) + map_origin.x);
        current_pose.y = (((room_map.rows - pose->y) * map_resolution) + map_origin.y);
        current_pose.theta = pose->theta;
        pose_path.push_back(current_pose);
    }

    optimizePathColumn(pose_path);
}

void InfinitelyNearBoundary::transformPointPathToPosePath(const std::vector<cv::Point2f> &point_path,
                                                          std::vector<geometry_msgs::Pose2D> &pose_path) {
    if (point_path.size() == 1) {
        geometry_msgs::Pose2D current_pose;
        current_pose.x = point_path[0].x;
        current_pose.y = point_path[0].y;
        current_pose.theta = 0.;
        pose_path.push_back(current_pose);
    } else {
        for (size_t point_index = 1; point_index < point_path.size(); ++point_index) {
            const cv::Point2f &current_point = point_path[point_index];

            geometry_msgs::Pose2D current_pose;
            current_pose.x = current_point.x;
            current_pose.y = current_point.y;
            current_pose.theta = 0.;
            cv::Point2f vector(0, 0);
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
}

void InfinitelyNearBoundary::optimizePathColumn(std::vector<geometry_msgs::Pose2D> &vector) {
    if (vector.size() < 3) {
        return;
    }
    std::vector<geometry_msgs::Pose2D> optimize;
    optimize.push_back(vector[0]);
    geometry_msgs::Pose2D last = vector[0];
    for (int i = 1; i < vector.size() - 1; ++i) {
        if (!conversion::one_line(last, vector[i], vector[i + 1])) {
            if (sqrt(pow(vector[i].x - vector[i + 1].x, 2) + pow(vector[i + 1].y - vector[i].y, 2)) < 1.0) {
                last = vector[i];
                optimize.push_back(vector[i]);
            }
        }
    }
    optimize.push_back(vector[vector.size() - 1]);

    vector.clear();
    for (const auto &item: optimize) {
        vector.push_back(item);
    }
}
