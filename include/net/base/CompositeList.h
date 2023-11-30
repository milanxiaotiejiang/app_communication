//
// Created by io on 23-11-29.
//

#ifndef APP_COMMUNICATION_COMPOSITELIST_H
#define APP_COMMUNICATION_COMPOSITELIST_H


#include <string>
#include <vector>
#include "model/RoomVo.h"

class CompositeFloatList {
private:
    std::string map_id;
    std::vector<std::vector<float>> floats;
public:
    CompositeFloatList() {}

    CompositeFloatList(const std::string &mapId, const std::vector<std::vector<float>> &floats) : map_id(mapId),
                                                                                                  floats(floats) {}

    const std::string &getMapId() const {
        return map_id;
    }

    void setMapId(const std::string &mapId) {
        map_id = mapId;
    }

    const std::vector<std::vector<float>> &getFloats() const {
        return floats;
    }

    void setFloats(const std::vector<std::vector<float>> &floats) {
        CompositeFloatList::floats = floats;
    }

    friend void to_json(json &j, const CompositeFloatList &model) {
        j = json{
                {"map_id", model.map_id},
                {"floats", model.floats},
        };
    }

    friend void from_json(const json &j, CompositeFloatList &model) {
        j.at("map_id").get_to(model.map_id);
        j.at("floats").get_to(model.floats);
    }
};

class CompositePointList {
private:
    std::string map_id;
    std::vector<std::vector<PointVo>> points;
public:
    CompositePointList() {}

    CompositePointList(const std::string &mapId, const std::vector<std::vector<PointVo>> &points) : map_id(mapId),
                                                                                                    points(points) {}

    const std::string &getMapId() const {
        return map_id;
    }

    void setMapId(const std::string &mapId) {
        map_id = mapId;
    }

    const std::vector<std::vector<PointVo>> &getPoints() const {
        return points;
    }

    void setPoints(const std::vector<std::vector<PointVo>> &points) {
        CompositePointList::points = points;
    }

    friend void to_json(json &j, const CompositePointList &model) {
        j = json{
                {"map_id", model.map_id},
                {"points", model.points},
        };
    }

    friend void from_json(const json &j, CompositePointList &model) {
        j.at("map_id").get_to(model.map_id);
        j.at("points").get_to(model.points);
    }
};


#endif //APP_COMMUNICATION_COMPOSITELIST_H
