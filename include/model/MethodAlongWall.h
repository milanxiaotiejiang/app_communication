//
// Created by lijiang on 2021/12/17.
//

#ifndef APP_COMMUNICATION_METHODALONGWALL_H
#define APP_COMMUNICATION_METHODALONGWALL_H

#include "string"
#include "vector"

#include "nlohmann/json.hpp"

using json = nlohmann::json;

class MethodAlongWall {
private:
    std::string method;
    std::vector<float> params;
    int id;

public:

    MethodAlongWall() {}

    virtual ~MethodAlongWall() {

    }

    MethodAlongWall(const std::string &method, const std::vector<float> &params, int id) : method(method),
                                                                                           params(params),
                                                                                           id(id) {}

    const std::string &getMethod() const {
        return method;
    }

    void setMethod(const std::string &method) {
        MethodAlongWall::method = method;
    }

    const std::vector<float> &getParams() const {
        return params;
    }

    void setParams(const std::vector<float> &params) {
        MethodAlongWall::params = params;
    }

    int getId() const {
        return id;
    }

    void setId(int id) {
        MethodAlongWall::id = id;
    }

    friend void to_json(json &j, const MethodAlongWall &b) {
        j = json{
                {"method", b.method},
                {"params", b.params},
                {"id",     b.id},
        };
    }

    friend void from_json(const json &j, MethodAlongWall &b) {
        j.at("method").get_to(b.method);
        j.at("params").get_to(b.params);
        j.at("id").get_to(b.id);
    }
};


#endif //APP_COMMUNICATION_METHODALONGWALL_H
