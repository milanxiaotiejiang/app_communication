//
// Created by Looper on 2022/9/13.
//

#include <utility>

#include "db/segmentation_model.h"

RoomPo::RoomPo(std::string oMapId, std::string name, int value, double area, double centerX, double centerY,
               double perimeter, std::string neighborRoomIds) : o_map_id(std::move(oMapId)), name(std::move(name)),
                                                                value(value),
                                                                area(area), center_x(centerX), center_y(centerY),
                                                                perimeter(perimeter),
                                                                neighbor_room_ids(std::move(neighborRoomIds)) {}

std::ostream &operator<<(std::ostream &os, const RoomPo &room) {
    os << "id: " << room.id << " o_map_id: " << room.o_map_id << " value: " << room.value << " area: " << room.area
       << " center_x: " << room.center_x << " center_y: " << room.center_y << " perimeter: " << room.perimeter
       << " neighbor_room_ids: " << room.neighbor_room_ids;
    return os;
}

RoomPo::RoomPo() = default;

std::ostream &operator<<(std::ostream &os, const MapPo &po) {
    os << "id: " << po.id << " name: " << po.name << " path: " << po.path << " main: " << po.main;
    return os;
}

MapPo::MapPo(const std::string &id, const std::string &name, const std::string &path, bool main, bool elevator,
             double elevatorPositionX, double elevatorPositionY, double elevatorPositionZ, double elevatorOrientationX,
             double elevatorOrientationY, double elevatorOrientationZ, double elevatorOrientationW)
        : id(id),
          name(name),
          path(path),
          main(main),
          elevator(elevator),
          elevator_position_x(elevatorPositionX),
          elevator_position_y(elevatorPositionY),
          elevator_position_z(elevatorPositionZ),
          elevator_orientation_x(elevatorOrientationX),
          elevator_orientation_y(elevatorOrientationY),
          elevator_orientation_z(elevatorOrientationZ),
          elevator_orientation_w(elevatorOrientationW) {}

MapPo::MapPo() = default;

PlanPo::PlanPo() = default;

PlanPo::PlanPo(const std::string &mapId, double robotRadius, int mapCorrectionClosingNeighborhoodSize,
               double gridObstacleOffset, double pathEps, double minCellArea, double maxDeviationFromTrack,
               int rangeNearBaseStation, double roomAreaFactorLowerLimit, double roomAreaFactorUpperLimit,
               int neighborhoodIndex, int maxIterations, double minCriticalPointDistanceFactor,
               double maxAreaForMerging, int distanceFromObstacles, int numberExtension, int multipleContourSpacing,
               int randomNumberGenerationRatio, int boundaryMinArea, int version)
        : map_id(mapId),
          robot_radius(robotRadius),
          map_correction_closing_neighborhood_size(mapCorrectionClosingNeighborhoodSize),
          grid_obstacle_offset(gridObstacleOffset),
          path_eps(pathEps),
          min_cell_area(minCellArea),
          max_deviation_from_track(maxDeviationFromTrack),
          range_near_base_station(rangeNearBaseStation),
          room_area_factor_lower_limit(roomAreaFactorLowerLimit),
          room_area_factor_upper_limit(roomAreaFactorUpperLimit),
          neighborhood_index(neighborhoodIndex),
          max_iterations(maxIterations),
          min_critical_point_distance_factor(minCriticalPointDistanceFactor),
          max_area_for_merging(maxAreaForMerging),
          distance_from_obstacles(distanceFromObstacles),
          number_extension(numberExtension),
          multiple_contour_spacing(multipleContourSpacing),
          random_number_generation_ratio(randomNumberGenerationRatio),
          boundary_min_area(boundaryMinArea),
          version(version) {}

Gate::Gate() {}

Gate::Gate(long id, const std::string &oMapId, double startX, double startY, double endX, double endY,
           double leftPositionX, double leftPositionY, double leftPositionZ, double leftOrientationX,
           double leftOrientationY, double leftOrientationZ, double leftOrientationW, double rightPositionX,
           double rightPositionY, double rightPositionZ, double rightOrientationX, double rightOrientationY,
           double rightOrientationZ, double rightOrientationW, const std::string &leftGateId,
           const std::string &rightGateId, const std::string &factoryID, int version)
        : id(id), o_map_id(oMapId), start_x(startX), start_y(startY),
          end_x(endX), end_y(endY), left_position_x(leftPositionX),
          left_position_y(leftPositionY),
          left_position_z(leftPositionZ),
          left_orientation_x(leftOrientationX),
          left_orientation_y(leftOrientationY),
          left_orientation_z(leftOrientationZ),
          left_orientation_w(leftOrientationW),
          right_position_x(rightPositionX),
          right_position_y(rightPositionY),
          right_position_z(rightPositionZ),
          right_orientation_x(rightOrientationX),
          right_orientation_y(rightOrientationY),
          right_orientation_z(rightOrientationZ),
          right_orientation_w(rightOrientationW),
          left_gate_ID(leftGateId),
          right_gate_ID(rightGateId),
          factory_ID(factoryID),
          version(version) {}

Gate::Gate(const std::string &oMapId, double startX, double startY, double endX, double endY, double leftPositionX,
           double leftPositionY, double leftPositionZ, double leftOrientationX, double leftOrientationY,
           double leftOrientationZ, double leftOrientationW, double rightPositionX, double rightPositionY,
           double rightPositionZ, double rightOrientationX, double rightOrientationY, double rightOrientationZ,
           double rightOrientationW) : o_map_id(oMapId), start_x(startX), start_y(startY), end_x(endX), end_y(endY),
                                       left_position_x(leftPositionX), left_position_y(leftPositionY),
                                       left_position_z(leftPositionZ), left_orientation_x(leftOrientationX),
                                       left_orientation_y(leftOrientationY), left_orientation_z(leftOrientationZ),
                                       left_orientation_w(leftOrientationW), right_position_x(rightPositionX),
                                       right_position_y(rightPositionY), right_position_z(rightPositionZ),
                                       right_orientation_x(rightOrientationX), right_orientation_y(rightOrientationY),
                                       right_orientation_z(rightOrientationZ), right_orientation_w(rightOrientationW) {}

std::ostream &operator<<(std::ostream &os, const Gate &gate) {
    os << "id: " << gate.id << " o_map_id: " << gate.o_map_id << " start_x: " << gate.start_x << " start_y: "
       << gate.start_y << " end_x: " << gate.end_x << " end_y: " << gate.end_y;
    return os;
}

BuildPo::BuildPo() {}

BuildPo::BuildPo(long id, const std::string &name) : id(id), name(name) {}

std::ostream &operator<<(std::ostream &os, const BuildPo &build) {
    os << "id: " << build.id << " name: " << build.name;
    return os;
}

BuildMapMapping::BuildMapMapping() {}

BuildMapMapping::BuildMapMapping(long oBuildId, const std::string &oMapId) : o_build_id(oBuildId), o_map_id(oMapId) {}
