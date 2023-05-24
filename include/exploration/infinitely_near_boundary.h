//
// Created by Looper on 2023/2/2.
//

#ifndef APP_COMMUNICATION_INFINITELY_NEAR_BOUNDARY_H
#define APP_COMMUNICATION_INFINITELY_NEAR_BOUNDARY_H

#include <opencv2/core/mat.hpp>
#include <geometry_msgs/Pose2D.h>
#include "A_star_pathplanner.h"
#include "exploration/douglas/Point2D.h"

class InfinitelyNearBoundary {
private:
    AStarPlanner path_planner;

    std::vector<Point2D> splitPoints(const Point2D &p1, const Point2D &p2, double distance);

    std::vector<Point2D> splitPointsIfNeeded(const std::vector<Point2D> &points, double distance);

    std::vector<geometry_msgs::Pose2D> transformPointPathToPosePath(const std::vector<Point2D> &point_path);

public:
    void getExplorationPath(const cv::Mat &original_map,
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
                            const double path_eps);
};


#endif //APP_COMMUNICATION_INFINITELY_NEAR_BOUNDARY_H
