//
// Created by Looper on 2023/2/24.
//

#ifndef APP_COMMUNICATION_CLOUD_ROBOT_CONTROL_H
#define APP_COMMUNICATION_CLOUD_ROBOT_CONTROL_H

#include <string>

class CloudRobotControl {
private:
    std::string staging_information;

public:
    static auto &instance() {
        static CloudRobotControl obj;
        return obj;
    }

    void saveInfo(std::string message) {
        staging_information = message;
    }

    std::string useInfo() {
        return staging_information;
    }

    void reset() {
        staging_information = "{}";
    }
};

#endif //APP_COMMUNICATION_CLOUD_ROBOT_CONTROL_H
