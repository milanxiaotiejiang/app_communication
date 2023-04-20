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
    MapStorage segmentationStorage = SqliteDataBase::initMapStorage();
public:
    static auto &instance() {
        static SegmentationDataBase obj;
        return obj;
    }

    void sync_schema();

    bool loadMainMap();

    MapPo &getDbMap();

    MapPo installMap(std::string name);

    MapPo installDefaultMap();

    std::vector<MapPo> loadAllMap();

    void updateMapName(const std::string &map_name);

    RoomPo selectRoomById(long roomId);

    std::vector<RoomPo> selectRoomByMapId(const std::string &mapId);

    void removeAllRoom(const std::string &mapId);

    void memory2Storage(cv::Mat &mat, std::vector<Room> &rooms);

    void storage2Memory(cv::Mat &mat, std::vector<Room> &rooms, double map_resolution_from_subscription);

    void reRoomName(int targetId, const std::string &name);

    void setPlanParam(const std::string &mapId,
                      double robotRadius,
                      int mapCorrectionClosingNeighborhoodSize,
                      double gridObstacleOffset,
                      double pathEps,
                      double minCellArea,
                      double maxDeviationFromTrack,
                      int rangeNearBaseStation,
                      double roomAreaFactorLowerLimit,
                      double roomAreaFactorUpperLimit,
                      int neighborhoodIndex,
                      int maxIterations,
                      double minCriticalPointDistanceFactor,
                      double maxAreaForMerging,
                      int distanceFromObstacles,
                      int numberExtension,
                      int multipleContourSpacing,
                      int random_number_generation_ratio,
                      int boundary_min_area
    );

    PlanPo getDbPlan(std::string map_id);
};


#endif //APP_COMMUNICATION_SEGMENTATION_DATA_BASE_H
