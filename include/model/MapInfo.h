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
    std::string map_name;
public:
    MapInfo();

    virtual ~MapInfo();

    MapInfo(int mapId, const std::string &mapName);

    int getMapId() const;

    void setMapId(int mapId);

    const std::string &getMapName() const;

    void setMapName(const std::string &mapName);

    friend void to_json(json &j, const MapInfo &b) {
        j = json{
                {"map_id",   b.map_id},
                {"map_name", b.map_name},
        };
    }

    friend void from_json(const json &j, MapInfo &b) {
        j.at("map_id").get_to(b.map_id);
        j.at("map_name").get_to(b.map_name);
    }
};


#endif //APP_COMMUNICATION_MAPINFO_H
