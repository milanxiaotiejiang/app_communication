//
// Created by Looper on 2022/11/27.
//

#ifndef APP_COMMUNICATION_ASYNC_MACHINE_H
#define APP_COMMUNICATION_ASYNC_MACHINE_H

#include "task/status/state_machine.h"
#include "task/subscribe/zoo_inner_status.h"
#include "clean_history/CleanHistoryCenter.h"
#include "simulation.h"
#include "future/node/node_control.h"

class AsyncMachine {
private:
    AsyncMachine() = default;

    AsyncMachine(AsyncMachine &) = delete;

    AsyncMachine &operator=(const AsyncMachine &) = delete;

public:
    ~AsyncMachine() = default;

private:

    loop::manual_epoll epoll_manual = loop::manual_epoll::manual_normal;
    loop::special_epoll epoll_special = loop::special_epoll::special_normal;
    loop::error_epoll epoll_error = loop::error_epoll::error_normal;
    loop::urgency_stop urgency_stop = loop::urgency_stop::trigger_urgency_stop;

    event::flow flow;

    ros::NodeHandle mHandle;
    EnterStatus enterStatus;

    bool inGateMachine;
public:
    static auto &instance() {
        static AsyncMachine obj;
        return obj;
    }

    void initialize(const ros::NodeHandle &handle) {
        mHandle = handle;

        int node_work_mode = 1;
        mHandle.getParam(NODE_CONTROLLER_WORK_MODE, node_work_mode);
        int carto_mode = 0;
        mHandle.getParam(CARTOGRAPHER_WORK_MODE, carto_mode);

        int async_task_flow = 0;
        mHandle.getParam(ASYNC_TASK_FLOW, async_task_flow);

        int async_task_epoll_manual = 0;
        mHandle.getParam(ASYNC_TASK_EPOLL_MANUAL, async_task_epoll_manual);
        int async_task_epoll_special = 0;
        mHandle.getParam(ASYNC_TASK_EPOLL_SPECIAL, async_task_epoll_special);
        int async_task_epoll_error = 0;
        mHandle.getParam(ASYNC_TASK_EPOLL_ERROR, async_task_epoll_error);
        int async_task_urgency_stop = 0;
        mHandle.getParam(ASYNC_TASK_URGENCY_STOP, async_task_urgency_stop);

        enterStatus = EnterStatus(async_task_epoll_manual,
                                  async_task_epoll_special,
                                  async_task_epoll_error,
                                  async_task_urgency_stop,
                                  async_task_flow,
                                  node_work_mode,
                                  carto_mode);
        LOG_IF(INFO, DEBUG_RESTORE) << "enterStatus : " << enterStatus;
    }

    void setEpoll(loop::manual_epoll epoll_manual,
                  loop::special_epoll epoll_special,
                  loop::error_epoll epoll_error,
                  loop::urgency_stop urgency_stop
    ) {
        mHandle.setParam(ASYNC_TASK_EPOLL_MANUAL, static_cast<int>(epoll_manual));
        mHandle.setParam(ASYNC_TASK_EPOLL_SPECIAL, static_cast<int>(epoll_special));
        mHandle.setParam(ASYNC_TASK_EPOLL_ERROR, static_cast<int>(epoll_error));
        mHandle.setParam(ASYNC_TASK_URGENCY_STOP, static_cast<int>(urgency_stop));

        AsyncMachine::epoll_manual = epoll_manual;
        AsyncMachine::epoll_special = epoll_special;
        AsyncMachine::epoll_error = epoll_error;
        AsyncMachine::urgency_stop = urgency_stop;
    }

    void setGateMachine(bool inGateMachine) {
        AsyncMachine::inGateMachine = inGateMachine;
    }

    loop::error_epoll getError() {
        return epoll_error;
    }

    loop::urgency_stop getUrgencyStop() {
        return urgency_stop;
    }

    void setFlow(event::flow flow) {
        mHandle.setParam(ASYNC_TASK_FLOW, static_cast<int>(flow));

        clean_history_db::CleanHistoryCenter::instance().setCurrentFlow(flow);
        AsyncMachine::flow = flow;
    }

    event::flow getFlow() const {
        return flow;
    }

    /**
     * 待机中、充电中、充电完成 waiting_for_task
     *                      arrive_base_station_success
     *                      manual_over_success
     *                      force_over_success
     *                      manual_control_over_success
     * 出站中 out_base_station
     *       switch_node_work_mode
     * 转场中 preliminary_preparation_completed
     *       ensure_move_to_start_point
     *       again_move_to_start_point
     *       again_prepare_cleaning_mechanism
     * 清扫中 cleaning_mechanism_ready
     *       flowing_water_production
     * 回充中 flowing_water_execution_completed
     *       arrive_base_point_success
     *       try_recharging_again
     *       try_move_base_point_again
     *       manual_control_over_and_move_base_point
     *       manual_control_back_try_move_base_point
     *       manual_control_base_point_and_charging
     * 低电量回充中 force_over_and_move_base_point
     *            force_back_try_move_base_point
     *            force_base_point_and_close_mechanism
     *            force_mechanism_close_and_charging
     * 错误中 hardware_interrupt_task
     *       software_interrupt_task
     * 急停中 force_task_pause
     *
     *
     *
     */
    int getMachineCode() {
        if (urgency_stop != loop::urgency_stop::release_urgency_stop) {
            return 10004;
        }
        if (NodeControl::instance().isMap()) {
            return 10015;
        }
        if (inGateMachine) {
            return 10016;
        }
        if (epoll_error == loop::error_epoll::error_manual_clean_start) {
            return 10013;
        }
        if (epoll_error == loop::error_epoll::error_unrecoverable) {
            return 10010;
        }
        if (epoll_manual == loop::manual_epoll::manual_pause) {
            return 10007;
        }
        switch (flow) {
            case event::flow::waiting_for_task:
            case event::flow::arrive_base_station_success:
                if (ZooInnerStatus::instance().getRsoc() >= 95) {
                    return 10012;
                } else if (ZooInnerStatus::instance().getRsoc() > 0) {
                    if (ZooInnerStatus::instance().getIsCharging() == true) {
                        return 10002;
                    } else {
                        return 10006;
                    }
                } else {
                    return 10006;
                }

            case event::flow::out_base_station:
            case event::flow::switch_node_work_mode:
                return 10011;
            case event::flow::preliminary_preparation_completed:
            case event::flow::ensure_move_to_start_point:
                return 10005;
            case event::flow::cleaning_mechanism_ready:
            case event::flow::flowing_water_production:
                return 10001;
            case event::flow::flowing_water_execution_completed:
                return 10014;
            case event::flow::arrive_base_point_success:
            case event::flow::try_recharging_again:
            case event::flow::try_move_base_point_again:
                return 10009;
            case event::flow::hardware_interrupt_task:
            case event::flow::software_interrupt_task:
                return 10010;
        }
    }

    std::string getMachineMessage(int code) {
        switch (code) {
            case 10001:
                return "清洁中";
            case 10002:
                return "充电中";
            case 10004:
                return "急停中";
            case 10005:
                return "转场中";
            case 10006:
                return "待机中";
            case 10007:
                return "暂停中";
            case 10009:
                return "回充中";
            case 10010:
                return "运行异常";
            case 10011:
                return "出站中";
            case 10012:
                return "充电完成";
            case 10013:
                return "手动模式";
            case 10014:
                return "进站中";
            case 10015:
                return "建图中";
            case 10016:
                return "过闸机中";
            default:
                return "未知";
        }
    }

    EnterStatus getEnterStatus() const {
        return enterStatus;
    }
};


#endif //APP_COMMUNICATION_ASYNC_MACHINE_H
