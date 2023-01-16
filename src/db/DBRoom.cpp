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
