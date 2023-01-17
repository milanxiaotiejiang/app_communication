//
// Created by Looper on 2022/9/16.
//

#ifndef APP_COMMUNICATION_SEGMENTATION_DATA_BASE_H
#define APP_COMMUNICATION_SEGMENTATION_DATA_BASE_H

#include "segmentation/CvUtils.h"
#include "db/segmentation_model.h"
#include "db/SqliteDataBase.h"

class SegmentationDataBase {
private:
    MapPo mapPo;
    Storage segmentationStorage = SqliteDataBase::initStorage();
public:
    static auto &instance() {
        static SegmentationDataBase obj;
        return obj;
    }

    bool loadMap();

    MapPo &getDbMap();

    std::vector<RoomPo> selectByMapId(const std::string &mapId);

    void removeAllRoom(const std::string &mapId);

    void memory2Storage(cv::Mat &mat, std::vector<Room> &rooms);

    void storage2Memory(cv::Mat &mat, std::vector<Room> &rooms, double map_resolution_from_subscription);

    void reRoomName(int targetId, const std::string &name);
};


#endif //APP_COMMUNICATION_SEGMENTATION_DATA_BASE_H
