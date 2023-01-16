//
// Created by Looper on 2022/9/15.
//

#ifndef APP_COMMUNICATION_SEGMENTATIONCENTER_H
#define APP_COMMUNICATION_SEGMENTATIONCENTER_H

#include "Room.h"
#include "model/RoomVo.h"
#include "CvUtils.h"
#include <opencv2/opencv.hpp>

const double room_area_factor_lower_limit = 0.1;
const double room_area_factor_upper_limit = 1000000;
const int neighborhood_index = 280;
const int max_iterations = 150;
const double min_critical_point_distance_factor = 0.5;
const double max_area_for_merging = 12.5;

class SegmentationCenter {
private:
    bool initialize_finish = false;

    bool detectionTooSmallRoom(const cv::Mat &segmented_map, Room room) const;

    bool pointInRoom(const cv::Mat &segmented_map, Room room, cv::Point point) const;

    bool lineThroughRoom(const cv::Mat &segmented_map, Room room, const cv::Point &ps, const cv::Point &pe) const;

public:
    static auto &instance() {
        static SegmentationCenter obj;
        return obj;
    }

    /**
     * 初始化
     */
    void initialize();

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


    void reRoomName(int targetId, std::string name);

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
    cv::Mat choiceOneRoom(cv::Mat &segmented_map, std::vector<Room> &rooms, int targetId);

    cv::Mat generateMat() const;

    bool checkPartition() const;

    MapRoomVo toVoRoom(cv::Mat &segmented_map, std::vector<Room> &rooms) const;
};


#endif //APP_COMMUNICATION_SEGMENTATIONCENTER_H
