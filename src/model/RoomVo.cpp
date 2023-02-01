//
// Created by Looper on 2022/9/16.
//

#include "model/RoomVo.h"

#include <utility>

PointVo::PointVo() {}

PointVo::PointVo(int x, int y) : x(x), y(y) {}

int PointVo::getX() const {
    return x;
}

void PointVo::setX(int x) {
    PointVo::x = x;
}

int PointVo::getY() const {
    return y;
}

void PointVo::setY(int y) {
    PointVo::y = y;
}

PoseVo::PoseVo() {}

PoseVo::PoseVo(float x, float y, float theta) : x(x), y(y), theta(theta) {}

float PoseVo::getX() const {
    return x;
}

void PoseVo::setX(float x) {
    PoseVo::x = x;
}

float PoseVo::getY() const {
    return y;
}

void PoseVo::setY(float y) {
    PoseVo::y = y;
}

float PoseVo::getTheta() const {
    return theta;
}

void PoseVo::setTheta(float theta) {
    PoseVo::theta = theta;
}

RoomVo::RoomVo() {}

RoomVo::RoomVo(int id, std::string name, PointVo center,
               const std::vector<PointVo> &memberPoints,
               const std::vector<int> &neighborRoomIds,
               double area, double perimeter) : id(id), name(std::move(name)), center(center),
                                                memberPoints(memberPoints),
                                                neighborRoomIds(neighborRoomIds),
                                                area(area),
                                                perimeter(perimeter) {}

int RoomVo::getId() const {
    return id;
}

void RoomVo::setId(int id) {
    RoomVo::id = id;
}

const PointVo &RoomVo::getCenter() const {
    return center;
}

void RoomVo::setCenter(const PointVo &center) {
    RoomVo::center = center;
}

const std::vector<PointVo> &RoomVo::getMemberPoints() const {
    return memberPoints;
}

void RoomVo::setMemberPoints(const std::vector<PointVo> &memberPoints) {
    RoomVo::memberPoints = memberPoints;
}

const std::vector<int> &RoomVo::getNeighborRoomIds() const {
    return neighborRoomIds;
}

void RoomVo::setNeighborRoomIds(const std::vector<int> &neighborRoomIds) {
    RoomVo::neighborRoomIds = neighborRoomIds;
}

double RoomVo::getArea() const {
    return area;
}

void RoomVo::setArea(double area) {
    RoomVo::area = area;
}

double RoomVo::getPerimeter() const {
    return perimeter;
}

void RoomVo::setPerimeter(double perimeter) {
    RoomVo::perimeter = perimeter;
}

RoomMergeTarget::RoomMergeTarget() {}

RoomMergeTarget::RoomMergeTarget(int targetId, int mergeId) : targetId(targetId), mergeId(mergeId) {}

int RoomMergeTarget::getTargetId() const {
    return targetId;
}

void RoomMergeTarget::setTargetId(int targetId) {
    RoomMergeTarget::targetId = targetId;
}

int RoomMergeTarget::getMergeId() const {
    return mergeId;
}

void RoomMergeTarget::setMergeId(int mergeId) {
    RoomMergeTarget::mergeId = mergeId;
}

RoomSegmentationTarget::RoomSegmentationTarget() {}

RoomSegmentationTarget::RoomSegmentationTarget(int targetId, const PointVo &ps, const PointVo &pe) : targetId(targetId),
                                                                                                     ps(ps), pe(pe) {}

int RoomSegmentationTarget::getTargetId() const {
    return targetId;
}

void RoomSegmentationTarget::setTargetId(int targetId) {
    RoomSegmentationTarget::targetId = targetId;
}

const PointVo &RoomSegmentationTarget::getPs() const {
    return ps;
}

void RoomSegmentationTarget::setPs(const PointVo &ps) {
    RoomSegmentationTarget::ps = ps;
}

const PointVo &RoomSegmentationTarget::getPe() const {
    return pe;
}

void RoomSegmentationTarget::setPe(const PointVo &pe) {
    RoomSegmentationTarget::pe = pe;
}

MapRoomVo::MapRoomVo() {}

MapRoomVo::MapRoomVo(int width, int height, const std::vector<RoomVo> &rooms) : width(width), height(height),
                                                                                rooms(rooms) {}

int MapRoomVo::getWidth() const {
    return width;
}

void MapRoomVo::setWidth(int width) {
    MapRoomVo::width = width;
}

int MapRoomVo::getHeight() const {
    return height;
}

void MapRoomVo::setHeight(int height) {
    MapRoomVo::height = height;
}

const std::vector<RoomVo> &MapRoomVo::getRooms() const {
    return rooms;
}

void MapRoomVo::setRooms(const std::vector<RoomVo> &rooms) {
    MapRoomVo::rooms = rooms;
}

RoomNameTarget::RoomNameTarget() {}

RoomNameTarget::RoomNameTarget(int targetId, const std::string &name) : targetId(targetId), name(name) {}

int RoomNameTarget::getTargetId() const {
    return targetId;
}

void RoomNameTarget::setTargetId(int targetId) {
    RoomNameTarget::targetId = targetId;
}

const std::string &RoomNameTarget::getName() const {
    return name;
}

void RoomNameTarget::setName(const std::string &name) {
    RoomNameTarget::name = name;
}

RoomExplorationTarget::RoomExplorationTarget() {}

RoomExplorationTarget::RoomExplorationTarget(int targetId, bool dataMode) : targetId(targetId), dataMode(dataMode) {}

int RoomExplorationTarget::getTargetId() const {
    return targetId;
}

void RoomExplorationTarget::setTargetId(int targetId) {
    RoomExplorationTarget::targetId = targetId;
}

int RoomExplorationTarget::getDataMode() const {
    return dataMode;
}

void RoomExplorationTarget::setDataMode(int dataMode) {
    RoomExplorationTarget::dataMode = dataMode;
}

int RoomExplorationTarget::getExplorerMode() const {
    return explorerMode;
}

void RoomExplorationTarget::setExplorerMode(int explorerMode) {
    RoomExplorationTarget::explorerMode = explorerMode;
}

bool RoomExplorationTarget::isPartition() const {
    return partition;
}

void RoomExplorationTarget::setPartition(bool partition) {
    partition = partition;
}

PlanParam::PlanParam() {}

PlanParam::PlanParam(double robotRadius, int mapCorrectionClosingNeighborhoodSize, double gridObstacleOffset,
                     double pathEps, double minCellArea, double maxDeviationFromTrack, int rangeNearBaseStation,
                     double roomAreaFactorLowerLimit, double roomAreaFactorUpperLimit, int neighborhoodIndex,
                     int maxIterations, double minCriticalPointDistanceFactor, double maxAreaForMerging,
                     int distanceFromObstacles, int numberExtension, int multipleContourSpacing) : robot_radius(
        robotRadius), map_correction_closing_neighborhood_size(mapCorrectionClosingNeighborhoodSize),
                                                                                                   grid_obstacle_offset(
                                                                                                           gridObstacleOffset),
                                                                                                   path_eps(pathEps),
                                                                                                   min_cell_area(
                                                                                                           minCellArea),
                                                                                                   max_deviation_from_track(
                                                                                                           maxDeviationFromTrack),
                                                                                                   range_near_base_station(
                                                                                                           rangeNearBaseStation),
                                                                                                   room_area_factor_lower_limit(
                                                                                                           roomAreaFactorLowerLimit),
                                                                                                   room_area_factor_upper_limit(
                                                                                                           roomAreaFactorUpperLimit),
                                                                                                   neighborhood_index(
                                                                                                           neighborhoodIndex),
                                                                                                   max_iterations(
                                                                                                           maxIterations),
                                                                                                   min_critical_point_distance_factor(
                                                                                                           minCriticalPointDistanceFactor),
                                                                                                   max_area_for_merging(
                                                                                                           maxAreaForMerging),
                                                                                                   distance_from_obstacles(
                                                                                                           distanceFromObstacles),
                                                                                                   number_extension(
                                                                                                           numberExtension),
                                                                                                   multiple_contour_spacing(
                                                                                                           multipleContourSpacing) {}

double PlanParam::getRobotRadius() const {
    return robot_radius;
}

int PlanParam::getMapCorrectionClosingNeighborhoodSize() const {
    return map_correction_closing_neighborhood_size;
}

double PlanParam::getGridObstacleOffset() const {
    return grid_obstacle_offset;
}

double PlanParam::getPathEps() const {
    return path_eps;
}

double PlanParam::getMinCellArea() const {
    return min_cell_area;
}

double PlanParam::getMaxDeviationFromTrack() const {
    return max_deviation_from_track;
}

int PlanParam::getRangeNearBaseStation() const {
    return range_near_base_station;
}

double PlanParam::getRoomAreaFactorLowerLimit() const {
    return room_area_factor_lower_limit;
}

double PlanParam::getRoomAreaFactorUpperLimit() const {
    return room_area_factor_upper_limit;
}

int PlanParam::getNeighborhoodIndex() const {
    return neighborhood_index;
}

int PlanParam::getMaxIterations() const {
    return max_iterations;
}

double PlanParam::getMinCriticalPointDistanceFactor() const {
    return min_critical_point_distance_factor;
}

double PlanParam::getMaxAreaForMerging() const {
    return max_area_for_merging;
}

int PlanParam::getDistanceFromObstacles() const {
    return distance_from_obstacles;
}

int PlanParam::getNumberExtension() const {
    return number_extension;
}

int PlanParam::getMultipleContourSpacing() const {
    return multiple_contour_spacing;
}
