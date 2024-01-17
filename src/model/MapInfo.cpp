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

bool MultiMapInfo::isElevator() const {
    return elevator;
}

void MultiMapInfo::setElevator(bool elevator) {
    MultiMapInfo::elevator = elevator;
}

double MultiMapInfo::getElevatorPositionX() const {
    return elevator_position_x;
}

void MultiMapInfo::setElevatorPositionX(double elevatorPositionX) {
    elevator_position_x = elevatorPositionX;
}

double MultiMapInfo::getElevatorPositionY() const {
    return elevator_position_y;
}

void MultiMapInfo::setElevatorPositionY(double elevatorPositionY) {
    elevator_position_y = elevatorPositionY;
}

double MultiMapInfo::getElevatorPositionZ() const {
    return elevator_position_z;
}

void MultiMapInfo::setElevatorPositionZ(double elevatorPositionZ) {
    elevator_position_z = elevatorPositionZ;
}

double MultiMapInfo::getElevatorOrientationX() const {
    return elevator_orientation_x;
}

void MultiMapInfo::setElevatorOrientationX(double elevatorOrientationX) {
    elevator_orientation_x = elevatorOrientationX;
}

double MultiMapInfo::getElevatorOrientationY() const {
    return elevator_orientation_y;
}

void MultiMapInfo::setElevatorOrientationY(double elevatorOrientationY) {
    elevator_orientation_y = elevatorOrientationY;
}

double MultiMapInfo::getElevatorOrientationZ() const {
    return elevator_orientation_z;
}

void MultiMapInfo::setElevatorOrientationZ(double elevatorOrientationZ) {
    elevator_orientation_z = elevatorOrientationZ;
}

double MultiMapInfo::getElevatorOrientationW() const {
    return elevator_orientation_w;
}

void MultiMapInfo::setElevatorOrientationW(double elevatorOrientationW) {
    elevator_orientation_w = elevatorOrientationW;
}

int MultiMapInfo::getFloor() const {
    return floor;
}

void MultiMapInfo::setFloor(int floor) {
    MultiMapInfo::floor = floor;
}

bool MultiMapInfo::isBaseStation() const {
    return base_station;
}

void MultiMapInfo::setBaseStation(bool baseStation) {
    base_station = baseStation;
}


long MultiMapInfo::getBuildId() const {
    return buildId;
}

void MultiMapInfo::setBuildId(long buildId) {
    MultiMapInfo::buildId = buildId;
}

const std::string &MultiMapInfo::getBuildName() const {
    return buildName;
}

void MultiMapInfo::setBuildName(const std::string &buildName) {
    MultiMapInfo::buildName = buildName;
}

double MultiMapInfo::getBaseStationPointX() const {
    return base_station_point_x;
}

void MultiMapInfo::setBaseStationPointX(double baseStationPointX) {
    base_station_point_x = baseStationPointX;
}

double MultiMapInfo::getBaseStationPointY() const {
    return base_station_point_y;
}

void MultiMapInfo::setBaseStationPointY(double baseStationPointY) {
    base_station_point_y = baseStationPointY;
}

double MultiMapInfo::getBaseStationPoseX() const {
    return base_station_pose_x;
}

void MultiMapInfo::setBaseStationPoseX(double baseStationPoseX) {
    base_station_pose_x = baseStationPoseX;
}

double MultiMapInfo::getBaseStationPoseY() const {
    return base_station_pose_y;
}

void MultiMapInfo::setBaseStationPoseY(double baseStationPoseY) {
    base_station_pose_y = baseStationPoseY;
}

double MultiMapInfo::getBaseStationPoseZ() const {
    return base_station_pose_z;
}

void MultiMapInfo::setBaseStationPoseZ(double baseStationPoseZ) {
    base_station_pose_z = baseStationPoseZ;
}

int MultiMapInfo::getMapCols() const {
    return map_cols;
}

void MultiMapInfo::setMapCols(int mapCols) {
    map_cols = mapCols;
}

int MultiMapInfo::getMapRows() const {
    return map_rows;
}

void MultiMapInfo::setMapRows(int mapRows) {
    map_rows = mapRows;
}

MultiMapInfo::MultiMapInfo(const std::string &id, const std::string &name, bool main, const std::string &path,
                           bool elevator, double elevatorPositionX, double elevatorPositionY, double elevatorPositionZ,
                           double elevatorOrientationX, double elevatorOrientationY, double elevatorOrientationZ,
                           double elevatorOrientationW, int floor, bool baseStation, double baseStationPointX,
                           double baseStationPointY, double baseStationPoseX, double baseStationPoseY,
                           double baseStationPoseZ, int mapCols, int mapRows, long buildId,
                           const std::string &buildName) : id(id), name(name), main(main), path(path),
                                                           elevator(elevator), elevator_position_x(elevatorPositionX),
                                                           elevator_position_y(elevatorPositionY),
                                                           elevator_position_z(elevatorPositionZ),
                                                           elevator_orientation_x(elevatorOrientationX),
                                                           elevator_orientation_y(elevatorOrientationY),
                                                           elevator_orientation_z(elevatorOrientationZ),
                                                           elevator_orientation_w(elevatorOrientationW), floor(floor),
                                                           base_station(baseStation),
                                                           base_station_point_x(baseStationPointX),
                                                           base_station_point_y(baseStationPointY),
                                                           base_station_pose_x(baseStationPoseX),
                                                           base_station_pose_y(baseStationPoseY),
                                                           base_station_pose_z(baseStationPoseZ), map_cols(mapCols),
                                                           map_rows(mapRows), buildId(buildId), buildName(buildName) {}

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

long BuildMapParam::getBuildId() const {
    return buildId;
}

int BuildMapParam::getFloor() const {
    return floor;
}

bool BuildMapParam::isBaseStation() const {
    return base_station;
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

BuildVo::BuildVo() {}

BuildVo::BuildVo(long id, const std::string &name) : id(id), name(name) {}

long BuildVo::getId() const {
    return id;
}

void BuildVo::setId(long id) {
    BuildVo::id = id;
}

const std::string &BuildVo::getName() const {
    return name;
}

void BuildVo::setName(const std::string &name) {
    BuildVo::name = name;
}

bool BuildVo::isMain() const {
    return main;
}

void BuildVo::setMain(bool main) {
    BuildVo::main = main;
}

BuildTimer::BuildTimer() {}

const BuildVo &BuildTimer::getBuild() const {
    return build;
}

void BuildTimer::setBuild(const BuildVo &build) {
    BuildTimer::build = build;
}

const std::vector<TimerVo> &BuildTimer::getTimers() const {
    return timers;
}

void BuildTimer::setTimers(const std::vector<TimerVo> &timers) {
    BuildTimer::timers = timers;
}
