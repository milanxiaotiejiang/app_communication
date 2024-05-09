/**
 * @file clean_history_model.cpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2022-12-01
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "clean_history/clean_history_model.h"

namespace clean_history_db {


    CleanHistory::CleanHistory(const std::string &taskId, int mode, int rate, const std::string &launchPeople,
                               const std::string &timeMode, long launchTime,
                               bool renew, const std::string &old_task_id, long new_task_id,
                               const std::string &o_map_id) : task_id_(taskId),
                                                              mode_(mode),
                                                              rate_(rate),
                                                              launch_people_(launchPeople),
                                                              time_mode_(timeMode),
                                                              launch_time_(launchTime),
                                                              renew(renew),
                                                              old_task_id(old_task_id),
                                                              new_task_id(new_task_id),
                                                              o_map_id(o_map_id) {}

    CleanHistory::CleanHistory(const std::string &taskId, int mode, int rate, int sweepStatus, int mopStatus,
                               int vacuumStatus, int pushStatus, int aromatherapyStatus, int disinfectStatus,
                               const std::string &launchPeople, const std::string &timeMode, long launchTime,
                               long executeTime, long cleanTime, long endTime, double cleanArea, int currentStep,
                               int currentFrequency, int totalStep, int totalFrequency, int historyState,
                               int currentFlow, int urgencyStop, int pauseNum, int manualBack, int lowPowerBack,
                               int forceBack, int outStation, int endSleep, int backBaseRetries,
                               int backBasePointArrived, int stationArrived, int rechargeRetries, int closeMechanism,
                               int openMechanism, int errorCode, const std::string &errorMsg, bool renew,
                               const std::string &old_task_id, long new_task_id, const std::string &o_map_id) :
            task_id_(taskId),
            mode_(mode),
            rate_(rate),
            sweep_status_(sweepStatus),
            mop_status_(mopStatus),
            vacuum_status_(vacuumStatus),
            push_status_(pushStatus),
            aromatherapy_status_(aromatherapyStatus),
            disinfect_status_(disinfectStatus),
            launch_people_(launchPeople),
            time_mode_(timeMode),
            launch_time_(launchTime),
            execute_time_(executeTime),
            clean_time_(cleanTime),
            end_time_(endTime),
            clean_area_(cleanArea),
            current_step_(currentStep),
            current_frequency_(currentFrequency),
            total_step_(totalStep),
            total_frequency_(totalFrequency),
            history_state_(historyState),
            current_flow_(currentFlow),
            urgency_stop_(urgencyStop),
            pause_num_(pauseNum),
            manual_back_(manualBack),
            low_power_back_(lowPowerBack),
            force_back_(forceBack),
            out_station_(outStation),
            end_sleep_(endSleep),
            back_base_retries_(backBaseRetries),
            back_base_point_arrived_(backBasePointArrived),
            station_arrived_(stationArrived),
            recharge_retries_(rechargeRetries),
            close_mechanism_(closeMechanism),
            open_mechanism_(openMechanism),
            error_code_(errorCode),
            error_msg_(errorMsg),
            renew(renew),
            old_task_id(old_task_id),
            new_task_id(new_task_id),
            o_map_id(old_task_id) {}
}

