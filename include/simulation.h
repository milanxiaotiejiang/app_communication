//
// Created by Looper on 2022/11/26.
//

#ifndef APP_COMMUNICATION_SIMULATION_H
#define APP_COMMUNICATION_SIMULATION_H

#include "glog/logging.h"
#include "string"

#define DEBUG_FIRING false
#define DEBUG_EXPLORATION false
#define DEBUG_SEGMENTATION false
#define DEBUG_TASK false

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
    int dry_accumulation = 0;
    int complex_path_num_splits = 0;

    static bool will() {
        return (rand() % 10) > 5;
    }

};


#endif //APP_COMMUNICATION_SIMULATION_H
