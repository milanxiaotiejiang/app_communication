//
// Created by Looper on 2022/11/9.
//

#ifndef APP_COMMUNICATION_STATE_MACHINE_H
#define APP_COMMUNICATION_STATE_MACHINE_H

#include <ostream>
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
        TIMEOUT,
        CRASH
    };

    enum GG {
        gg_normal_flow,
        gg_task_over,
        gg_manual_mode,
        gg_urgency_stop
    };

    enum SB {
        sb_software,
        sb_lift,
        sb_unrecoverable,
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
        error_lift,//电梯
        error_unrecoverable,//不可恢复的错误（需要拆分）当前暂时为激光雷达错误
        error_unknown
    };

    enum urgency_stop {
        trigger_urgency_stop,
        recovery_urgency_stop,
        release_urgency_stop,
    };


}

struct EnterStatus {

    loop::manual_epoll epoll_manual = loop::manual_epoll::manual_normal;
    loop::special_epoll epoll_special = loop::special_epoll::special_normal;
    loop::error_epoll epoll_error = loop::error_epoll::error_normal;
    loop::urgency_stop urgency_stop = loop::urgency_stop::trigger_urgency_stop;
    event::flow task_flow = event::flow::waiting_for_task;
    int node_mode = 0;
    int carto_mode = 0;

    EnterStatus() = default;

    EnterStatus(int manual, int special, int error, int stop, int flow, int node_mode, int carto_mode) :
            node_mode(node_mode), carto_mode(carto_mode) {
        epoll_manual = loop::manual_epoll(manual);
        epoll_special = loop::special_epoll(special);
        epoll_error = loop::error_epoll(error);
        urgency_stop = loop::urgency_stop(stop);
        task_flow = event::flow(flow);

    }

    friend std::ostream &operator<<(std::ostream &os, const EnterStatus &status) {
        os << "epoll_manual: " << status.epoll_manual << " epoll_special: " << status.epoll_special << " epoll_error: "
           << status.epoll_error << " urgency_stop: " << status.urgency_stop << " task_flow: " << status.task_flow
           << " node_mode: " << status.node_mode << " carto_mode: " << status.carto_mode;
        return os;
    }
};

#endif //APP_COMMUNICATION_STATE_MACHINE_H
