//
// Created by Looper on 2023/2/2.
//

#ifndef APP_COMMUNICATION_INFINITELY_NEAR_BOUNDARY_H
#define APP_COMMUNICATION_INFINITELY_NEAR_BOUNDARY_H

#include <opencv2/core/mat.hpp>
#include <geometry_msgs/Pose2D.h>
#include "A_star_pathplanner.h"

class InfinitelyNearBoundary {
private:
    AStarPlanner path_planner;

    void transformPointPathToPosePath(const std::vector<cv::Point2f> &point_path,
                                      std::vector<geometry_msgs::Pose2D> &pose_path);

public:
    void getExplorationPath(const cv::Mat &original_map,
                            const cv::Mat &room_map,
                            std::vector<geometry_msgs::Pose2D> &pose_path,
                            std::vector<cv::Point> &point_path,
                            std::vector<std::vector<geometry_msgs::Pose2D>> &complex_pose_path,
                            float map_resolution,
                            const cv::Point &starting_position,
                            const cv::Point2d &map_origin,
                            double robot_radius,
                            int number_extension,
                            int distance_from_obstacles,
                            int multiple_contour_spacing,
                            int random_number_generation_ratio,
                            int boundary_min_area);
};


#endif //APP_COMMUNICATION_INFINITELY_NEAR_BOUNDARY_H
