//
// Created by Looper on 2022/9/30.
//

#ifndef APP_COMMUNICATION_EXPLORATIONCENTER_H
#define APP_COMMUNICATION_EXPLORATIONCENTER_H

#include <ros/ros.h>
#include "odom_subscribe.h"
#include "segmentation/map_attribute.h"
#include "lru_cache.h"
#include "model/RoomVo.h"
#include "segmentation/Room.h"
#include "exploration_generate.h"
#include "future/thread_pool.h"

#define  EXPLORATION_THREAD_POOL_MAX_NUM 1

const int BOUSTROPHEDON_EXPLORER_MODE = 1;

enum ExplorationModel {
    FULL,
    SUB,
    RECT
};

class ExplorationCenter {
private:
    bool initialize_finish = false;
    std::recursive_mutex cv_mut;

    OdomSubscribe *poseSubscribe;

    async::ThreadPool pool_;

    cache::lru_cache<std::string, RoomCoverage> coverageCache = cache::lru_cache<std::string, RoomCoverage>(3);

    void generatePlanningPath(const cv::Mat &room_map, ExplorationModel model, int explorer_mode,
                              bool ordain_start, const cv::Point &start_position,
                              std::vector<geometry_msgs::Pose2D> &exploration_path,
                              std::vector<cv::Point> &point_path);

    void optimizePlanningPath(const cv::Mat &room_map,
                              std::vector<geometry_msgs::Pose2D> &exploration_path,
                              std::vector<cv::Point> &point_path,
                              bool distance = true);

    bool baseStationAvailable(cv::Mat &room_map, const cv::Point &point);

    cv::Mat findClosestPointRoom(cv::Mat &room_map, const cv::Point &point, double min_cell_area);

    bool removeUnconnectedRoomParts(cv::Mat &room_map);

    cv::Mat prohibitionMat(const cv::Mat &room_map) const;

    void morphologicalEdging(cv::Mat &room_map, int map_correction_closing_neighborhood_size) const;

    void pose2CVPoint(const cv::Mat &room_map, std::vector<cv::Point> &pointList,
                      const std::vector<geometry_msgs::Pose2D> &postList,
                      const cv::Point2d &map_origin);

    void cvPoint2Pose(const cv::Mat &room_map, std::vector<geometry_msgs::Pose2D> &postList,
                      const std::vector<cv::Point> &pointList, const cv::Point2d &map_origin);

    cv::Mat loadGenerateMap(int grid_spacing_in_pixel);

    bool detectionTooSmallRoom(const cv::Mat &map, int iterations) const;

    cv::Point &findBaseNearReachable(cv::Mat &map, cv::Point &reachablePoint, int range);

public:
    static auto &instance() {
        static ExplorationCenter obj;
        return obj;
    }

    ros::Publisher path_pub_;

    CoveragePathGenerator coveragePathGenerator;
    SubregionPathGenerator subregionPathGenerator;

    void initialize(ros::NodeHandle handle);

    void uninstall();

    void repaintCoveragePath(bool isMapChange);

    void repaintSubregionPath();

    RoomCoverage obtainCoveragePath();

    RoomCoverage obtainSubregionPath();

    void infinitelyNearBoundary(const cv::Mat &room_map, std::vector<geometry_msgs::Pose2D> &pose_path,
                                std::vector<cv::Point> &point_path);

    void generatePlanningPathRect(const cv::Mat &room_map, int explorer_mode,
                                  std::vector<geometry_msgs::Pose2D> &exploration_path,
                                  std::vector<cv::Point> &point_path);

    void generatePlanningPathSub(const cv::Mat &room_map, int explorer_mode,
                                 std::vector<geometry_msgs::Pose2D> &exploration_path,
                                 std::vector<cv::Point> &point_path);

    void generatePlanningPathFull(const cv::Mat &room_map, int explorer_mode,
                                  std::vector<geometry_msgs::Pose2D> &exploration_path,
                                  std::vector<cv::Point> &point_path);

    void generatePlanningSegmentationPath(const cv::Mat &room_map, cv::Mat segmented_map, std::vector<Room> rooms,
                                          int explorer_mode,
                                          std::vector<geometry_msgs::Pose2D> &exploration_path,
                                          std::vector<cv::Point> &point_path);

    void pathPublish(const std::vector<geometry_msgs::Pose2D> &exploration_path) const;

    void cacheRoomCoverage(const RoomCoverage &coverage);

    RoomCoverage findRoomCoverage(const std::string &coverageId, bool latest);
};

#endif //APP_COMMUNICATION_EXPLORATIONCENTER_H
