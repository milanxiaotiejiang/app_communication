//
// Created by lijiang on 2021/12/17.
//

#include "model/MapInfo.h"

MapInfo::MapInfo() {}

MapInfo::MapInfo(const std::string &id, const std::string &mapName) : id(id), map_name(mapName) {}

MapInfo::MapInfo(const std::string &id, const std::string &mapName, double score) : id(id), map_name(mapName),
                                                                                    score(score) {}

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

const std::string &MapInfo::getId() const {
    return id;
}

void MapInfo::setId(const std::string &id) {
    MapInfo::id = id;
}

bool MapInfo::isReset() const {
    return reset;
}

void MapInfo::setReset(bool reset) {
    MapInfo::reset = reset;
}

double MapInfo::getScore() const {
    return score;
}

void MapInfo::setScore(double score) {
    MapInfo::score = score;
}

std::ostream &operator<<(std::ostream &os, const MapInfo &info) {
    os << "map_id: " << info.map_id << " id: " << info.id << " map_name: " << info.map_name << " reset: " << info.reset
       << " score: " << info.score;
    return os;
}

MapParam::MapParam() {}

bool MapParam::isSave() const {
    return save;
}

bool MapParam::isReset() const {
    return reset;
}

const std::string &MapParam::getMapName() const {
    return map_name;
}

bool MapParam::isNewMap() const {
    return new_map;
}

MapParam::MapParam(bool save, bool reset, bool newMap, const std::string &mapName) : save(save), reset(reset),
                                                                                     new_map(newMap),
                                                                                     map_name(mapName) {}

void MapParam::setReset(bool reset) {
    MapParam::reset = reset;
}


MapScore::MapScore() {}

MapScore::MapScore(const std::string &id, double score) : id(id), score(score) {}

const std::string &MapScore::getId() const {
    return id;
}

void MapScore::setId(const std::string &id) {
    MapScore::id = id;
}

double MapScore::getScore() const {
    return score;
}

void MapScore::setScore(double score) {
    MapScore::score = score;
}

std::ostream &operator<<(std::ostream &os, const MapScore &score) {
    os << "id: " << score.id << " score: " << score.score << " map_name: " << score.map_name;
    return os;
}
