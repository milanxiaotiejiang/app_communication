//
// Created by lijiang on 2021/12/17.
//

#ifndef APP_COMMUNICATION_MAPINFO_H
#define APP_COMMUNICATION_MAPINFO_H

#include <string>
#include <ostream>

#include "nlohmann/json.hpp"

using json = nlohmann::json;

class MapParam {
private:
    bool save;
    bool reset;
    bool new_map;
    std::string map_name;
public:
    MapParam();

    bool isSave() const;

    bool isReset() const;

    bool isNewMap() const;

    const std::string &getMapName() const;

    MapParam(bool save, bool reset, bool newMap, const std::string &mapName);

    void setReset(bool reset);

    friend void to_json(json &j, const MapParam &b) {
        j = json{
                {"save",  b.save},
                {"reset", b.reset},
        };
    }

    friend void from_json(const json &j, MapParam &b) {
        j.at("save").get_to(b.save);
        j.at("reset").get_to(b.reset);
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


#endif //APP_COMMUNICATION_MAPINFO_H
