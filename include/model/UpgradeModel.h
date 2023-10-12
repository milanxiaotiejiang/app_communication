//
// Created by mi on 2022/8/1.
//

#ifndef APP_COMMUNICATION_UPGRADEMODEL_H
#define APP_COMMUNICATION_UPGRADEMODEL_H

#include <nlohmann/json.hpp>
#include "model/Point.h"

class WorkStatusUpgrade {
private:
    int sweep_status;
    int drag_status;
    int absorb_status;
    int push_status;
    int aromatherapy_status;
public:
    WorkStatusUpgrade();

    WorkStatusUpgrade(int sweepStatus, int dragStatus, int absorbStatus, int pushStatus, int aromStatus);

    int getSweepStatus() const;

    void setSweepStatus(int sweepStatus);

    int getDragStatus() const;

    void setDragStatus(int dragStatus);

    int getAbsorbStatus() const;

    void setAbsorbStatus(int absorbStatus);

    int getPushStatus() const;

    void setPushStatus(int pushStatus);

    int getAromatherapyStatus();

    void setAromatherapyStatus(int st);

    friend void to_json(json &j, const WorkStatusUpgrade &b) {
        j = json{
                {"sweep_status",        b.sweep_status},
                {"drag_status",         b.drag_status},
                {"absorb_status",       b.absorb_status},
                {"push_status",         b.push_status},
                {"aromatherapy_status", b.aromatherapy_status}
        };
    }

    friend void from_json(const json &j, WorkStatusUpgrade &b) {
        j.at("sweep_status").get_to(b.sweep_status);
        j.at("drag_status").get_to(b.drag_status);
        j.at("absorb_status").get_to(b.absorb_status);
        j.at("push_status").get_to(b.push_status);
        j.at("aromatherapy_status").get_to(b.aromatherapy_status);
    }
};


class CleanHistoryUpgrade {
private:
    bool is_complete;
    long launch_time;
    long execute_time;
    long end_time;
    int task_mode;
    std::string task_id;
    int clean_area;
    int clean_time;
    int error_code;
    std::string error_code2;
    std::string error_message;

    // Ewen change begin
    bool m_base_complete;
    int task_type;
    std::vector<std::string> component_id_list_;
    int skip_path_point_count_;
    std::vector<int> oper_event;
    int rate_;
    int sweep_status_;
    int mop_status_;
    int vacuum_status_;
    int push_status_;
    int aromatherapy_status_;
    int disinfect_status_;

    int current_step_;
    int current_frquency_;
    int total_step_;
    int total_frequency_;
    int history_state_;
    int current_flow_;
    int urgency_stop_;
    int pause_num_;
    int manual_back_;
    int low_power_back_;
    int force_back_;

    int out_station_;
    int end_sleep_;
    int back_base_retries_;
    int back_base_point_arrived_;
    int station_arrived_;
    int recharge_retries_;
    int close_mechanism_;
    int open_mechanism_;

    bool renew;
    std::string old_task_id;
    long new_task_id;

public:
    CleanHistoryUpgrade();

    CleanHistoryUpgrade(bool isComplete, long launchTime, long executeTime, long endTime, int taskMode,
                        const std::string &taskId, int cleanArea, int cleanTime,
                        int errorCode, const std::string errorCode2, const std::string &errorMessage,
                        bool mBaseComplete,
                        int taskType,
                        const std::vector<std::string> &componentIdList, int skipPathPointCount,
                        const std::vector<int> &operEvent, int rate, int sweepStatus, int mopStatus, int vacuumStatus,
                        int pushStatus, int aromatherapyStatus, int disinfectStatus, int currentStep,
                        int currentFrquency, int totalStep, int totalFrequency, int historyState, int currentFlow,
                        int urgencyStop, int pauseNum, int manualBack, int lowPowerBack, int forceBack, int outStation,
                        int endSleep, int backBaseRetries, int backBasePointArrived, int stationArrived,
                        int rechargeRetries, int closeMechanism, int openMechanism,
                        bool renew, const std::string &old_task_id, long new_task_id);

    bool isBaseComplete() const;

    int getTaskType() const;

    std::vector<int> getOperEvent() const;

    // Ewen change end
    bool isComplete() const;

    void setIsComplete(bool isComplete);

    long getLaunchTime() const;

    void setLaunchTime(long launchTime);

    long getExecuteTime() const;

    void setExecuteTime(long executeTime);

    long getEndTime() const;

    void setEndTime(long endTime);

    int getTaskMode() const;

    void setTaskMode(int taskMode);

    const std::string &getTaskId() const;

    void setTaskId(const std::string &taskId);

    int getCleanArea() const;

    void setCleanArea(int cleanArea);

    int getCleanTime() const;

    void setCleanTime(int cleanTime);

    int getErrorCode() const;

    void setErrorCode(int errorCode);

    const std::string &getErrorMessage() const;

    void setErrorMessage(const std::string &errorMessage);

    friend void to_json(json &j, const CleanHistoryUpgrade &b) {
        j = json{
                {"is_complete",             b.is_complete},
                {"launch_time",             b.launch_time},
                {"execute_time",            b.execute_time},
                {"end_time",                b.end_time},
                {"task_mode",               b.task_mode},
                {"task_id",                 b.task_id},
                {"clean_area",              b.clean_area},
                {"clean_time",              b.clean_time},
                {"error_code",              b.error_code},
                {"error_code2",             b.error_code2},
                {"error_message",           b.error_message},

                {"base_complete",           b.m_base_complete},
                {"task_type",               b.task_type},
                {"oper_event",              b.oper_event},
                {"component_id_list",       b.component_id_list_},
                {"skip_path_point_count",   b.skip_path_point_count_},
                {"rate",                    b.rate_},
                {"sweep_status",            b.sweep_status_},
                {"mop_status",              b.mop_status_},
                {"vacuum_status",           b.vacuum_status_},
                {"push_status",             b.push_status_},
                {"aromatherapy_status",     b.aromatherapy_status_},
                {"disinfect_status",        b.disinfect_status_},

                {"current_step",            b.current_step_},
                {"current_frquency",        b.current_frquency_},
                {"total_step",              b.total_step_},
                {"total_frequency",         b.total_frequency_},
                {"history_state",           b.history_state_},
                {"current_flow",            b.current_flow_},
                {"urgency_stop",            b.urgency_stop_},
                {"pause_num",               b.pause_num_},
                {"manual_back",             b.manual_back_},
                {"low_power_back",          b.low_power_back_},
                {"force_back",              b.force_back_},

                {"out_station",             b.out_station_},
                {"end_sleep",               b.end_sleep_},
                {"back_base_retries",       b.back_base_retries_},
                {"back_base_point_arrived", b.back_base_point_arrived_},
                {"station_arrived",         b.station_arrived_},
                {"recharge_retries",        b.recharge_retries_},
                {"close_mechanism",         b.close_mechanism_},
                {"open_mechanism",          b.open_mechanism_},

                {"renew",                   b.renew},
                {"old_task_id",             b.old_task_id},
                {"new_task_id",             b.new_task_id}
        };
    }

    friend void from_json(const json &j, CleanHistoryUpgrade &b) {
        j.at("is_complete").get_to(b.is_complete);
        j.at("launch_time").get_to(b.launch_time);
        j.at("execute_time").get_to(b.execute_time);
        j.at("end_time").get_to(b.end_time);
        j.at("task_mode").get_to(b.task_mode);
        j.at("task_id").get_to(b.task_id);
        j.at("clean_area").get_to(b.clean_area);
        j.at("clean_time").get_to(b.clean_time);
        j.at("error_code").get_to(b.error_code);
        j.at("error_code2").get_to(b.error_code2);
        j.at("error_message").get_to(b.error_message);

        j.at("base_complete").get_to(b.m_base_complete);
        j.at("task_type").get_to(b.task_type);
        j.at("oper_event").get_to(b.oper_event);
        j.at("component_id_list").get_to(b.component_id_list_);
        j.at("skip_path_point_count").get_to(b.skip_path_point_count_);
        j.at("rate").get_to(b.rate_);
        j.at("sweep_status").get_to(b.sweep_status_);
        j.at("mop_status").get_to(b.mop_status_);
        j.at("vacuum_status").get_to(b.vacuum_status_);
        j.at("push_status").get_to(b.push_status_);
        j.at("aromatherapy_status").get_to(b.aromatherapy_status_);
        j.at("disinfect_status").get_to(b.disinfect_status_);

        j.at("current_step").get_to(b.current_step_);
        j.at("current_frquency").get_to(b.current_frquency_);
        j.at("total_step").get_to(b.total_step_);
        j.at("total_frequency").get_to(b.total_frequency_);
        j.at("history_state").get_to(b.history_state_);
        j.at("current_flow").get_to(b.current_flow_);
        j.at("urgency_stop").get_to(b.urgency_stop_);
        j.at("pause_num").get_to(b.pause_num_);
        j.at("manual_back").get_to(b.manual_back_);
        j.at("low_power_back").get_to(b.low_power_back_);
        j.at("force_back").get_to(b.force_back_);

        j.at("out_station").get_to(b.out_station_);
        j.at("end_sleep").get_to(b.end_sleep_);
        j.at("back_base_retries").get_to(b.back_base_retries_);
        j.at("back_base_point_arrived").get_to(b.back_base_point_arrived_);
        j.at("station_arrived").get_to(b.station_arrived_);
        j.at("recharge_retries").get_to(b.recharge_retries_);
        j.at("close_mechanism").get_to(b.close_mechanism_);
        j.at("open_mechanism").get_to(b.open_mechanism_);

        j.at("renew").get_to(b.renew);
        j.at("old_task_id").get_to(b.old_task_id);
        j.at("new_task_id").get_to(b.new_task_id);
    }
};

#endif //APP_COMMUNICATION_UPGRADEMODEL_H
