//
// Created by Looper on 2023/2/2.
//

#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include "exploration/infinitely_near_boundary.h"
#include "glog/logging.h"
#include "exploration/line.h"

static bool DISPLAY_TRAJECTORY = false;

void InfinitelyNearBoundary::getExplorationPath(const cv::Mat &room_map, std::vector<geometry_msgs::Pose2D> &pose_path,
                                                std::vector<cv::Point> &point_path,
                                                const float map_resolution, const cv::Point2d map_origin,
                                                int number_extension, int distance_from_obstacles,
                                                int multiple_contour_spacing) {

    if (DISPLAY_TRAJECTORY) {
        cv::imshow("room_map", room_map);
        cv::waitKey();
    }

    std::vector<cv::Point2f> middle_point_path;

    for (int r = 0; r < number_extension; ++r) {

        int scale_in_pixel = distance_from_obstacles + multiple_contour_spacing * r;
        LOG(INFO) << "(infinitely near boundary) scale_in_pixel: " << scale_in_pixel;

        auto borderMat = room_map.clone();
        cv::erode(borderMat, borderMat, cv::Mat(), cv::Point(1, 1), scale_in_pixel);
        std::vector<std::vector<cv::Point>> borderContours;
        cv::findContours(borderMat, borderContours, CV_RETR_CCOMP, CV_CHAIN_APPROX_NONE);

        for (const auto &vector: borderContours) {

            cv::Mat room_mat = cv::Mat::zeros(room_map.rows, room_map.cols, CV_8UC1);
            cv::drawContours(room_mat, std::vector<std::vector<cv::Point> >(1, vector), -1, cv::Scalar(255), CV_FILLED);
            cv::imshow("room_mat", room_mat);
            cv::waitKey();

            for (const auto &point: vector) {
                middle_point_path.push_back(point);
            }
        }
    }

    if (middle_point_path.empty()) {
        LOG(ERROR) << "Warning: there are no accessible points in this room.";
        return;
    }

    if (DISPLAY_TRAJECTORY) {
        auto show_map = room_map.clone();
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
