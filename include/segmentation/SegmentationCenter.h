//
// Created by Looper on 2022/9/15.
//

#ifndef APP_COMMUNICATION_SEGMENTATIONCENTER_H
#define APP_COMMUNICATION_SEGMENTATIONCENTER_H

#include "Room.h"
#include "model/RoomVo.h"
#include "CvUtils.h"
#include "db/segmentation_model.h"
#include "segmentation_subscribe.h"
#include "exploration/A_star_pathplanner.h"
#include "model/gate_info.h"
#include <opencv2/opencv.hpp>
#include <ros/node_handle.h>
#include <mutex>

class SegmentationCenter {
private:
    SegmentationCenter() = default;

    SegmentationCenter(SegmentationCenter &) = delete;

    SegmentationCenter &operator=(const SegmentationCenter &) = delete;

public:
    ~SegmentationCenter() = default;

private:
    bool initialize_finish = false;
    std::recursive_mutex cv_mut;

    SegmentationSubscribe *segmentationSubscribe;

    bool detectionTooSmallRoom(const cv::Mat &segmented_map, Room room, const PlanPo &plan) const;

    bool pointInRoom(const cv::Mat &segmented_map, Room room, const cv::Point &point) const;

    bool lineThroughRoom(const cv::Mat &segmented_map, Room room, const cv::Point &ps, const cv::Point &pe) const;

    /**
     * 检查闸机线
     */
    Room checkGateWire(cv::Mat &segmented_map, const Gate &gate);

    /**
     * 检查闸机摆渡点位
     */
    void checkGatePoint(cv::Mat &segmented_map, std::vector<Room> &rooms, const Gate &gate);

public:
    static auto &instance() {
        static SegmentationCenter obj;
        return obj;
    }

    /**
     * 初始化
     */
    bool initialize(const ros::NodeHandle &handle);

    /**
     * 重置所有分区的设置
     */
    void resetSegmentation();

    /**
     * 将原始地图进行首次分割
     */
    void originalSegmentation(cv::Mat &segmented_map, std::vector<Room> &rooms,
                              const cv::Point &ps, const cv::Point &pe);

    /**
     * 手动分割
     */
    void handSegmentation(cv::Mat &segmented_map, std::vector<Room> &rooms, int target_index,
                          const cv::Point &ps, const cv::Point &pe);

    /**
     * 合并房间
     */
    void mergeRoom(cv::Mat &segmented_map, std::vector<Room> &rooms, int target_index,
                   int room_to_merge_index);


    void reRoomName(int targetId, const std::string &name);

    /**
     * 落盘
     */
    void memory2Storage(cv::Mat &segmented_map, std::vector<Room> &rooms);

    /**
     * 读取
     */
    void storage2Memory(cv::Mat &segmented_map, std::vector<Room> &rooms);

    /**
     * 自动分区
     */
    void automaticSegmentation(cv::Mat &segmented_map, std::vector<Room> &rooms);

    /**
     * 选择某个房间
     * @return 返回仅包含此房间的地图
     */
    cv::Mat choiceOneRoom(cv::Mat &segmented_map, std::vector<Room> &rooms, long targetId);

    cv::Mat generateMat();

    bool checkPartition() const;

    MapRoomVo resultMapRoomVo() const;

    void isRestrictedZone(const cv::Mat &room_map, bool &isOffMap, bool &isRestrictedZone,
                          bool &isMaxPassable, bool &isPlanPath, bool debug);

    bool
    pointInArea(const cv::Mat &area_map, const cv::Point &stationPoint, const cv::Point &point, bool largest) const;

    /**
     * 将原始地图进行首次分割，用于验证
     */
    void gateSegmentation(cv::Mat &segmented_map, std::vector<Room> &rooms, const Gate &gate);

    /**
     * 闸机分割
     */
    void gateManySegmentation(cv::Mat &segmented_map, std::vector<Room> &rooms,
                              std::map<std::pair<int, int>, std::pair<Gate, bool>> &planMap, const Gate &gate);

    /**
     * 获取原来的闸机摆渡点在现有分区地图上的值
     */
    int obtainOriginalGatePointValue(cv::Mat &segmented_map, const Gate originalGate,
                                     double regionDirection, double pointDirection);

    /**
     * 闸机开启
     */
    void gateManyOpen(cv::Mat &open_map, const Gate &gate);
};


#endif //APP_COMMUNICATION_SEGMENTATIONCENTER_H
