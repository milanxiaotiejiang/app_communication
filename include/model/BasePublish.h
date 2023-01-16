//
// Created by mi on 2022/4/27.
//

#ifndef APP_COMMUNICATION_BASEPUBLISH_H
#define APP_COMMUNICATION_BASEPUBLISH_H

#include "string"

#include "nlohmann/json.hpp"

using json = nlohmann::json;

template<class T>
class BasePublish {

private:
    int version;
    std::string explain;
    T params;
public:
    BasePublish(int version, const std::string &explain, T params) : version(version), explain(explain),
                                                                     params(params) {}

    friend void to_json(json &j, const BasePublish<T> &publish) {
        j = json{
                {"version", publish.version},
                {"explain", publish.explain},
                {"params",  publish.params}
        };
    }

    friend void from_json(const json &j, BasePublish<T> &publish) {
        j.at("version").get_to(publish.version);
        j.at("explain").get_to(publish.explain);
        j.at("params").get_to(publish.params);
    }
};

#endif //APP_COMMUNICATION_BASEPUBLISH_H
