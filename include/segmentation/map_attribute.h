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
#include "yaml-cpp/yaml.h"

#include "glog/logging.h"
#include "model/Point.h"

const double robot_radius = 0.30;//0.45;
const double map_resolution_from_subscription = 0.05;

const int VIRTUAL_WALL_DUS_COUNT = 2;
const int PENALTY_ZONE_DUS_COUNT = 4;

const std::string map_yaml_path = ros::package::getPath("robot_slam")
                                  + "/maps/mymap.yaml";
const std::string prohibition_yaml_path = ros::package::getPath("data_base")
                                          + "/config/prohibition_areas.yaml";

class MapAttribute {
private:
    bool initialize_finish = false;

    geometry_msgs::Pose map_origin_pose;
    cv::Point2d map_origin;

    geometry_msgs::Pose2D starting_position_pose;

    std::vector<std::vector<Point>> virtualWallList;
    std::vector<std::vector<Point>> penaltyZoneList;
public:
    static auto &instance() {
        static MapAttribute obj;
        return obj;
    }

    bool isInitializeFinish() const {
        return initialize_finish;
    }

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

    void handleProhibition(std::vector<std::vector<Point>> &list,
                           const YAML::Node &node, int dusCount) const;

    cv::Point rosPoint2MapPoint(const cv::Mat &room_map, const Point &point) const;
};


#endif //APP_COMMUNICATION_MAP_ATTRIBUTE_H
