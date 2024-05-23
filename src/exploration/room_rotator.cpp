//
// Created by Looper on 2022/9/30.
//

#include "exploration/room_rotator.h"
#include "exploration/histogram.h"
#include "exploration/cv_extend.h"
#include "simulation.h"

static bool DISPLAY_ROOM_ROTATOR = false;

void RoomRotator::rotateRoom(const cv::Mat &room_map, cv::Mat &rotated_room_map, const cv::Mat &R,
                             const cv::Rect &bounding_rect) {
    cv::warpAffine(room_map, rotated_room_map, R, bounding_rect.size(), cv::INTER_AREA);

//    cv::Mat dst;
//    cv::pyrDown(rotated_room_map, dst, cv::Size(rotated_room_map.cols / 2, rotated_room_map.rows / 2));
//    cv::pyrUp(dst, rotated_room_map, rotated_room_map.size());
//    cv::resize(rotated_room_map, dst, cv::Size(), 2.0, 2.0, cv::INTER_LINEAR);
//    cv::GaussianBlur(dst, dst, cv::Size(3, 3), 0, 0);
//    cv::resize(dst, rotated_room_map, rotated_room_map.size(), 2.0, 2.0, cv::INTER_LINEAR);

    cv::threshold(rotated_room_map, rotated_room_map, 127, 255, cv::THRESH_BINARY);
}

double RoomRotator::computeRoomRotationMatrix(const cv::Mat &room_map, cv::Mat &R, cv::Rect &bounding_rect,
                                              const double map_resolution, const cv::Point *center,
                                              const double rotation_offset) {
    double rotation_angle = computeRoomMainDirection(room_map, map_resolution) + rotation_offset;
//    LOG_IF(INFO, DEBUG_EXPLORATION) << "RoomRotator::computeRoomRotationMatrix: main rotation angle: " << rotation_angle
//              << "   (rotation_angle * 180) / CV_PI = " << (rotation_angle * 180) / CV_PI;

    cv::Point center_of_rotation;
    if (center == nullptr) {
        cv::Point min_room, max_room;
        getMinMaxCoordinates(room_map, min_room, max_room);
        center_of_rotation.x = 0.5 * (min_room.x + max_room.x);
        center_of_rotation.y = 0.5 * (min_room.y + max_room.y);
    } else
        center_of_rotation = *center;

    R = cv::getRotationMatrix2D(center_of_rotation, (rotation_angle * 180) / CV_PI, 1.0);

    //旋转中心点、尺寸大小和旋转角度
    bounding_rect = cv::RotatedRect(center_of_rotation, room_map.size(), (rotation_angle * 180) / CV_PI).boundingRect();
    R.at<double>(0, 2) += 0.5 * bounding_rect.width - center_of_rotation.x;
    R.at<double>(1, 2) += 0.5 * bounding_rect.height - center_of_rotation.y;

    return rotation_angle;
}

double RoomRotator::computeRoomMainDirection(const cv::Mat &room_map, const double map_resolution) {
    const double map_resolution_inverse = 1. / map_resolution;

    cv::Mat edge_map;
    cv::Canny(room_map, edge_map, 50, 150, 3);

    if (DISPLAY_ROOM_ROTATOR) {
        cv::imshow("edge_map", edge_map);
        cv::waitKey();
    }

    std::vector<cv::Vec4i> lines;
    double min_line_length = 1.0;    // in [m]
    for (; min_line_length > 0.1; min_line_length -= 0.2) {
        cv::HoughLinesP(edge_map, lines, 1, CV_PI / 180,
                        min_line_length * map_resolution_inverse,
                        min_line_length * map_resolution_inverse,
                        1.5 * min_line_length * map_resolution_inverse
        );
        cv::Mat room_hough = edge_map.clone();
        for (size_t i = 0; i < lines.size(); ++i) {
            cv::Point p1(lines[i][0], lines[i][1]), p2(lines[i][2], lines[i][3]);
            cv::line(room_hough, p1, p2, cv::Scalar(128), 2);
        }

        if (DISPLAY_ROOM_ROTATOR) {
            cv::imshow("room_hough", room_hough);
            cv::waitKey();
        }

        if (lines.size() >= 4)
            break;
    }

    Histogram<double> direction_histogram(0, CV_PI, 36);
    for (size_t i = 0; i < lines.size(); ++i) {
        double dx = lines[i][2] - lines[i][0];
        double dy = lines[i][3] - lines[i][1];
        if (dy * dy + dx * dx > 0.0) {
            double current_direction = std::atan2(dy, dx);
            while (current_direction < 0.)
                current_direction += CV_PI;
            while (current_direction > CV_PI)
                current_direction -= CV_PI;
            direction_histogram.addData(current_direction, sqrt(dy * dy + dx * dx));
        }
    }
    return direction_histogram.getMaxBinPreciseVal();
}

void RoomRotator::transformPathBackToOriginalRotation(const std::vector<cv::Point2f> &fov_middlepoint_path,
                                                      std::vector<geometry_msgs::Pose2D> &path_fov_poses,
                                                      const cv::Mat &R) {
    path_fov_poses.clear();

    cv::Mat R_inv;
    cv::invertAffineTransform(R, R_inv);
    std::vector<cv::Point2f> fov_middlepoint_path_transformed;
    cv::transform(fov_middlepoint_path, fov_middlepoint_path_transformed, R_inv);

    transformPointPathToPosePath(fov_middlepoint_path_transformed, path_fov_poses);
}

void RoomRotator::transformPointPathToPosePath(const std::vector<cv::Point2f> &point_path,
                                               std::vector<geometry_msgs::Pose2D> &pose_path) {
    if (point_path.empty()) {
        return;
    }

    if (point_path.size() == 1) {
        geometry_msgs::Pose2D zero_pose;
        zero_pose.x = point_path[0].x;
        zero_pose.y = point_path[0].y;
        zero_pose.theta = 0.;
        pose_path.push_back(zero_pose);
    } else {

        geometry_msgs::Pose2D zero_pose;
        zero_pose.x = point_path[0].x;
        zero_pose.y = point_path[0].y;
        zero_pose.theta = 0.;
        pose_path.push_back(zero_pose);

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

void RoomRotator::transformPointPathToPosePath(std::vector<geometry_msgs::Pose2D> &pose_path) {
    std::vector<cv::Point2f> point_path;
    for (size_t i = 0; i < pose_path.size(); ++i)
        point_path.push_back(cv::Point2f(pose_path[i].x, pose_path[i].y));

    pose_path.clear();
    transformPointPathToPosePath(point_path, pose_path);
}

void RoomRotator::getMinMaxCoordinates(const cv::Mat &map, cv::Point &min_room, cv::Point &max_room) {
    min_room.x = std::numeric_limits<int>::max();
    min_room.y = std::numeric_limits<int>::max();
    max_room.x = 0;
    max_room.y = 0;
    for (int v = 0; v < map.rows; ++v) {
        for (int u = 0; u < map.cols; ++u) {
            if (map.at<uchar>(v, u) == 255) {
                min_room.x = std::min(min_room.x, u);
                min_room.y = std::min(min_room.y, v);
                max_room.x = std::max(max_room.x, u);
                max_room.y = std::max(max_room.y, v);
            }
        }
    }
}
