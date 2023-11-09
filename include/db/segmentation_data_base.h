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
        static SegmentationDataBase obj;
        return obj;
    }

    static GateInfo gate2Info(const Gate &gate);

    static Gate info2Gate(const GateInfo &gateInfo);

    void sync_schema();

    bool loadMainMap();

    MapPo &getDbMap();

    MapPo installMap(std::string name);

    void changeMap(const std::string &map_id);

    MapPo installDefaultMap();

    std::vector<MapPo> loadAllMap();

    void updateMapName(const std::string &map_id, const std::string &map_name);

    void removeMap(const std::string &map_id);

    RoomPo selectRoomById(long roomId);

    std::vector<RoomPo> selectRoomByMapId(const std::string &mapId);

    void removeAllRoom(const std::string &mapId);

    void removeAllRoom();

    void memory2Storage(cv::Mat &mat, std::vector<Room> &rooms);

    void storage2Memory(cv::Mat &mat, std::vector<Room> &rooms, double map_resolution_from_subscription);

    void reRoomName(int targetId, const std::string &name);

    PlanPo loadDefaultPlanParam(const std::string &mapId);

    PlanPo setPlanParam(const std::string &mapId,
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

    PlanPo getDbPlan(const std::string &mapId);

    void saveGate(const Gate &gate);

    std::vector<Gate> loadGate(const std::string &mapId);

    void purgeGate(const std::string &mapId);

    int addGateInfo(const GateInfo &gateInfo);

    void deleteGateForId(long id);

    long modifyGateInfo(const GateInfo &gateInfo);

    std::vector<GateInfo> loadGateInfo(const std::string &mapId);

    GateInfo queryGateForId(long id);
};


#endif //APP_COMMUNICATION_SEGMENTATION_DATA_BASE_H
