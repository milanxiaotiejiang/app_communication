//
// Created by Looper on 2022/11/26.
//

#ifndef APP_COMMUNICATION_SIMULATION_H
#define APP_COMMUNICATION_SIMULATION_H

#include "string"

const bool isTimely = false;
const bool isTestCrash = false;

class Environment {

public:
    static auto &instance() {
        static Environment obj;
        return obj;
    }

    bool isRealEnvironment = true;
    std::string room_coverage_uuid = "";

};


#endif //APP_COMMUNICATION_SIMULATION_H
