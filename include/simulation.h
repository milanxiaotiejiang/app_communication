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
    bool update_map;
    bool direct_start_move_base;
    int explorer_mode = 2;

    static bool will() {
        return (rand() % 10) > 5;
    }

};


#endif //APP_COMMUNICATION_SIMULATION_H
