//
// Created by Looper on 2022/9/16.
//

#ifndef APP_COMMUNICATION_ROOMVO_H
#define APP_COMMUNICATION_ROOMVO_H

#include <opencv2/core/types.hpp>
#include "string"
#include "vector"

#include "nlohmann/json.hpp"

using json = nlohmann::json;

class PointVo {
private:
    int x;
    int y;
public:
    PointVo();

    PointVo(int x, int y);

    int getX() const;

    void setX(int x);

    int getY() const;

    void setY(int y);

    friend void to_json(json &j, const PointVo &pointVo) {
        j = json{
                {"x", pointVo.x},
                {"y", pointVo.y}
        };
    }

    friend void from_json(const json &j, PointVo &pointVo) {
        j.at("x").get_to(pointVo.x);
        j.at("y").get_to(pointVo.y);
    }
};

class PoseVo {
private:
    float x;
    float y;
    float theta;
public:
    PoseVo();

    PoseVo(float x, float y, float theta);

    float getX() const;

    void setX(float x);

    float getY() const;

    void setY(float y);

    float getTheta() const;

    void setTheta(float theta);

    friend void to_json(json &j, const PoseVo &poseVo) {
        j = json{
                {"x",     poseVo.x},
                {"y",     poseVo.y},
                {"theta", poseVo.theta}
        };
    }

    friend void from_json(const json &j, PoseVo &poseVo) {
        j.at("x").get_to(poseVo.x);
        j.at("y").get_to(poseVo.y);
        j.at("theta").get_to(poseVo.theta);
    }
};

class RoomVo {
private:
    int id;
    std::string name;
    PointVo center;
    std::vector<PointVo> memberPoints;
    std::vector<int> neighborRoomIds;
    double area;
    double perimeter;
public:
    RoomVo();

    RoomVo(int id, std::string name, PointVo center,
           const std::vector<PointVo> &memberPoints,
           const std::vector<int> &neighborRoomIds,
           double area,
           double perimeter);

    int getId() const;

    void setId(int id);

    const std::vector<PointVo> &getMemberPoints() const;

    void setMemberPoints(const std::vector<PointVo> &memberPoints);

    const std::vector<int> &getNeighborRoomIds() const;

    void setNeighborRoomIds(const std::vector<int> &neighborRoomIds);

    double getArea() const;

    void setArea(double area);

    double getPerimeter() const;

    void setPerimeter(double perimeter);

    friend void to_json(json &j, const RoomVo &roomVo) {
        j = json{
                {"id",              roomVo.id},
                {"name",            roomVo.name},
                {"center",          roomVo.center},
                {"memberPoints",    roomVo.memberPoints},
                {"neighborRoomIds", roomVo.neighborRoomIds},
                {"area",            roomVo.area},
                {"perimeter",       roomVo.perimeter}
        };
    }

    friend void from_json(const json &j, RoomVo &roomVo) {
        j.at("id").get_to(roomVo.id);
        j.at("name").get_to(roomVo.name);
        j.at("center").get_to(roomVo.center);
        j.at("memberPoints").get_to(roomVo.memberPoints);
        j.at("neighborRoomIds").get_to(roomVo.neighborRoomIds);
        j.at("area").get_to(roomVo.area);
        j.at("perimeter").get_to(roomVo.perimeter);
    }

    const PointVo &getCenter() const;

    void setCenter(const PointVo &center);
};

class MapRoomVo {
private:
    int width;
    int height;
    std::vector<RoomVo> rooms;
public:
    MapRoomVo();

    MapRoomVo(int width, int height, const std::vector<RoomVo> &rooms);

    int getWidth() const;

    void setWidth(int width);

    int getHeight() const;

    void setHeight(int height);

    const std::vector<RoomVo> &getRooms() const;

    void setRooms(const std::vector<RoomVo> &rooms);

    friend void to_json(json &j, const MapRoomVo &mapRoomVo) {
        j = json{
                {"width",  mapRoomVo.width},
                {"height", mapRoomVo.height},
                {"rooms",  mapRoomVo.rooms}
        };
    }

    friend void from_json(const json &j, MapRoomVo &mapRoomVo) {
        j.at("width").get_to(mapRoomVo.width);
        j.at("height").get_to(mapRoomVo.height);
        j.at("rooms").get_to(mapRoomVo.rooms);
    }
};

class RoomMergeTarget {
private:
    int targetId;
    int mergeId;
public:
    RoomMergeTarget();

    RoomMergeTarget(int targetId, int mergeId);

    int getTargetId() const;

    void setTargetId(int targetId);

    int getMergeId() const;

    void setMergeId(int mergeId);

    friend void to_json(json &j, const RoomMergeTarget &roomMergeTarget) {
        j = json{
                {"targetId", roomMergeTarget.targetId},
                {"mergeId",  roomMergeTarget.mergeId}
        };
    }

    friend void from_json(const json &j, RoomMergeTarget &mergeTarget) {
        j.at("targetId").get_to(mergeTarget.targetId);
        j.at("mergeId").get_to(mergeTarget.mergeId);
    }
};

const int ORIGINAL_SEGMENTATION = -1;

class RoomSegmentationTarget {
private:
    int targetId;//-1表示原图分割
    PointVo ps;
    PointVo pe;
public:

    RoomSegmentationTarget();

    RoomSegmentationTarget(int targetId, const PointVo &ps, const PointVo &pe);

    int getTargetId() const;

    void setTargetId(int targetId);

    const PointVo &getPs() const;

    void setPs(const PointVo &ps);

    const PointVo &getPe() const;

    void setPe(const PointVo &pe);

    friend void to_json(json &j, const RoomSegmentationTarget &roomMergeTarget) {
        j = json{
                {"targetId", roomMergeTarget.targetId},
                {"ps",       roomMergeTarget.ps},
                {"pe",       roomMergeTarget.pe}
        };
    }

    friend void from_json(const json &j, RoomSegmentationTarget &roomMergeTarget) {
        j.at("targetId").get_to(roomMergeTarget.targetId);
        j.at("ps").get_to(roomMergeTarget.ps);
        j.at("pe").get_to(roomMergeTarget.pe);
    }
};

class RoomNameTarget {
private:
    int targetId;//-1表示原图分割
    std::string name;
public:
    RoomNameTarget();

    RoomNameTarget(int targetId, const std::string &name);

    int getTargetId() const;

    void setTargetId(int targetId);

    const std::string &getName() const;

    void setName(const std::string &name);

    friend void to_json(json &j, const RoomNameTarget &roomNameTarget) {
        j = json{
                {"targetId", roomNameTarget.targetId},
                {"name",     roomNameTarget.name}
        };
    }

    friend void from_json(const json &j, RoomNameTarget &roomNameTarget) {
        j.at("targetId").get_to(roomNameTarget.targetId);
        j.at("name").get_to(roomNameTarget.name);
    }

};

class RoomCoverage {
private:
    std::string coverageId;
    std::vector<PointVo> pointList;
    std::vector<PoseVo> poseList;
public:
    RoomCoverage() {}

    RoomCoverage(std::string coverageId, const std::vector<PointVo> &pointList, const std::vector<PoseVo> &poseList)
            : coverageId(coverageId), pointList(pointList), poseList(poseList) {}

    const std::string &getCoverageId() const {
        return coverageId;
    }

    void setCoverageId(const std::string &coverageId) {
        RoomCoverage::coverageId = coverageId;
    }

    const std::vector<PointVo> &getPointList() const {
        return pointList;
    }

    void setPointList(const std::vector<PointVo> &pointList) {
        RoomCoverage::pointList = pointList;
    }

    const std::vector<PoseVo> &getPoseList() const {
        return poseList;
    }

    void setPoseList(const std::vector<PoseVo> &poseList) {
        RoomCoverage::poseList = poseList;
    }

    friend void to_json(json &j, const RoomCoverage &roomCoverage) {
        j = json{
                {"coverageId", roomCoverage.coverageId},
                {"pointList",  roomCoverage.pointList},
                {"poseList",   roomCoverage.poseList}
        };
    }

    friend void from_json(const json &j, RoomCoverage &roomCoverage) {
        j.at("coverageId").get_to(roomCoverage.coverageId);
        j.at("pointList").get_to(roomCoverage.pointList);
        j.at("poseList").get_to(roomCoverage.poseList);
    }

};

class RoomExplorationTarget {
private:
    bool partition;
    int targetId;
    int dataMode;
    int explorerMode;
public:
    RoomExplorationTarget();

    RoomExplorationTarget(int targetId, bool dataMode);

    int getTargetId() const;

    void setTargetId(int targetId);

    int getDataMode() const;

    void setDataMode(int dataMode);

    bool isPartition() const;

    void setPartition(bool partition);

    int getExplorerMode() const;

    void setExplorerMode(int explorerMode);

    friend void to_json(json &j, const RoomExplorationTarget &roomExplorationTarget) {
        j = json{
                {"partition",    roomExplorationTarget.partition},
                {"targetId",     roomExplorationTarget.targetId},
                {"dataMode",     roomExplorationTarget.dataMode},
                {"explorerMode", roomExplorationTarget.explorerMode}
        };
    }

    friend void from_json(const json &j, RoomExplorationTarget &roomExplorationTarget) {
        j.at("partition").get_to(roomExplorationTarget.partition);
        j.at("targetId").get_to(roomExplorationTarget.targetId);
        j.at("dataMode").get_to(roomExplorationTarget.dataMode);
        j.at("explorerMode").get_to(roomExplorationTarget.explorerMode);
    }
};

class PlanParam {
private:
    double robot_radius;
    int map_correction_closing_neighborhood_size;
    double grid_obstacle_offset;
    double path_eps;
    double min_cell_area;
    double max_deviation_from_track;
    int range_near_base_station;

    double room_area_factor_lower_limit;
    double room_area_factor_upper_limit;
    int neighborhood_index;
    int max_iterations;
    double min_critical_point_distance_factor;
    double max_area_for_merging;

    int distance_from_obstacles;
    int number_extension;
    int multiple_contour_spacing;

public:

    PlanParam();

    PlanParam(double robotRadius, int mapCorrectionClosingNeighborhoodSize, double gridObstacleOffset, double pathEps,
              double minCellArea, double maxDeviationFromTrack, int rangeNearBaseStation,
              double roomAreaFactorLowerLimit, double roomAreaFactorUpperLimit, int neighborhoodIndex,
              int maxIterations, double minCriticalPointDistanceFactor, double maxAreaForMerging,
              int distanceFromObstacles, int numberExtension, int multipleContourSpacing);

    friend void to_json(json &j, const PlanParam &planParam) {
        j = json{
                {"robot_radius",                             planParam.robot_radius},
                {"map_correction_closing_neighborhood_size", planParam.map_correction_closing_neighborhood_size},
                {"grid_obstacle_offset",                     planParam.grid_obstacle_offset},
                {"min_cell_area",                            planParam.min_cell_area},
                {"max_deviation_from_track",                 planParam.max_deviation_from_track},
                {"range_near_base_station",                  planParam.range_near_base_station},
                {"room_area_factor_lower_limit",             planParam.room_area_factor_lower_limit},
                {"room_area_factor_upper_limit",             planParam.room_area_factor_upper_limit},
                {"neighborhood_index",                       planParam.neighborhood_index},
                {"max_iterations",                           planParam.max_iterations},
                {"min_critical_point_distance_factor",       planParam.min_critical_point_distance_factor},
                {"max_area_for_merging",                     planParam.max_area_for_merging},
                {"distance_from_obstacles",                  planParam.distance_from_obstacles},
                {"number_extension",                         planParam.number_extension},
                {"multiple_contour_spacing",                 planParam.multiple_contour_spacing}
        };
    }

    friend void from_json(const json &j, PlanParam &planParam) {
        j.at("robot_radius").get_to(planParam.robot_radius);
        j.at("map_correction_closing_neighborhood_size").get_to(planParam.map_correction_closing_neighborhood_size);
        j.at("grid_obstacle_offset").get_to(planParam.grid_obstacle_offset);
        j.at("path_eps").get_to(planParam.path_eps);
        j.at("min_cell_area").get_to(planParam.min_cell_area);
        j.at("max_deviation_from_track").get_to(planParam.max_deviation_from_track);
        j.at("range_near_base_station").get_to(planParam.range_near_base_station);
        j.at("room_area_factor_lower_limit").get_to(planParam.room_area_factor_lower_limit);
        j.at("room_area_factor_upper_limit").get_to(planParam.room_area_factor_upper_limit);
        j.at("neighborhood_index").get_to(planParam.neighborhood_index);
        j.at("max_iterations").get_to(planParam.max_iterations);
        j.at("min_critical_point_distance_factor").get_to(planParam.min_critical_point_distance_factor);
        j.at("max_area_for_merging").get_to(planParam.max_area_for_merging);
        j.at("distance_from_obstacles").get_to(planParam.distance_from_obstacles);
        j.at("number_extension").get_to(planParam.number_extension);
        j.at("multiple_contour_spacing").get_to(planParam.multiple_contour_spacing);
    }

    double getRobotRadius() const;

    int getMapCorrectionClosingNeighborhoodSize() const;

    double getGridObstacleOffset() const;

    double getPathEps() const;

    double getMinCellArea() const;

    double getMaxDeviationFromTrack() const;

    int getRangeNearBaseStation() const;

    double getRoomAreaFactorLowerLimit() const;

    double getRoomAreaFactorUpperLimit() const;

    int getNeighborhoodIndex() const;

    int getMaxIterations() const;

    double getMinCriticalPointDistanceFactor() const;

    double getMaxAreaForMerging() const;

    int getDistanceFromObstacles() const;

    int getNumberExtension() const;

    int getMultipleContourSpacing() const;
};

#endif //APP_COMMUNICATION_ROOMVO_H
