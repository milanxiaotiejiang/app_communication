//
// Created by Looper on 2023/2/2.
//

#ifndef APP_COMMUNICATION_INFINITELY_NEAR_BOUNDARY_H
#define APP_COMMUNICATION_INFINITELY_NEAR_BOUNDARY_H

#include <opencv2/core/mat.hpp>
#include <geometry_msgs/Pose2D.h>

class InfinitelyNearBoundary {
private:
    void transformPointPathToPosePath(const std::vector<cv::Point2f> &point_path,
                                      std::vector<geometry_msgs::Pose2D> &pose_path);

    void optimizePathColumn(std::vector<geometry_msgs::Pose2D> &vector);

public:
    void getExplorationPath(const cv::Mat &room_map, std::vector<geometry_msgs::Pose2D> &pose_path,
                            std::vector<cv::Point> &point_path,
                            const float map_resolution, const cv::Point2d map_origin,
                            int number_extension, int distance_from_obstacles, int multiple_contour_spacing);
};


#endif //APP_COMMUNICATION_INFINITELY_NEAR_BOUNDARY_H
