//
// Created by Looper on 2022/9/30.
//

#ifndef APP_COMMUNICATION_ROOM_ROTATOR_H
#define APP_COMMUNICATION_ROOM_ROTATOR_H

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <vector>

#include <geometry_msgs/Pose2D.h>

class RoomRotator {

public:
    RoomRotator() {
    }

    void
    rotateRoom(const cv::Mat &room_map, cv::Mat &rotated_room_map, const cv::Mat &R, const cv::Rect &bounding_rect);

    double
    computeRoomRotationMatrix(const cv::Mat &room_map, cv::Mat &R, cv::Rect &bounding_rect, const double map_resolution,
                              const cv::Point *center = 0, const double rotation_offset = 0.);

    double computeRoomMainDirection(const cv::Mat &room_map, const double map_resolution);

    void transformPathBackToOriginalRotation(const std::vector<cv::Point2f> &fov_middlepoint_path,
                                             std::vector<geometry_msgs::Pose2D> &path_fov_poses, const cv::Mat &R);

    void transformPointPathToPosePath(const std::vector<cv::Point2f> &point_path,
                                      std::vector<geometry_msgs::Pose2D> &pose_path);

    void transformPointPathToPosePath(std::vector<geometry_msgs::Pose2D> &pose_path);

    void getMinMaxCoordinates(const cv::Mat &map, cv::Point &min_room, cv::Point &max_room);
};


#endif //APP_COMMUNICATION_ROOM_ROTATOR_H
