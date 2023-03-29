//
// Created by Looper on 2023/3/16.
//

#ifndef APP_COMMUNICATION_TWIST_H
#define APP_COMMUNICATION_TWIST_H

#include "nlohmann/json.hpp"

using json = nlohmann::json;

class MyLinear {
public:
    float x;
    float y;
    float z;

    friend void to_json(json &j, const MyLinear &linear) {
        j = json{
                {"x", linear.x},
                {"y", linear.y},
                {"z", linear.z}
        };
    }

    friend void from_json(const json &j, MyLinear &linear) {
        j.at("x").get_to(linear.x);
        j.at("y").get_to(linear.y);
        j.at("z").get_to(linear.z);
    }
};

class MyAngular {
public:
    float x;
    float y;
    float z;

    friend void to_json(json &j, const MyAngular &angular) {
        j = json{
                {"x", angular.x},
                {"y", angular.y},
                {"z", angular.z}
        };
    }

    friend void from_json(const json &j, MyAngular &angular) {
        j.at("x").get_to(angular.x);
        j.at("y").get_to(angular.y);
        j.at("z").get_to(angular.z);
    }
};

class MyTwist {
public:
    MyLinear linear;
    MyAngular angular;

    friend void to_json(json &j, const MyTwist &twist) {
        j = json{
                {"linear",  twist.linear},
                {"angular", twist.angular}
        };
    }

    friend void from_json(const json &j, MyTwist &twist) {
        j.at("linear").get_to(twist.linear);
        j.at("angular").get_to(twist.angular);
    }
};


#endif //APP_COMMUNICATION_TWIST_H
