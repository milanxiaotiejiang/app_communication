//
// Created by Looper on 2022/10/13.
//

#ifndef APP_COMMUNICATION_MAP_ATTRIBUTE_H
#define APP_COMMUNICATION_MAP_ATTRIBUTE_H

#include "ros/ros.h"
#include "ros/package.h"
#include "string"
#include <geometry_msgs/Pose2D.h>
#include <geometry_msgs/PoseStamped.h>
#include <opencv2/opencv.hpp>
#include <condition_variable>
#include "yaml-cpp/yaml.h"

#include "simulation.h"
#include "model/Point.h"
#include "db/path.h"

const double map_resolution_from_subscription = 0.05;

const int VIRTUAL_WALL_DUS_COUNT = 2;
const int PENALTY_ZONE_DUS_COUNT = 4;

class MapAttribute {
private:
    bool initialize_finish = false;

    std::atomic<bool> creating_map{false};
    std::condition_variable wait_cv;
    std::mutex wait_mutex;

    geometry_msgs::Pose map_origin_pose;
    cv::Point2d map_origin;

    geometry_msgs::Pose2D starting_position_pose;

    std::vector<std::vector<Point>> virtualWallList;
    std::vector<std::vector<Point>> penaltyZoneList;

    const double robot_radius_ = 0.26;
    const int map_correction_closing_neighborhood_size_ = 2;
    const double grid_obstacle_offset_ = 0.2;
    const double path_eps_ = 4.0;
    const double min_cell_area_ = 60.0;
    const int max_deviation_from_track_ = -1;
    const int range_near_base_station_ = 5;

    const double room_area_factor_lower_limit_ = 0.1;
    const double room_area_factor_upper_limit_ = 1000000;
    const int neighborhood_index_ = 280;
    const int max_iterations_ = 150;
    const double min_critical_point_distance_factor_ = 0.5;
    const double max_area_for_merging_ = 12.5;

    const int distance_from_obstacles_ = 5;
    const int number_extension_ = 1;
    const int multiple_contour_spacing_ = 0;
    const int random_number_generation_ratio_ = 100;
    const int boundary_min_area_ = 1;

public:
    static auto &instance() {
        static MapAttribute obj;
        return obj;
    }

    bool isInitializeFinish() const {
        return initialize_finish;
    }

    bool isCreatingMap() const;

    const geometry_msgs::Pose &getMapOriginPose() const {
        return map_origin_pose;
    }

    const cv::Point2d &getMapOrigin() const {
        return map_origin;
    }

    const std::vector<std::vector<Point>> &getVirtualWallList() const {
        return virtualWallList;
    }

    const std::vector<std::vector<Point>> &getPenaltyZoneList() const {
        return penaltyZoneList;
    }

    const geometry_msgs::Pose2D &getRobotPositionPose() const {
        return starting_position_pose;
    }

    void setRobotPositionPose(geometry_msgs::Pose2D positionPose);

    cv::Point getRobotPositionPoint(const cv::Mat &room_map) const;

    void loadStation();

    void resetProhibition();

    void loadVirtualWall();

    void loadPenaltyZone();

    void loadPlanParam();

    void loadDefaultPlanParam();

    void handleProhibition(std::vector<std::vector<Point>> &list,
                           const YAML::Node &node, int dusCount) const;

    cv::Point rosPoint2MapPoint(const cv::Mat &room_map, const Point &point) const;

    cv::Point rosPoint2MapPoint(double rows, double cols, const Point &point) const;

    bool saveMap();

    void notifySaveMap();
};


#endif //APP_COMMUNICATION_MAP_ATTRIBUTE_H
