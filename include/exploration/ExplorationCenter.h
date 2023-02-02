//
// Created by Looper on 2022/9/30.
//

#ifndef APP_COMMUNICATION_EXPLORATIONCENTER_H
#define APP_COMMUNICATION_EXPLORATIONCENTER_H

#include <ros/ros.h>
#include "exploration/pose/tracked_pose_subscribe.h"
#include "exploration/pose/amcl_pose_subscribe.h"
#include "segmentation/map_attribute.h"
#include "lru_cache.h"
#include "model/RoomVo.h"
#include "segmentation/Room.h"

enum ExplorationModel {
    FULL,
    SUB,
    RECT
};

class ExplorationCenter {
private:
    bool initialize_finish = false;

    AmclPoseSubscribe *poseSubscribe;
    TrackedSubscribe *trackedSubscribe;

    cache::lru_cache<std::string, RoomCoverage> coverageCache = cache::lru_cache<std::string, RoomCoverage>(3);

    bool baseStationAvailable(cv::Mat &room_map, const cv::Point &point);

    cv::Mat findClosestPointRoom(cv::Mat &room_map, const cv::Point &point);

    bool removeUnconnectedRoomParts(cv::Mat &room_map);

    cv::Mat prohibitionMat(const cv::Mat &room_map) const;

    void pose2CVPoint(const cv::Mat &room_map, std::vector<cv::Point> &pointList,
                      const std::vector<geometry_msgs::Pose2D> &postList,
                      const cv::Point2d &map_origin);

    cv::Mat loadGenerateMap(int grid_spacing_in_pixel);

    void optimizePathColumn(std::vector<geometry_msgs::Pose2D> &vector);

    bool detectionTooSmallRoom(const cv::Mat &map, int iterations) const;


public:
    static auto &instance() {
        static ExplorationCenter obj;
        return obj;
    }

    ros::Publisher path_pub_;

    void initialize(ros::NodeHandle handle);

    void uninstall();

    void infinitelyNearBoundary(const cv::Mat &room_map);

    void generatePlanningPath(const cv::Mat &room_map, ExplorationModel model,
                              std::vector<geometry_msgs::Pose2D> &exploration_path,
                              std::vector<cv::Point> &point_path);

    void generatePlanningPath(const cv::Mat &room_map, ExplorationModel model, int explorer_mode,
                              bool ordain_start, const cv::Point &start_position,
                              std::vector<geometry_msgs::Pose2D> &exploration_path,
                              std::vector<cv::Point> &point_path);

    void generatePlanningSegmentationPath(const cv::Mat &room_map, cv::Mat segmented_map, std::vector<Room> rooms,
                                          int explorer_mode,
                                          std::vector<geometry_msgs::Pose2D> &exploration_path,
                                          std::vector<cv::Point> &point_path);

    void pathPublish(const std::vector<geometry_msgs::Pose2D> &exploration_path) const;

    void cacheRoomCoverage(const RoomCoverage &coverage);

    RoomCoverage findRoomCoverage(const std::string &coverageId);
};

#endif //APP_COMMUNICATION_EXPLORATIONCENTER_H
