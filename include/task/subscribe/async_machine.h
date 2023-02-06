//
// Created by Looper on 2022/11/27.
//

#ifndef APP_COMMUNICATION_ASYNC_MACHINE_H
#define APP_COMMUNICATION_ASYNC_MACHINE_H

#include "task/status/state_machine.h"
#include "task/subscribe/zoo_inner_status.h"

class AsyncMachine {
private:
    event::flow flow;

public:
    static auto &instance() {
        static AsyncMachine obj;
        return obj;
    }

    void setFlow(event::flow flow) {
        AsyncMachine::flow = flow;
    }

    event::flow getFlow() const {
        return flow;
    }

    /**
     * 10006 待机中
     * 10002 充电中
     * 10012 充电完成
     * 10005 转场中
     * 10011 出站中
     * 10001 清扫中
     * 10009 回充中
     * 10008 低电量回充中
     * 10010 dock_error
     * 10007 暂停中
     * 10003 暂停中
     * 10004 急停中
     *
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
     *       manual_over_and_move_base_point
     *       manual_back_try_move_base_point
     *       manual_base_point_and_close_mechanism
     *       manual_mechanism_close_and_charging
     *       manual_control_over_and_move_base_point
     *       manual_control_back_try_move_base_point
     *       manual_control_base_point_and_charging
     * 低电量回充中 force_over_and_move_base_point
     *            force_back_try_move_base_point
     *            force_base_point_and_close_mechanism
     *            force_mechanism_close_and_charging
     * 错误中 hardware_interrupt_task
     *       software_interrupt_task
     * 暂停中 manual_task_pause
     * 急停中 force_task_pause
     *
     *
     *
     */
    int getMachineCode() {
        switch (flow) {
            case event::flow::waiting_for_task:
            case event::flow::arrive_base_station_success:
            case event::flow::manual_over_success:
            case event::flow::force_over_success:
            case event::flow::manual_control_over_success:
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
            case event::flow::again_move_to_start_point:
            case event::flow::again_prepare_cleaning_mechanism:
                return 10005;
            case event::flow::cleaning_mechanism_ready:
            case event::flow::flowing_water_production:
                return 10001;
            case event::flow::flowing_water_execution_completed:
            case event::flow::arrive_base_point_success:
            case event::flow::try_recharging_again:
            case event::flow::try_move_base_point_again:
            case event::flow::manual_over_and_move_base_point:
            case event::flow::manual_back_try_move_base_point:
            case event::flow::manual_base_point_and_close_mechanism:
            case event::flow::manual_control_over_and_move_base_point:
            case event::flow::manual_control_back_try_move_base_point:
                return 10009;
            case event::flow::force_over_and_move_base_point:
            case event::flow::force_back_try_move_base_point:
            case event::flow::force_base_point_and_close_mechanism:
                if (ZooInnerStatus::instance().getRsoc() <= 10) {
                    return 10008;
                } else {
                    return 10009;
                }
            case event::flow::hardware_interrupt_task:
            case event::flow::software_interrupt_task:
                return 10010;
            case event::flow::manual_task_pause:
                return 10007;
            case event::flow::force_task_pause:
                return 10004;
            case event::flow::manual_cleaning:
                return 10013;
            case event::flow::force_mechanism_close_and_charging:
            case event::flow::manual_control_base_point_and_charging:
            case event::flow::manual_mechanism_close_and_charging:
                return 10014;
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
            case 10008:
                return "低电量回充中";
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
            case 10007:
                return "暂停中";
            case 10014:
                return "进站中";
            default:
                return "未知";
        }
    }
};


#endif //APP_COMMUNICATION_ASYNC_MACHINE_H
