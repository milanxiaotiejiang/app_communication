//
// Created by Looper on 2022/9/16.
//

#ifndef APP_COMMUNICATION_SEGMENTATION_DATA_BASE_H
#define APP_COMMUNICATION_SEGMENTATION_DATA_BASE_H

#include "segmentation/CvUtils.h"
#include "db/segmentation_model.h"
#include "db/SqliteDataBase.h"
#include "model/gate_info.h"

class SegmentationDataBase {
private:
    SegmentationDataBase() = default;

    SegmentationDataBase(SegmentationDataBase &) = delete;

    SegmentationDataBase &operator=(const SegmentationDataBase &) = delete;

public:
    ~SegmentationDataBase() = default;

private:
    MapPo mapPo;
    MapStorage segmentationStorage = SqliteDataBase::initMapStorage();

public:
    static auto &instance() {
        static SegmentationDataBase obj;
        return obj;
    }

    static GateInfo gate2Info(const Gate &gate);

    static Gate info2Gate(const GateInfo &gateInfo);

    void sync_schema();

    bool loadMainMap();

    MapPo &getDbMap();

    MapPo installMap(std::string name);

    MapPo installDefaultMap();

    std::vector<MapPo> loadAllMap();

    void updateMapName(const std::string &map_id, const std::string &map_name);

    RoomPo selectRoomById(long roomId);

    std::vector<RoomPo> selectRoomByMapId(const std::string &mapId);

    void removeAllRoom(const std::string &mapId);

    void removeAllRoom();

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
                      int boundary_min_area,
                      int version = CURRENT_PLAN_VERSION
    );

    void removePlanParam(const std::string &mapId);

    PlanPo getDbPlan(std::string map_id);

    void saveGate(const Gate &gate);

    std::vector<Gate> loadGate(const std::string &mapId);

    void purgeGate(const std::string &mapId);

    long addGateInfo(const GateInfo &gateInfo);

    void deleteGateForId(long id);

    long modifyGateInfo(const GateInfo &gateInfo);

    std::vector<GateInfo> loadGateInfo(const std::string &mapId);

    GateInfo queryGateForId(long id);
};


#endif //APP_COMMUNICATION_SEGMENTATION_DATA_BASE_H
