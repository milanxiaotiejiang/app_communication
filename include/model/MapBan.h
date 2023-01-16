//
// Created by lijiang on 2021/12/17.
//

#ifndef APP_COMMUNICATION_MAPBAN_H
#define APP_COMMUNICATION_MAPBAN_H

#include "string"
#include "vector"

#include "nlohmann/json.hpp"

using json = nlohmann::json;

class MapBan {
private:
    int id;
    std::string method;
    std::vector<std::vector<float>> params;
public:
    MapBan() {}

    virtual ~MapBan() {

    }

    MapBan(int id, const std::string &method, const std::vector<std::vector<float>> &params) : id(id), method(method),
                                                                                               params(params) {}

    friend void to_json(json &j, const MapBan &b) {
        j = json{
                {"id",     b.id},
                {"method", b.method},
                {"params", b.params},
        };
    }

    friend void from_json(const json &j, MapBan &b) {
        j.at("id").get_to(b.id);
        j.at("method").get_to(b.method);
        j.at("params").get_to(b.params);
    }
};


#endif //APP_COMMUNICATION_MAPBAN_H
