//
// Created by lijiang on 2021/12/17.
//

#ifndef APP_COMMUNICATION_METHODMAP_H
#define APP_COMMUNICATION_METHODMAP_H

#include "string"
#include "MapInfo.h"

class MethodMap {

private:
    int id;
    std::string method;
    MapInfo params;

public:
    MethodMap() {}

    virtual ~MethodMap() {
    }

    MethodMap(int id, const std::string &method, const MapInfo &params) : id(id), method(method), params(params) {}

    friend void to_json(json &j, const MethodMap &b) {
        j = json{
                {"id",     b.id},
                {"method", b.method},
                {"params", b.params},
        };
    }

    friend void from_json(const json &j, MethodMap &b) {
        j.at("id").get_to(b.id);
        j.at("method").get_to(b.method);
        j.at("params").get_to(b.params);
    }
};


#endif //APP_COMMUNICATION_METHODMAP_H
