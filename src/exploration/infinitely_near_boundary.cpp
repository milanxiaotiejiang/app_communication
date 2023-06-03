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
static bool BOUNDARY_DISTANCE = false;

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

        // step 1 方差最大，均值8.x可达到50的方差
//        int scale_in_pixel = (int) std::floor(half_grid_spacing_as_int +//机器人半径
//                                              distance_from_obstacles +//与障碍物的间距
//                                              grid_spacing_in_pixel * r +//多轮廓
//                                              multiple_contour_spacing * r);
//        auto borderMat = room_map.clone();
//        explorationErode(borderMat, borderMat, cv::MORPH_CROSS, scale_in_pixel);
        // step 2 方差应为最小，均值8.x的方差可减少到不到30，但是distance_from_obstacles的变换区间最小，只有0-2
//        int scale_in_pixel = (int) std::floor(half_grid_spacing_as_int +//机器人半径
//                                              distance_from_obstacles +//与障碍物的间距
//                                              grid_spacing_in_pixel * r +//多轮廓
//                                              multiple_contour_spacing * r);
//        auto borderMat = room_map.clone();
//        cv::erode(borderMat, borderMat, cv::Mat(), cv::Point(-1, -1), scale_in_pixel);
        // step 3
        int scale_in_pixel = (int) std::floor(0 +//机器人半径
                                              distance_from_obstacles +//与障碍物的间距
                                              grid_spacing_in_pixel * r +//多轮廓
                                              multiple_contour_spacing * r);
        auto borderMat = room_map.clone();
        explorationErode(borderMat, borderMat, cv::MORPH_RECT, half_grid_spacing_as_int);
        cv::erode(borderMat, borderMat, cv::Mat(), cv::Point(-1, -1), scale_in_pixel);
        // step 4
//        int scale_in_pixel = (int) std::floor(0 +//机器人半径
//                                              distance_from_obstacles +//与障碍物的间距
//                                              grid_spacing_in_pixel * r +//多轮廓
//                                              multiple_contour_spacing * r);
//        auto borderMat = room_map.clone();
//        cv::erode(borderMat, borderMat, cv::Mat(), cv::Point(-1, -1), half_grid_spacing_as_int);
//        explorationErode(borderMat, borderMat, cv::MORPH_RECT, scale_in_pixel);

        LOG(INFO) << "(infinitely near boundary) 边界距离 scale_in_pixel: " << (half_grid_spacing_as_int + scale_in_pixel)
                  << " px";

        //下列分别测试四种 step，采用 step3 为主

        /**
         * step 1  distance_from_obstacles = 8 explorationErode
         * 边界 总个数 1346 总距离 11134.2 最大 35.0571 最小 0.707107 均值 8.27208 方差 47.5207 标准差 6.89352
         * 抽希 总个数 140 总距离 1239.17 最大 33 最小 0.707107 均值 8.85121 方差 65.8489 标准差 8.11473
         * 插值 总个数 326 总距离 2731.32 最大 35.0571 最小 0.707107 均值 8.37829 方差 53.2001 标准差 7.29384
         *
         * step 2  distance_from_obstacles = 2 cv::erode
         * 边界 总个数 1443 总距离 11859.5 最大 35.0571 最小 5 均值 8.21862 方差 31.6296 标准差 5.62402
         * 抽希 总个数 136 总距离 1186.46 最大 28.0713 最小 5 均值 8.72398 方差 27.6568 标准差 5.25898
         * 插值 总个数 337 总距离 2824.26 最大 31.8084 最小 4.21637 均值 8.38059 方差 29.9615 标准差 5.47371
         *
         * step 3  distance_from_obstacles = 3 explorationErode + cv::erode
         * 边界 总个数 1592 总距离 11614.6 最大 35.0143 最小 4 均值 7.2956 方差 31.8549 标准差 5.64402
         * 抽希 总个数 160 总距离 1217.29 最大 26.8701 最小 4 均值 7.60807 方差 26.6954 标准差 5.16676
         * 插值 总个数 387 总距离 2842.77 最大 31.6189 最小 3 均值 7.34566 方差 29.1599 标准差 5.39999
         * step 3  distance_from_obstacles = 4 explorationErode + cv::erode
         * 边界 总个数 1443 总距离 11859.5 最大 35.0571 最小 5 均值 8.21862 方差 31.6296 标准差 5.62402
         * 抽希 总个数 136 总距离 1186.46 最大 28.0713 最小 5 均值 8.72398 方差 27.6568 标准差 5.25898
         * 插值 总个数 337 总距离 2824.26 最大 31.8084 最小 4.21637 均值 8.38059 方差 29.9615 标准差 5.47371
         *
         * step 4  distance_from_obstacles = 5 cv::erode + explorationErode + MORPH_CROSS
         * 边界 总个数 1420 总距离 12434.3 最大 35.0571 最小 4.94975 均值 8.75656 方差 44.5342 标准差 6.6734
         * 抽希 总个数 148 总距离 1363.26 最大 33 最小 4.94975 均值 9.2112 方差 42.6706 标准差 6.53227
         * 插值 总个数 352 总距离 3167.41 最大 33 最小 4.21637 均值 8.99833 方差 45.1578 标准差 6.71995
         * step 4  distance_from_obstacles = 4 cv::erode + explorationErode + MORPH_CROSS
         * 边界 总个数 1437 总距离 11923.1 最大 35.0143 最小 4 均值 8.29723 方差 45.6431 标准差 6.75597
         * 抽希 总个数 149 总距离 1303.01 最大 32 最小 4 均值 8.74505 方差 43.7154 标准差 6.61176
         * 插值 总个数 356 总距离 2992.14 最大 32.1267 最小 3.34994 均值 8.40489 方差 43.5039 标准差 6.59575
         * step 4  distance_from_obstacles = 3 cv::erode + explorationErode + MORPH_CROSS
         * 边界 总个数 1426 总距离 10023.3 最大 35.0143 最小 4 均值 7.02898 方差 33.0637 标准差 5.7501
         * 抽希 总个数 150 总距离 1155.43 最大 26.163 最小 4 均值 7.70289 方差 31.9121 标准差 5.64908
         * 插值 总个数 350 总距离 2513.13 最大 31.6189 最小 3.33334 均值 7.18038 方差 31.2051 标准差 5.58615
         *
         * step 4  distance_from_obstacles = 5 cv::erode + explorationErode + MORPH_RECT
         * 边界 总个数 1432 总距离 13060.6 最大 35.0571 最小 5 均值 9.12051 方差 45.4789 标准差 6.74381
         * 抽希 总个数 98 总距离 780.743 最大 33 最小 5 均值 7.96676 方差 30.0511 标准差 5.48189
         * 插值 总个数 310 总距离 2712.43 最大 33 最小 4 均值 8.74976 方差 41.8465 标准差 6.46889
         * step 4  distance_from_obstacles = 4 cv::erode + explorationErode + MORPH_RECT
         * 边界 总个数 1461 总距离 12434.3 最大 35.0143 最小 4 均值 8.51078 方差 45.9715 标准差 6.78023
         * 抽希 总个数 110 总距离 848.693 最大 32 最小 4 均值 7.71539 方差 30.3183 标准差 5.5062
         * 插值 总个数 322 总距离 2647.03 最大 32.1267 最小 3.5 均值 8.22059 方差 40.7023 标准差 6.37984
         * step 4  distance_from_obstacles = 3 cv::erode + explorationErode + MORPH_RECT
         * 边界 总个数 1451 总距离 10420.5 最大 35.0143 最小 4 均值 7.18163 方差 33.0117 标准差 5.74558
         * 抽希 总个数 140 总距离 1081.78 最大 26.8701 最小 4 均值 7.72698 方差 28.8617 标准差 5.37231
         * 插值 总个数 349 总距离 2557.1 最大 31.6189 最小 3.48008 均值 7.32694 方差 30.8252 标准差 5.55204
         */

        //以下为仿真数据，主要测试 cv::erode 与 explorationErode，MORPH_RECT 与 MORPH_CROSS 对规划的影响
        //得出结论，cv::erode + MORPH_RECT 方式较好，但 cv::erode 粒度较大
        /*
         * scale_in_pixel = 10
         *
         * cv::MORPH_CROSS
         * 边界 总个数 1785 总距离 17852.4 最大 58 最小 0.707107 均值 10.0014 方差 159.011 标准差 12.61
         * 抽希 总个数 134 总距离 1237.76 最大 57.8705 最小 0.707107 均值 9.23702 方差 113.043 标准差 10.6322
         * 插值 总个数 369 总距离 3570.34 最大 57.875 最小 0.707107 均值 9.67571 方差 147.697 标准差 12.1531
         *
         * cv::MORPH_RECT
         * 边界 总个数 1786 总距离 18530.1 最大 58 最小 4 均值 10.3752 方差 157.484 标准差 12.5493
         * 抽希 总个数 88 总距离 906.92 最大 57.3149 最小 4 均值 10.3059 方差 107.152 标准差 10.3514
         * 插值 总个数 356 总距离 3654.44 最大 57.9 最小 3.125 均值 10.2653 方差 144.905 标准差 12.0377
         *
         * erode scale_in_pixel
         * 边界 总个数 1666 总距离 27822.8 最大 63 最小 10 均值 16.7004 方差 140.377 标准差 11.8481
         * 抽希 总个数 84 总距离 1342.47 最大 58.4637 最小 10 均值 15.9818 方差 94.4449 标准差 9.71828
         * 插值 总个数 332 总距离 5442.86 最大 62.8 最小 9.25 均值 16.3942 方差 130.379 标准差 11.4184
         *
         * erode std::floor(scale_in_pixel / 2) = 5
         * 边界 总个数 1530 总距离 8390.66 最大 14.8661 最小 5 均值 5.48409 方差 1.8715 标准差 1.36803
         * 抽希 总个数 78 总距离 507.963 最大 14.8661 最小 5 均值 6.51235 方差 3.83287 标准差 1.95777
         * 插值 总个数 314 总距离 1786.97 最大 14.8661 最小 4.12311 均值 5.69097 方差 2.75298 标准差 1.65921
         */

        /*
         * scale_in_pixel = 5
         *
         * cv::MORPH_CROSS
         * 边界 总个数 1546 总距离 3317.26 平均 2.14571 最大 11.1803 最小 0.707107 均值 2.14571 方差 1.20512 标准差 1.09778
         * 抽希 总个数 102 总距离 275.846 平均 2.70437 最大 11.0454 最小 0.707107 均值 2.70437 方差 5.07852 标准差 2.25356
         * 插值 总个数 323 总距离 685.791 平均 2.12319 最大 11.0454 最小 0.707107 均值 2.12319 方差 1.83732 标准差 1.35548
         *
         * cv::MORPH_RECT
         * 边界 总个数 1574 总距离 3646.8 最大 11.4018 最小 2 均值 2.3169 方差 1.47919 标准差 1.21622
         * 抽希 总个数 88 总距离 272.055 最大 11.4018 最小 2 均值 3.09153 方差 4.5788 标准差 2.13981
         * 插值 总个数 318 总距离 798.793 最大 11.4018 最小 1.125 均值 2.51193 方差 2.53355 标准差 1.59171
         *
         * erode scale_in_pixel
         * 边界 总个数 1530 总距离 8390.66 最大 14.8661 最小 5 均值 5.48409 方差 1.8715 标准差 1.36803
         * 抽希 总个数 78 总距离 507.963 最大 14.8661 最小 5 均值 6.51235 方差 3.83287 标准差 1.95777
         * 插值 总个数 314 总距离 1786.97 最大 14.8661 最小 4.12311 均值 5.69097 方差 2.75298 标准差 1.65921
         *
         * erode std::floor(scale_in_pixel / 2) = 2
         * 边界 总个数 1583 总距离 3449.4 最大 11.4018 最小 2 均值 2.17903 方差 0.699719 标准差 0.836492
         * 抽希 总个数 93 总距离 253.536 最大 11.4018 最小 2 均值 2.72619 方差 2.23453 标准差 1.49484
         * 插值 总个数 324 总距离 732.183 最大 11.4018 最小 1.10526 均值 2.25982 方差 1.30407 标准差 1.14196
         */

        if (DISPLAY_TRAJECTORY) {
            cv::imshow("m " + std::to_string(r) + " " + std::to_string(scale_in_pixel), borderMat);
            cv::waitKey();
        }

        std::vector<std::vector<cv::Point>> borderContours;
        cv::findContours(borderMat, borderContours, CV_RETR_CCOMP, CV_CHAIN_APPROX_NONE);

        if (DISPLAY_TRAJECTORY) {
            auto show_map = original_map.clone();
            cv::resize(show_map, show_map, cv::Size(), 1, 1, cv::INTER_LINEAR);
            std::vector<std::vector<cv::Point>> showBorderContours;
            for (const auto &contours: borderContours) {
                std::vector<cv::Point> resizeContours;
                for (const auto &item: contours) {
                    resizeContours.push_back(item * 1);
                }
                showBorderContours.push_back(resizeContours);
            }
            cv::drawContours(show_map, showBorderContours, -1, cv::Scalar(128), 1);
            cv::imshow("原图绘制边界线", show_map);
            cv::waitKey();
        }

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
        boundary_distance(original_map, middle_complex, "边界");

        // OpenCv
        std::vector<cv::Point2f> list;
        cv::approxPolyDP(middle_complex, list, 0.5, false);
        boundary_distance(original_map, list, "抽希");

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
        boundary_distance(original_map, neededPoints, "插值");

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

void
InfinitelyNearBoundary::boundary_distance(const cv::Mat &original_map, const std::vector<cv::Point2f> &points,
                                          const std::string &name) const {
    if (!BOUNDARY_DISTANCE)
        return;

    auto display_map = original_map.clone();
    std::vector<std::vector<cv::Point>> borderContours;
    cv::findContours(display_map, borderContours, CV_RETR_LIST, CV_CHAIN_APPROX_NONE);
    std::vector<std::vector<cv::Point>> maxContours;
    size_t area = 0;
    for (const auto &item: borderContours) {
        if (item.size() > area) {
            area = item.size();
            maxContours.clear();
            maxContours.push_back(item);
        }
    }
    double distance = 0;
    double max = 0;
    double min = 100;
    std::vector<double> vecNums;
    for (const auto &item: points) {
        auto pointPolygonTest = cv::pointPolygonTest(maxContours[0], item, true);
//        LOG(ERROR) << "pointPolygonTest : " << pointPolygonTest;
        distance = distance + pointPolygonTest;
        if (pointPolygonTest > max) {
            max = pointPolygonTest;
        }
        if (pointPolygonTest < min) {
            min = pointPolygonTest;
        }
        vecNums.push_back(pointPolygonTest);
    }
    double sumNum = accumulate(vecNums.begin(), vecNums.end(), 0.0);
    double mean = sumNum / vecNums.size(); //均值
    double accum = 0.0;
    for_each(vecNums.begin(), vecNums.end(), [&](const double d) {
        accum += (d - mean) * (d - mean);
    });
    double variance = accum / vecNums.size(); //方差
    double stdev = sqrt(variance); //标准差

    LOG(ERROR) << name
               << " 总个数 " << points.size()
               << " 总距离 " << distance
               << " 最大 " << max
               << " 最小 " << min
               << " 均值 " << mean
               << " 方差 " << variance
               << " 标准差 " << stdev;
//    cv::Mat d_map = cv::Mat::zeros(original_map.rows, original_map.cols, CV_8UC1);
//    cv::drawContours(d_map, maxContours, -1, cv::Scalar(255), 1);
//    cv::imshow("1", d_map);
//    cv::waitKey();
}

void
InfinitelyNearBoundary::boundary_distance(const cv::Mat &original_map, const std::vector<Point2D> &points,
                                          const std::string &name) const {
    std::vector<cv::Point2f> displays;
    for (const auto &item: points) {
        displays.emplace_back(item.x, item.y);
    }
    boundary_distance(original_map, displays, name);
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