//
// Created by lijiang on 2021/12/17.
//

#ifndef APP_COMMUNICATION_MAPINFO_H
#define APP_COMMUNICATION_MAPINFO_H

#include <string>

#include "nlohmann/json.hpp"

using json = nlohmann::json;

class MapInfo {
private:
    int map_id;
    std::string id;//real id
    std::string map_name;
    bool reset;
    float score;
public:
    MapInfo();

    MapInfo(const std::string &id, const std::string &mapName);

    virtual ~MapInfo();

    int getMapId() const;

    void setMapId(int mapId);

    const std::string &getId() const;

    void setId(const std::string &id);

    const std::string &getMapName() const;

    void setMapName(const std::string &mapName);

    bool isReset() const;

    void setReset(bool reset);

    float getScore() const;

    void setScore(float score);

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
};


#endif //APP_COMMUNICATION_MAPINFO_H
