//
// Created by Looper on 2022/11/9.
//

#ifndef APP_COMMUNICATION_STATE_MACHINE_H
#define APP_COMMUNICATION_STATE_MACHINE_H

#include "unordered_map"

namespace event {

    enum flow {
        waiting_for_task = 0,
        out_base_station,//出站
        switch_node_work_mode,//切换工作模式
        preliminary_preparation_completed,//前期准备工作完成

        ensure_move_to_start_point,//保证移动到第一个点
        cleaning_mechanism_ready,//清洁机构已就绪
        flowing_water_production,//流水作业（轮训点位）
        again_move_to_start_point,//再次移动到第一个点位
        again_prepare_cleaning_mechanism,//准备开启清洁机构
        flowing_water_execution_completed,//流水作业已完成
        arrive_base_point_success,//成功到达基站点位
        try_recharging_again,//回充失败时，再次尝试回充
        try_move_base_point_again,//返回基站点位失败后重试

        arrive_base_station_success,//成功回到基站，并且充电

        manual_over_and_move_base_point,//手动返回基站点位
        manual_back_try_move_base_point,//返回基站点位有误差，重试
        manual_base_point_and_close_mechanism,//返回基站成功，准备收起清洁机构
        manual_mechanism_close_and_charging,//收起完清洁机构，准备回充
        manual_over_success,//手动结束成功
        manual_task_pause,//手动暂停中
        manual_cleaning,//手动模式

        force_over_and_move_base_point,//强制返回基站点位，用于电量到达一定数值后，程序强制切换
        force_back_try_move_base_point,//强制返回基站点位有误差，重试
        force_base_point_and_close_mechanism,//强制返回基站成功，准备收起清洁机构
        force_mechanism_close_and_charging,//强制返回后收起完清洁机构，准备回充
        force_over_success,//强制结束成功
        force_task_pause,//急停中

        manual_control_over_and_move_base_point,//手动控制强制返回基站点位，一般用于开机后不在基站位置
        manual_control_back_try_move_base_point,//手动控制返回基站点位有误差，重试
        manual_control_base_point_and_charging,//手动控制返回基站过程中，已经到达基站点，准备充电
        manual_control_over_success,//手动控制返回基站结束成功

        hardware_interrupt_task,//清洁机构出错导致，一般直接返回基站，但任务是否继续也可控制
        software_interrupt_task//程序出现严重错误，不可恢复
    };

    enum status {
        AUTO_STATE,
        MANUAL_STATE,
        FORCE_STATE
    };

    enum error {
        SUCCEEDED,
        PENDING,
        ACTIVE,
        RECALLED,
        REJECTED,
        PREEMPTED,
        ABORTED,
        LOST,
        TIMEOUT
    };
}


#endif //APP_COMMUNICATION_STATE_MACHINE_H
