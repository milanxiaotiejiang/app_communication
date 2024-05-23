//
// Created by Looper on 2022/11/26.
//

#ifndef APP_COMMUNICATION_SIMULATION_H
#define APP_COMMUNICATION_SIMULATION_H

#include "glog/logging.h"
#include "string"

#define DEBUG_REQUEST true
#define DEBUG_FIRING true
#define DEBUG_EXPLORATION true
#define DEBUG_EXPLORATION_CACHE true
#define DEBUG_SEGMENTATION true
#define DEBUG_NODE true
#define DEBUG_TASK true
#define DEBUG_TIMER true
#define DEBUG_MAINTENANCE true
#define DEBUG_OTA true
#define DEBUG_DUMP true
#define DEBUG_SENSOR true
#define DEBUG_RESTORE true
#define DEBUG_GATE true
#define DEBUG_CLEAN_MECHANISM true
#define DEBUG_MULTIPLE_MAP true
#define DEBUG_ELEVATOR true


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

    bool isRealEnvironmentTest = false;

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

    bool no_station_mapping_mode{true};

    int the_interval_between_two_messages{300};
    int maximum_number_of_retry_attempts_for_errors_to_the_elevator{2};
    int maximum_number_of_retries_for_elevator_logic_errors{4};
    int maximum_number_of_entering_the_elevator{1};
    int the_time_interval_for_continuously_lighting_up_floors{3000};
    int the_time_interval_for_continuous_floor_queries{1000};
    int the_time_interval_for_continuous_floor_determination{1000};
    int maximum_waiting_time_for_elevator{60 * 10 * 1000};
    int maximum_time_for_entering_and_exiting_the_elevator{60 * 2 * 1000};
    double entering_inner_steering_speed{0.4};
    bool serial_port_send_print{false};
    bool serial_port_accept_print{false};
    bool jump_elevator_status_door_state{true};
    int maximum_delay_time{9};


    int pre_circulation_error_retry_count_max{2};
    int post_circulation_error_retry_count_max{2};

    int pre_circulation_error_retry_timeout{30000};
    int post_circulation_error_retry_timeout{30000};

    int outside_from_target_distance{10};

    int pre_elevator_in_error_retry_count_max{2};
    int post_elevator_in_error_retry_count_max{2};

    int pre_elevator_in_error_retry_timeout{30000};
    int post_elevator_in_error_retry_timeout{30000};

    double inner_white_pixel_ratio{0.6};
    double internal_spatial_analysis_count{30};

    static bool will() {
        return (rand() % 10) > 5;
    }

};


#endif //APP_COMMUNICATION_SIMULATION_H
