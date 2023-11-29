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
#include <ostream>
#include "yaml-cpp/yaml.h"

#include "simulation.h"
#include "model/Point.h"
#include "db/path.h"
#include "task/RealPoint.h"

const double map_resolution_from_subscription = 0.05;

const int VIRTUAL_WALL_DUS_COUNT = 2;
const int PENALTY_ZONE_DUS_COUNT = 4;

struct MapAttribute {
    std::string attrPath;
    geometry_msgs::Pose originPose;
    std::string mapPath;
    int mapCols;//width
    int mapRows;//height
    cv::Point2d originPoint;

    MapAttribute() = default;

    MapAttribute(const std::string &attrPath) : attrPath(attrPath) {}

    friend std::ostream &operator<<(std::ostream &os, const MapAttribute &attribute) {
        os << "attrPath: " << attribute.attrPath << " originPoseX: " << attribute.originPose.position.x
           << " originPoseY: " << attribute.originPose.position.y << " mapPath: "
           << attribute.mapPath << " mapCols: " << attribute.mapCols << " mapRows: " << attribute.mapRows
           << " originPoint: " << attribute.originPoint;
        return os;
    }
};

class MapAttributeSingleton {
private:
    MapAttributeSingleton() = default;

    MapAttributeSingleton(MapAttributeSingleton &) = delete;

    MapAttributeSingleton &operator=(const MapAttributeSingleton &) = delete;

public:
    ~MapAttributeSingleton() = default;

private:
    std::atomic<bool> creating_map{false};
    std::condition_variable wait_cv;
    std::mutex wait_mutex;

    MapAttribute currentMapAttribute;

    geometry_msgs::Pose2D starting_position_pose;

    std::vector<std::vector<Point>> virtualWallList;
    std::vector<std::vector<Point>> penaltyZoneList;

public:
    static auto &instance() {
        static MapAttributeSingleton obj;
        return obj;
    }

    bool isCreatingMap() const;

    const geometry_msgs::Pose &getMapOriginPose() const {
        return currentMapAttribute.originPose;
    }

    cv::Point2d getMapOrigin() const {
        return {currentMapAttribute.originPose.position.x, currentMapAttribute.originPose.position.y};
    }

    const std::vector<std::vector<Point>> &getVirtualWallList() const {
        return virtualWallList;
    }

    const MapAttribute &getCurrentMapAttribute() const {
        return currentMapAttribute;
    }

    const std::vector<std::vector<Point>> &getPenaltyZoneList() const {
        return penaltyZoneList;
    }

    const geometry_msgs::Pose2D &getRobotPositionPose() const {
        return starting_position_pose;
    }

    void setRobotPositionPose(geometry_msgs::Pose2D positionPose);

    cv::Point getRobotPositionPoint(const cv::Mat &room_map) const;

    cv::Point getRobotPositionPoint(int rows, int cols) const;

    bool loadStation();

    void resetProhibition();

    void loadVirtualWall();

    void loadPenaltyZone();

    void loadPlanParam();

    void loadDefaultPlanParam();

    void handleProhibition(std::vector<std::vector<Point>> &list,
                           const YAML::Node &node, int dusCount) const;

    /**
     * ros坐标转地图坐标
     */
    cv::Point rosPoint2MapPoint(const cv::Mat &room_map, const Point &point) const;

    /**
     * ros坐标转地图坐标
     */
    cv::Point rosPoint2MapPoint(int rows, int cols, const Point &point) const;

    /**
     * ros坐标转地图坐标
     * 地图原点需要传入，可用于非当前地图
     */
    cv::Point rosPoint2MapPoint(const cv::Point2d &map_origin, int rows, int cols, const Point &point) const;

    /**
     * 地图坐标转ros坐标
     */
    Point mapPoint2RosPoint(int rows, int cols, const cv::Point &point) const;

    /**
     * 地图坐标转ros坐标
     * 地图原点需要传入，可用于非当前地图
     */
    Point mapPoint2RosPoint(const cv::Point2d &map_origin, int rows, int cols, const cv::Point &point) const;

    bool saveMap();

    void notifySaveMap();

    static bool readAnyMapInfo(MapAttribute &mapAttribute);

    static RealPoint createCurrentPoint();
};


#endif //APP_COMMUNICATION_MAP_ATTRIBUTE_H
