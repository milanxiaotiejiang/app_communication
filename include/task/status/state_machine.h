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
        cleaning_mechanism_ready,//清洁机构已就绪
        ensure_move_to_start_point,//保证移动到第一个点
        flowing_water_production,//流水作业（轮训点位）
        arrive_base_point_success,//成功到达基站点位
        flowing_water_execution_completed,//流水作业已完成
        arrive_base_station_success,//成功回到基站，并且充电
        try_recharging_again,//回充失败时，再次尝试回充
        try_move_base_point_again,//返回基站点位失败后重试

        hardware_interrupt_task,//清洁机构出错导致，一般直接返回基站，但任务是否继续也可控制
        software_interrupt_task,//程序出现严重错误，不可恢复

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

namespace loop {

    enum execute_handle {
        handle_manual,
        handle_special,
        handle_error,
        handle_stop,
        handle_task,
        handle_point,
        handle_unknown
    };

    enum manual_epoll {
        manual_normal,
        manual_resume,//手动继续
        manual_pause,//手动暂停
        manual_back,//手动返回
        manual_force_back,//手动强制返回基站
        manual_task_over,//手动下达新任务
        manual_unknown,
    };

    enum special_epoll {
        special_normal,
        special_low_battery,//低电量回充
        special_branch_water,//清水箱空回充
        special_sewage_water,//污水箱满回充
        special_branch_sewage_water,//清水箱污水箱满回充
        special_dust_push_anomaly,//尘推滚异常返回基站
        special_unknown,
    };

    enum error_epoll {
        error_normal,
        error_manual_clean_start,//手动模式开启
        error_manual_clean_end,//手动模式结束
        error_unrecoverable,//不可恢复的错误（需要拆分）
        error_unknown
    };

    enum urgency_stop {
        trigger_urgency_stop,
        recovery_urgency_stop,
        release_urgency_stop,
    };


}


#endif //APP_COMMUNICATION_STATE_MACHINE_H
