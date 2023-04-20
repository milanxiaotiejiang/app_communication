//
// Created by lijiang on 2021/12/17.
//

#include "model/MapInfo.h"

MapInfo::MapInfo() {}

MapInfo::MapInfo(const std::string &id, const std::string &mapName) : id(id), map_name(mapName) {}

MapInfo::~MapInfo() {

}

int MapInfo::getMapId() const {
    return map_id;
}

void MapInfo::setMapId(int mapId) {
    map_id = mapId;
}

const std::string &MapInfo::getMapName() const {
    return map_name;
}

void MapInfo::setMapName(const std::string &mapName) {
    map_name = mapName;
}
