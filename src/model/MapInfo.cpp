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

const std::string &MultiMapInfo::getId() const {
    return id;
}

void MultiMapInfo::setId(const std::string &id) {
    MultiMapInfo::id = id;
}

const std::string &MultiMapInfo::getName() const {
    return name;
}

void MultiMapInfo::setName(const std::string &name) {
    MultiMapInfo::name = name;
}

bool MultiMapInfo::isMain() const {
    return main;
}

void MultiMapInfo::setMain(bool main) {
    MultiMapInfo::main = main;
}

const std::string &MultiMapInfo::getPath() const {
    return path;
}

void MultiMapInfo::setPath(const std::string &path) {
    MultiMapInfo::path = path;
}

MultiMapInfo::MultiMapInfo() {}

MultiMapInfo::MultiMapInfo(const std::string &id, const std::string &name, bool main, const std::string &path) : id(id),
                                                                                                                 name(name),
                                                                                                                 main(main),
                                                                                                                 path(path) {}

std::ostream &operator<<(std::ostream &os, const MultiMapInfo &info) {
    os << "id: " << info.id << " name: " << info.name << " main: " << info.main << " path: " << info.path;
    return os;
}

BuildMapParam::BuildMapParam() {}

bool BuildMapParam::isSave() const {
    return save;
}

bool BuildMapParam::isReset() const {
    return reset;
}

const std::string &BuildMapParam::getMapName() const {
    return map_name;
}

bool BuildMapParam::isNewMap() const {
    return new_map;
}

BuildMapParam::BuildMapParam(bool save, bool reset, bool newMap, const std::string &mapName) : save(save), reset(reset),
                                                                                               new_map(newMap),
                                                                                               map_name(mapName) {}

void BuildMapParam::setReset(bool reset) {
    BuildMapParam::reset = reset;
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
