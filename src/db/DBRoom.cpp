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

MapPo::MapPo(std::string id, std::string name, std::string path) : id(std::move(id)), name(std::move(name)),
                                                                   path(std::move(path)) {}

MapPo::MapPo() = default;

PlanPo::PlanPo() = default;

PlanPo::PlanPo(const std::string &mapId, double robotRadius, int mapCorrectionClosingNeighborhoodSize,
               double gridObstacleOffset, double pathEps, double minCellArea, double maxDeviationFromTrack,
               const double roomAreaFactorLowerLimit, const double roomAreaFactorUpperLimit,
               const int neighborhoodIndex, const int maxIterations, const double minCriticalPointDistanceFactor,
               const double maxAreaForMerging) : map_id(mapId), robot_radius(robotRadius),
                                                 map_correction_closing_neighborhood_size(
                                                         mapCorrectionClosingNeighborhoodSize),
                                                 grid_obstacle_offset(gridObstacleOffset), path_eps(pathEps),
                                                 min_cell_area(minCellArea),
                                                 max_deviation_from_track(maxDeviationFromTrack),
                                                 room_area_factor_lower_limit(roomAreaFactorLowerLimit),
                                                 room_area_factor_upper_limit(roomAreaFactorUpperLimit),
                                                 neighborhood_index(neighborhoodIndex), max_iterations(maxIterations),
                                                 min_critical_point_distance_factor(minCriticalPointDistanceFactor),
                                                 max_area_for_merging(maxAreaForMerging) {}
