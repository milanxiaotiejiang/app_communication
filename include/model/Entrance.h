//
// Created by lijiang on 2021/12/17.
//

#ifndef APP_COMMUNICATION_ENTRANCE_H
#define APP_COMMUNICATION_ENTRANCE_H

#include "string"

#include "nlohmann/json.hpp"

using json = nlohmann::json;

class Entrance {
private:
    std::string method;
    int id;

public:

    Entrance() {}

    virtual ~Entrance() {

    }

    Entrance(const std::string &method, int id) : method(method), id(id) {}

    const std::string &getMethod() const {
        return method;
    }

    void setMethod(const std::string &method) {
        Entrance::method = method;
    }

    int getId() const {
        return id;
    }

    void setId(int id) {
        Entrance::id = id;
    }

    friend void to_json(json &j, const Entrance &r) {
        j = json{{"id",     r.id},
                 {"method", r.method}};
    }

    friend void from_json(const json &j, Entrance &r) {
        j.at("id").get_to(r.id);
        j.at("method").get_to(r.method);

    }
};


#endif //APP_COMMUNICATION_ENTRANCE_H
