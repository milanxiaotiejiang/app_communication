//
// Created by Looper on 2022/11/26.
//

#ifndef APP_COMMUNICATION_SIMULATION_H
#define APP_COMMUNICATION_SIMULATION_H

#include "glog/logging.h"
#include "string"

#define DEBUG_REQUEST false
#define DEBUG_FIRING true
#define DEBUG_EXPLORATION false
#define DEBUG_EXPLORATION_CACHE false
#define DEBUG_SEGMENTATION false
#define DEBUG_NODE true
#define DEBUG_TASK true
#define DEBUG_TIMER false
#define DEBUG_MAINTENANCE false
#define DEBUG_OTA false
#define DEBUG_DUMP true
#define DEBUG_SENSOR false
#define DEBUG_RESTORE true
#define DEBUG_GATE true
#define DEBUG_CLEAN_MECHANISM false
#define DEBUG_MULTIPLE_MAP true


#define NODE_CONTROLLER_WORK_MODE "/node_controller/work_mode"
#define CARTOGRAPHER_WORK_MODE "/cartographer_work_mode"

#define ASYNC_TASK_FLOW "/async_task/flow"
#define ASYNC_TASK_EPOLL_MANUAL "/async_task/epoll_manual"
#define ASYNC_TASK_EPOLL_SPECIAL "/async_task/epoll_special"
#define ASYNC_TASK_EPOLL_ERROR "/async_task/epoll_error"
#define ASYNC_TASK_URGENCY_STOP "/async_task/urgency_stop"

const bool isTimely = false;
const bool isTestCrash = false;

class Environment {
private:
    Environment() = default;

    Environment(Environment &) = delete;

    Environment &operator=(const Environment &) = delete;

public:
    ~Environment() = default;

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
    bool rectangular_ambulatory_plane = true;
    bool gzip_map = true;

    bool rec_app_node_crash = false;

    std::string nebula_base_url;
    std::string nebula_account;
    std::string nebula_secret;
    std::string device_name{"1"};
    std::string glog_info_time_pid;

    int inu_firing_launch_interval{20};
    int inu_launch_middle_interval{2};
    int inu_final_confirmation_interval{10};
    int inu_start_last_stop_server_interval{10};

    bool no_station_mapping_mode{false};

    static bool will() {
        return (rand() % 10) > 5;
    }

};


#endif //APP_COMMUNICATION_SIMULATION_H
