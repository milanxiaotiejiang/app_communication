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

MapParam::MapParam() {}

MapParam::MapParam(bool save, bool reset) : save(save), reset(reset) {}

bool MapParam::isSave() const {
    return save;
}

void MapParam::setSave(bool save) {
    MapParam::save = save;
}

bool MapParam::isReset() const {
    return reset;
}

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
