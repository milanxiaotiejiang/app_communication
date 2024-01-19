//
// Created by lijiang on 2021/12/17.
//

#ifndef APP_COMMUNICATION_MAPINFO_H
#define APP_COMMUNICATION_MAPINFO_H

#include <string>
#include <ostream>

#include "nlohmann/json.hpp"
#include "task.h"

using json = nlohmann::json;

class BuildVo {
private:
    long id{};
    std::string name;
    bool main;
public:
    BuildVo();

    BuildVo(long id, const std::string &name);

    long getId() const;

    void setId(long id);

    const std::string &getName() const;

    void setName(const std::string &name);

    bool isMain() const;

    void setMain(bool main);

    friend void to_json(json &j, const BuildVo &b) {
        j = json{
                {"id",   b.id},
                {"name", b.name},
                {"main", b.main},
        };
    }

    friend void from_json(const json &j, BuildVo &b) {
        j.at("id").get_to(b.id);
        j.at("name").get_to(b.name);
        j.at("main").get_to(b.main);
    }
};


class BuildMapParam {
private:
    bool save{false};
    bool reset{false};
    bool new_map{false};
    std::string map_name{""};

    long buildId{0};
    int floor{1};
    bool base_station;
public:
    BuildMapParam();

    bool isSave() const;

    bool isReset() const;

    bool isNewMap() const;

    const std::string &getMapName() const;

    long getBuildId() const;

    int getFloor() const;

    bool isBaseStation() const;

    BuildMapParam(bool save, bool reset, bool newMap, const std::string &mapName);

    void setReset(bool reset);

    friend void to_json(json &j, const BuildMapParam &b) {
        j = json{
                {"save",     b.save},
                {"reset",    b.reset},
                {"new_map",  b.new_map},
                {"map_name", b.map_name},
                {"build_id", b.buildId},
                {"floor", b.floor},
                {"base_station", b.base_station},
        };
    }

    friend void from_json(const json &j, BuildMapParam &b) {
        j.at("save").get_to(b.save);
        j.at("reset").get_to(b.reset);
        j.at("new_map").get_to(b.new_map);
        j.at("map_name").get_to(b.map_name);
        j.at("build_id").get_to(b.buildId);
        j.at("floor").get_to(b.floor);
        j.at("base_station").get_to(b.base_station);
    }
};

class MapScore {
private:
    std::string id;//real id
    double score;
    std::string map_name;
public:
    MapScore();

    MapScore(const std::string &id, double score);

    const std::string &getId() const;

    void setId(const std::string &id);

    double getScore() const;

    void setScore(double score);

    friend void to_json(json &j, const MapScore &b) {
        j = json{
                {"id",    b.id},
                {"score", b.score},
        };
    }

    friend void from_json(const json &j, MapScore &b) {
        j.at("id").get_to(b.id);
        j.at("score").get_to(b.score);
    }

    friend std::ostream &operator<<(std::ostream &os, const MapScore &score);
};

class MapInfo {
private:
    int map_id;
    std::string id;//real id
    std::string map_name;
    bool reset;
    double score;
public:
    MapInfo();

    MapInfo(const std::string &id, const std::string &mapName);

    MapInfo(const std::string &id, const std::string &mapName, double score);

    virtual ~MapInfo();

    int getMapId() const;

    void setMapId(int mapId);

    const std::string &getId() const;

    void setId(const std::string &id);

    const std::string &getMapName() const;

    void setMapName(const std::string &mapName);

    bool isReset() const;

    void setReset(bool reset);

    double getScore() const;

    void setScore(double score);

    friend void to_json(json &j, const MapInfo &b) {
        j = json{
                {"id",       b.id},
                {"map_id",   b.map_id},
                {"map_name", b.map_name},
                {"reset",    b.reset},
                {"score",    b.score},
        };
    }

    friend void from_json(const json &j, MapInfo &b) {
        if (j.contains("id")) {
            j.at("id").get_to(b.id);
        }
        j.at("map_id").get_to(b.map_id);
        j.at("map_name").get_to(b.map_name);
        if (j.contains("reset")) {
            j.at("reset").get_to(b.reset);
        } else {
            b.reset = false;
        }
        if (j.contains("score")) {
            j.at("score").get_to(b.score);
        }
    }

    friend std::ostream &operator<<(std::ostream &os, const MapInfo &info);
};

class MultiMapInfo {
private:
    std::string id;
    std::string name;
    bool main;
    std::string path;

    bool elevator;
    double elevator_position_x{};
    double elevator_position_y{};
    double elevator_position_z{};
    double elevator_orientation_x{};
    double elevator_orientation_y{};
    double elevator_orientation_z{};
    double elevator_orientation_w{};

    int floor;
    bool base_station;
    double base_station_point_x;
    double base_station_point_y;
    double base_station_pose_x;
    double base_station_pose_y;
    double base_station_pose_z;
    int map_cols;
    int map_rows;

    long buildId{};
    std::string buildName;
public:
    MultiMapInfo();

    MultiMapInfo(const std::string &id, const std::string &name, bool main, const std::string &path);

    MultiMapInfo(const std::string &id, const std::string &name, bool main, const std::string &path, bool elevator,
                 double elevatorPositionX, double elevatorPositionY, double elevatorPositionZ,
                 double elevatorOrientationX, double elevatorOrientationY, double elevatorOrientationZ,
                 double elevatorOrientationW, int floor, bool baseStation, double baseStationPointX,
                 double baseStationPointY, double baseStationPoseX, double baseStationPoseY, double baseStationPoseZ,
                 int mapCols, int mapRows, long buildId, const std::string &buildName);

    const std::string &getId() const;

    void setId(const std::string &id);

    const std::string &getName() const;

    void setName(const std::string &name);

    bool isMain() const;

    void setMain(bool main);

    const std::string &getPath() const;

    void setPath(const std::string &path);

    bool isElevator() const;

    void setElevator(bool elevator);

    double getElevatorPositionX() const;

    void setElevatorPositionX(double elevatorPositionX);

    double getElevatorPositionY() const;

    void setElevatorPositionY(double elevatorPositionY);

    double getElevatorPositionZ() const;

    void setElevatorPositionZ(double elevatorPositionZ);

    double getElevatorOrientationX() const;

    void setElevatorOrientationX(double elevatorOrientationX);

    double getElevatorOrientationY() const;

    void setElevatorOrientationY(double elevatorOrientationY);

    double getElevatorOrientationZ() const;

    void setElevatorOrientationZ(double elevatorOrientationZ);

    double getElevatorOrientationW() const;

    void setElevatorOrientationW(double elevatorOrientationW);

    int getFloor() const;

    void setFloor(int floor);

    bool isBaseStation() const;

    void setBaseStation(bool baseStation);

    long getBuildId() const;

    void setBuildId(long buildId);

    const std::string &getBuildName() const;

    void setBuildName(const std::string &buildName);

    double getBaseStationPositionX() const;

    void setBaseStationPositionX(double baseStationPositionX);

    double getBaseStationPositionY() const;

    void setBaseStationPositionY(double baseStationPositionY);

    friend void to_json(json &j, const MultiMapInfo &b) {
        j = json{
                {"id",                     b.id},
                {"name",                   b.name},
                {"main",                   b.main},
                {"path",                   b.path},
                {"elevator",               b.elevator},
                {"elevator_position_x",    b.elevator_position_x},
                {"elevator_position_y",    b.elevator_position_y},
                {"elevator_position_z",    b.elevator_position_z},
                {"elevator_orientation_x", b.elevator_orientation_x},
                {"elevator_orientation_y", b.elevator_orientation_y},
                {"elevator_orientation_z", b.elevator_orientation_z},
                {"elevator_orientation_w", b.elevator_orientation_w},
                {"floor",                  b.floor},
                {"base_station",           b.base_station},
                {"base_station_point_x",   b.base_station_point_x},
                {"base_station_point_y",   b.base_station_point_y},
                {"base_station_pose_x",    b.base_station_pose_x},
                {"base_station_pose_y",    b.base_station_pose_y},
                {"base_station_pose_z",    b.base_station_pose_z},
                {"map_cols",               b.map_cols},
                {"map_rows",               b.map_rows},
                {"build_id",               b.buildId},
                {"build_name",             b.buildName},
        };
    }

    friend void from_json(const json &j, MultiMapInfo &b) {
        j.at("id").get_to(b.id);
        j.at("name").get_to(b.name);
        j.at("main").get_to(b.main);
        j.at("path").get_to(b.path);
        j.at("elevator").get_to(b.elevator);
        j.at("elevator_position_x").get_to(b.elevator_position_x);
        j.at("elevator_position_y").get_to(b.elevator_position_y);
        j.at("elevator_position_z").get_to(b.elevator_position_z);
        j.at("elevator_orientation_x").get_to(b.elevator_orientation_x);
        j.at("elevator_orientation_y").get_to(b.elevator_orientation_y);
        j.at("elevator_orientation_z").get_to(b.elevator_orientation_z);
        j.at("elevator_orientation_w").get_to(b.elevator_orientation_w);
        j.at("floor").get_to(b.floor);
        j.at("base_station").get_to(b.base_station);
        j.at("base_station_point_x").get_to(b.base_station_point_x);
        j.at("base_station_point_y").get_to(b.base_station_point_y);
        j.at("base_station_pose_x").get_to(b.base_station_pose_x);
        j.at("base_station_pose_y").get_to(b.base_station_pose_y);
        j.at("base_station_pose_z").get_to(b.base_station_pose_z);
        j.at("map_cols").get_to(b.map_cols);
        j.at("map_rows").get_to(b.map_rows);
        j.at("build_id").get_to(b.buildId);
        j.at("build_name").get_to(b.buildName);
    }

    friend std::ostream &operator<<(std::ostream &os, const MultiMapInfo &info);

    double getBaseStationPointX() const;

    void setBaseStationPointX(double baseStationPointX);

    double getBaseStationPointY() const;

    void setBaseStationPointY(double baseStationPointY);

    double getBaseStationPoseX() const;

    void setBaseStationPoseX(double baseStationPoseX);

    double getBaseStationPoseY() const;

    void setBaseStationPoseY(double baseStationPoseY);

    double getBaseStationPoseZ() const;

    void setBaseStationPoseZ(double baseStationPoseZ);

    int getMapCols() const;

    void setMapCols(int mapCols);

    int getMapRows() const;

    void setMapRows(int mapRows);
};

class ModifyMapName {
private:
    std::string id;
    std::string name;
public:
    ModifyMapName() {}

    ModifyMapName(const std::string &id, const std::string &name) : id(id), name(name) {}

    const std::string &getId() const {
        return id;
    }

    void setId(const std::string &id) {
        ModifyMapName::id = id;
    }

    const std::string &getName() const {
        return name;
    }

    void setName(const std::string &name) {
        ModifyMapName::name = name;
    }

    friend void to_json(json &j, const ModifyMapName &b) {
        j = json{
                {"id",   b.id},
                {"name", b.name},
        };
    }

    friend void from_json(const json &j, ModifyMapName &b) {
        j.at("id").get_to(b.id);
        j.at("name").get_to(b.name);
    }

};

struct MapImageRequest {
    std::string map_id;

    friend void to_json(json &j, const MapImageRequest &mapImage) {
        j = json{
                {"map_id", mapImage.map_id},
        };
    }

    friend void from_json(const json &j, MapImageRequest &mapImage) {
        j.at("map_id").get_to(mapImage.map_id);
    }
};

struct MapImageResponse {
    int type{1};
    std::string image;
    int width;
    int height;

    friend void to_json(json &j, const MapImageResponse &mapImage) {
        j = json{
                {"type",   mapImage.type},
                {"image",  mapImage.image},
                {"width",  mapImage.width},
                {"height", mapImage.height},
        };
    }

    friend void from_json(const json &j, MapImageResponse &mapImage) {
        j.at("type").get_to(mapImage.type);
        j.at("image").get_to(mapImage.image);
        j.at("width").get_to(mapImage.width);
        j.at("height").get_to(mapImage.height);
    }
};

struct MapBaseStation {
    std::string map_id;
    bool base_station;

    friend void to_json(json &j, const MapBaseStation &bean) {
        j = json{
                {"map_id",       bean.map_id},
                {"base_station", bean.base_station},
        };
    }

    friend void from_json(const json &j, MapBaseStation &bean) {
        j.at("map_id").get_to(bean.map_id);
        j.at("base_station").get_to(bean.base_station);
    }
};

struct MapFloor {
    std::string map_id;
    int floor;

    friend void to_json(json &j, const MapFloor &bean) {
        j = json{
                {"map_id", bean.map_id},
                {"floor",  bean.floor},
        };
    }

    friend void from_json(const json &j, MapFloor &bean) {
        j.at("map_id").get_to(bean.map_id);
        j.at("floor").get_to(bean.floor);
    }
};

struct MapElevator {
    std::string map_id;
    bool elevator;

    friend void to_json(json &j, const MapElevator &bean) {
        j = json{
                {"map_id",   bean.map_id},
                {"elevator", bean.elevator},
        };
    }

    friend void from_json(const json &j, MapElevator &bean) {
        j.at("map_id").get_to(bean.map_id);
        j.at("elevator").get_to(bean.elevator);
    }
};

struct AttachBuildMap {
    long buildId;
    std::string mapId;

    friend void to_json(json &j, const AttachBuildMap &bean) {
        j = json{
                {"build_id", bean.buildId},
                {"map_id",   bean.mapId},
        };
    }

    friend void from_json(const json &j, AttachBuildMap &bean) {
        j.at("build_id").get_to(bean.buildId);
        j.at("map_id").get_to(bean.mapId);
    }

};

class BuildTimer {
private:
    BuildVo build;
    std::vector<TimerVo> timers;
public:
    BuildTimer();

    const BuildVo &getBuild() const;

    void setBuild(const BuildVo &build);

    const std::vector<TimerVo> &getTimers() const;

    void setTimers(const std::vector<TimerVo> &timers);

    friend void to_json(json &j, const BuildTimer &bean) {
        j = json{
                {"build",  bean.build},
                {"timers", bean.timers},
        };
    }

    friend void from_json(const json &j, BuildTimer &bean) {
        j.at("build").get_to(bean.build);
        j.at("timers").get_to(bean.timers);
    }
};


#endif //APP_COMMUNICATION_MAPINFO_H
