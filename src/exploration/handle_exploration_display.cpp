//
// Created by Looper on 2022/10/11.
//

#include "exploration/handle_exploration_display.h"
#include "segmentation/SegmentationCenter.h"
#include "segmentation/map_attribute.h"

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

#include "simulation.h"

void planning_pose_path_display(const cv::Mat &map, const cv::Point2d &map_origin,
                                const std::vector<std::vector<geometry_msgs::Pose2D>> &complex_path,
                                float resize,
                                const std::string &winname) {
    LOG_IF(INFO, DEBUG_EXPLORATION) << "压缩比例 ： " << resize;
    if (complex_path.empty()) {
        return;
    }
    const cv::Point &startPoint = MapAttribute::instance().getRobotPositionPoint(map);

    int cols = map.cols;
    int rows = map.rows;

    cv::Mat fov_path_map = map.clone();
    cv::resize(fov_path_map, fov_path_map, cv::Size(), resize, resize, cv::INTER_LINEAR);

    for (const auto &exploration_path: complex_path) {
        for (size_t step = 1; step < exploration_path.size(); ++step) {
            if (!exploration_path.empty())
                cv::circle(fov_path_map,
                           resize * cv::Point(
                                   cols - (exploration_path[0].x - map_origin.x) / map_resolution_from_subscription,
                                   rows - (exploration_path[0].y - map_origin.y) / map_resolution_from_subscription
                           ), 3, cv::Scalar(150), CV_FILLED);
            for (size_t i = 1; i <= step; ++i) {
                cv::Point p1(cols - (exploration_path[i - 1].x - map_origin.x) / map_resolution_from_subscription,
                             rows - (exploration_path[i - 1].y - map_origin.y) / map_resolution_from_subscription);
                cv::Point p2(cols - (exploration_path[i].x - map_origin.x) / map_resolution_from_subscription,
                             rows - (exploration_path[i].y - map_origin.y) / map_resolution_from_subscription);
                cv::circle(fov_path_map, resize * p2, 3, cv::Scalar(200), CV_FILLED);
                cv::line(fov_path_map, resize * p1, resize * p2, cv::Scalar(150), 1);
//                cv::Point p3(p2.x + 5 * cos(exploration_path[i].theta), p2.y + 5 * sin(exploration_path[i].theta));
//                if (i == step) {
//                    cv::circle(fov_path_map, resize * p2, 2, cv::Scalar(80), CV_FILLED);
//                    cv::line(fov_path_map, resize * p1, resize * p2, cv::Scalar(150), 1);
//                    cv::line(fov_path_map, resize * p2, resize * p3, cv::Scalar(50), 1);
//                }
            }

            cv::circle(fov_path_map, resize * startPoint, 3, cv::Scalar(160), CV_FILLED);

            cv::imshow(winname, fov_path_map);
            cv::waitKey();
        }
    }
    cv::imshow(winname, fov_path_map);
    cv::waitKey();
}

void planning_pose_path_display(const cv::Mat &map, const cv::Point2d &map_origin,
                                std::vector<geometry_msgs::Pose2D> exploration_path,
                                float resize, const std::string &winname) {
    LOG_IF(INFO, DEBUG_EXPLORATION) << "压缩比例 ： " << resize;
    if (exploration_path.empty()) {
        return;
    }
    const cv::Point &startPoint = MapAttribute::instance().getRobotPositionPoint(map);

    int cols = map.cols;
    int rows = map.rows;

    cv::Mat fov_path_map;
    for (size_t step = 1; step < exploration_path.size(); ++step) {
        fov_path_map = map.clone();
        cv::resize(fov_path_map, fov_path_map, cv::Size(), resize, resize, cv::INTER_LINEAR);
        if (!exploration_path.empty())
            cv::circle(fov_path_map,
                       resize * cv::Point(
                               cols - (exploration_path[0].x - map_origin.x) / map_resolution_from_subscription,
                               rows - (exploration_path[0].y - map_origin.y) / map_resolution_from_subscription
                       ), 2, cv::Scalar(150), CV_FILLED);
        for (size_t i = 1; i <= step; ++i) {
            cv::Point p1(cols - (exploration_path[i - 1].x - map_origin.x) / map_resolution_from_subscription,
                         rows - (exploration_path[i - 1].y - map_origin.y) / map_resolution_from_subscription);
            cv::Point p2(cols - (exploration_path[i].x - map_origin.x) / map_resolution_from_subscription,
                         rows - (exploration_path[i].y - map_origin.y) / map_resolution_from_subscription);
            cv::circle(fov_path_map, resize * p2, 2, cv::Scalar(200), CV_FILLED);
            cv::line(fov_path_map, resize * p1, resize * p2, cv::Scalar(150), 1);
            cv::Point p3(p2.x + 5 * cos(exploration_path[i].theta), p2.y + 5 * sin(exploration_path[i].theta));
            if (i == step) {
                cv::circle(fov_path_map, resize * p2, 2, cv::Scalar(80), CV_FILLED);
                cv::line(fov_path_map, resize * p1, resize * p2, cv::Scalar(150), 1);
                cv::line(fov_path_map, resize * p2, resize * p3, cv::Scalar(50), 1);
            }
        }

        cv::circle(fov_path_map, resize * startPoint, 3, cv::Scalar(160), CV_FILLED);

        cv::imshow(winname, fov_path_map);
        cv::waitKey();
    }
    cv::imshow(winname, fov_path_map);
    cv::waitKey();

//    const std::string &basicString = path::robot_slam_map_path();
//    auto uuid = boost::uuids::to_string(boost::uuids::random_generator()());
//    auto randomPngPath = basicString + uuid + ".png";
//    auto depth = fov_path_map.clone();
//    CvUtils::savePng(randomPngPath, depth);
}

void planning_point_path_display(const cv::Mat &map, std::vector<cv::Point> point_path, float resize,
                                 const std::string &winname) {
    LOG_IF(INFO, DEBUG_EXPLORATION) << "压缩比例 ： " << resize;
    if (point_path.empty()) {
        return;
    }
    cv::Mat fov_path_map;
    for (size_t step = 1; step < point_path.size(); ++step) {
        fov_path_map = map.clone();
        cv::resize(fov_path_map, fov_path_map, cv::Size(), resize, resize, cv::INTER_LINEAR);
        if (!point_path.empty())
            cv::circle(fov_path_map, resize * cv::Point(point_path[0].x, point_path[0].y),
                       2, cv::Scalar(150), CV_FILLED);
        for (size_t i = 1; i <= step; ++i) {
            cv::Point p1(point_path[i - 1].x, point_path[i - 1].y);
            cv::Point p2(point_path[i].x, point_path[i].y);
            cv::circle(fov_path_map, resize * p2, 2, cv::Scalar(200), CV_FILLED);
            cv::line(fov_path_map, resize * p1, resize * p2, cv::Scalar(150), 1);

            if (i == step) {
                cv::circle(fov_path_map, resize * p2, 2, cv::Scalar(80), CV_FILLED);
                cv::line(fov_path_map, resize * p1, resize * p2, cv::Scalar(150), 1);
            }
        }
//        cv::imshow(winname, fov_path_map);
//        cv::waitKey();
    }
    cv::imshow(winname, fov_path_map);
    cv::waitKey();
}

void center_point_display(const cv::Mat &map, const cv::Point &point, float resize, const std::string &winname) {
    cv::Mat copy_mat;
    cv::resize(map.clone(), copy_mat, cv::Size(), resize, resize, cv::INTER_LINEAR);
    cv::circle(copy_mat, point * resize, 2, cv::Scalar(150), CV_FILLED);
    cv::imshow(winname, copy_mat);
    cv::waitKey();
}

void planning_point_segmentation_path_display(const cv::Mat &map,
                                              std::vector<int> optimal_order,
                                              std::vector<cv::Point> polygon_centers,
                                              std::map<int, std::vector<cv::Point>> each_map,
                                              std::map<int, std::vector<std::vector<cv::Point>>> each_contours_map,
                                              float resize, const std::string &winname) {
    const cv::Mat &temp_map = map.clone();
    cv::Mat show_map;
    cv::resize(temp_map, show_map, cv::Size(), resize, resize, cv::INTER_LINEAR);
    cv::Point lastPoint;
    for (int i = 0; i < optimal_order.size(); ++i) {
        int pos = optimal_order[i];

        std::vector<std::vector<cv::Point>> &each_contours = each_contours_map[pos];
        std::vector<std::vector<cv::Point>> resize_contours;
        for (const auto &vector: each_contours) {
            std::vector<cv::Point> child_resize_contours;
            for (const auto &item: vector) {
                child_resize_contours.push_back(item * resize);
            }
            resize_contours.push_back(child_resize_contours);
        }

        cv::Mat room_mat = cv::Mat::zeros(map.rows * resize, map.cols * resize, CV_8UC1);
        cv::drawContours(room_mat, resize_contours, -1, cv::Scalar(255), CV_FILLED);

        cv::Mat erode_mat;
        cv::erode(room_mat, erode_mat, cv::Mat(), cv::Point(-1, -1), 1);

        cv::bitwise_xor(room_mat, erode_mat, erode_mat);
        cv::bitwise_xor(show_map, erode_mat, show_map);

        cv::Point point = polygon_centers[pos];

        std::vector<cv::Point> &each_path = each_map[pos];
        cv::circle(show_map, point * resize, 6, cv::Scalar(100), CV_FILLED);
        cv::putText(show_map, std::to_string(i), point * resize, cv::FONT_HERSHEY_TRIPLEX,
                    0.8, cv::Scalar(128), 1, CV_AA);

        for (int y = 0; y < each_path.size(); ++y) {
            auto currentPoint = each_path[y];
            if (i != 0 || y != 0) {
                cv::line(show_map, lastPoint * resize, currentPoint * resize, cv::Scalar(200), 1);
            }
            if (y == 0 || y == each_path.size() - 1) {
                cv::circle(show_map, currentPoint * resize, 4, cv::Scalar(150), CV_FILLED);
            }
            cv::circle(show_map, currentPoint * resize, 2, cv::Scalar(150), CV_FILLED);
            lastPoint = currentPoint;

//            cv::imshow("show_map", show_map);
//            cv::waitKey();
        }
    }
    cv::imshow(winname, show_map);
    cv::waitKey();
}

void save_planning_point_segmentation_path(const cv::Mat &map, cv::Mat segmented_map, cv::Point start_point,
                                           std::vector<int> optimal_order,
                                           std::vector<cv::Point> polygon_centers,
                                           std::map<int, std::vector<cv::Point>> each_map,
                                           std::map<int, std::vector<std::vector<cv::Point>>> each_contours_map) {
    const cv::Mat &clone_map = map.clone();

    cv::Point lastPoint;
    for (int i = 0; i < optimal_order.size(); ++i) {
        int pos = optimal_order[i];

        std::vector<std::vector<cv::Point>> &each_contours = each_contours_map[pos];
        std::vector<std::vector<cv::Point>> resize_contours;
        for (const auto &vector: each_contours) {
            std::vector<cv::Point> child_resize_contours;
            for (const auto &item: vector) {
                child_resize_contours.push_back(item);
            }
            resize_contours.push_back(child_resize_contours);
        }

        cv::Mat room_mat = cv::Mat::zeros(map.rows, map.cols, CV_8UC1);
        cv::drawContours(room_mat, resize_contours, -1, cv::Scalar(255), CV_FILLED);

        cv::Mat erode_mat;
        cv::erode(room_mat, erode_mat, cv::Mat(), cv::Point(-1, -1), 1);

        cv::bitwise_xor(room_mat, erode_mat, erode_mat);
        cv::bitwise_xor(clone_map, erode_mat, clone_map);

        cv::Point point = polygon_centers[pos];
        int type = segmented_map.type();

        std::vector<cv::Point> &each_path = each_map[pos];
        cv::circle(segmented_map, point, 3, cv::Scalar(30000), CV_FILLED);
        cv::putText(segmented_map, std::to_string(i), point, cv::FONT_HERSHEY_TRIPLEX,
                    0.8, cv::Scalar(0), 1, CV_AA);
        cv::circle(segmented_map, start_point, 3, cv::Scalar(50000), CV_FILLED);

        for (int y = 0; y < each_path.size(); ++y) {
            auto currentPoint = each_path[y];
            if (i != 0 || y != 0) {
                cv::line(segmented_map, lastPoint, currentPoint, cv::Scalar(20000), 1);
            }
            if (y == 0 || y == each_path.size() - 1) {
                cv::circle(segmented_map, currentPoint, 2, cv::Scalar(10000), CV_FILLED);
            }
            cv::circle(segmented_map, currentPoint, 1, cv::Scalar(40000), CV_FILLED);
            lastPoint = currentPoint;

        }
    }

    auto uuid = boost::uuids::to_string(boost::uuids::random_generator()());
    auto randomPngPath = path::robot_slam_map_dir() + uuid + ".png";
    auto depth = segmented_map.clone();
    CvUtils::savePng(randomPngPath, depth);
}