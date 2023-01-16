//
// Created by Quan.Li on 2022/12/1.
//

#ifndef APP_COMMUNICATION_CLEANHISTORY_MODEL_H
#define APP_COMMUNICATION_CLEANHISTORY_MODEL_H

#include "model/WorkStatus.h"
#include "task/status/state_machine.h"
#include "string"
#include <ostream>
#include <vector>

namespace clean_history_db {

    enum history_state {
        waiting = 0,//等待更新
        executing = 1,//正在更新
        done = 2,//更新完成
        error = 3//更新完成但任务有错误
    };

    const int WAITING = 0;
    const int SUCCEED = 1;
    const int FAIL = -1;

    class CleanHistory {//回复清洁历史
    public:
        //任务基本信息
        std::string task_id_;
        int mode_;
        int rate_;
        int sweep_status_{-1};
        int mop_status_{-1};
        int vacuum_status_{-1};
        int push_status_{-1};
        int aromatherapy_status_{-1};
        int disinfect_status_{-1};
        std::string launch_people_;
        std::string time_mode_;
        long launch_time_;
        //任务执行情况相关
        long execute_time_{-1};//开始执行时间
        long clean_time_{0};//清扫用时
        long end_time_{-1}; //执行结束时间
        double clean_area_{0.0};//清扫面积
        int current_step_{0};    //当前步数
        int current_frequency_{0};//当前执行次数
        int total_step_{0};       //单次点数
        int total_frequency_{0};//总次数
        int history_state_{history_state::waiting};//更新状态
        //流程跟踪
        int current_flow_{event::flow::waiting_for_task};//当前流程
        int urgency_stop_{0};//急停次数
        int pause_num_{0};//暂停次数
        int manual_back_{0};//手动返回基站
        int low_power_back_{0};//低电量返回基站
        int water_level_back_{0};//水箱返回基站
        int force_back_{0};//强制返回基站
        int out_station_{WAITING};//出站状态 0未执行到，1正常，-1异常
        int end_sleep_{WAITING};//结束睡眠模式是否成功
        int back_base_retries_{0};//返回基站重试次数
        int back_base_point_arrived_{WAITING};//是否成功回到摆渡点
        int station_arrived_{WAITING};//是否成功回到基站
        int recharge_retries_{0};//回充重复次数
        int close_mechanism_{WAITING};//关闭清洁机构是否成功
        int open_mechanism_{WAITING};//打开清洁机构是否成功

        std::vector<char> oper_event_;

        //错误信息
        int error_code_{0};//错误代码
        std::string error_code2_{""};//2代错误码
        std::string error_msg_{""};//错误信息

    public:
        CleanHistory() = default;

        CleanHistory(const std::string &taskId, int mode, int rate, const std::string &launchPeople,
                     const std::string &timeMode, long launchTime);


        CleanHistory(const std::string &taskId, int mode, int rate, int sweepStatus, int mopStatus, int vacuumStatus,
                     int pushStatus, int aromatherapyStatus, int disinfectStatus, const std::string &launchPeople,
                     const std::string &timeMode, long launchTime, long executeTime, long cleanTime, long endTime,
                     double cleanArea, int currentStep, int currentFrequency, int totalStep, int totalFrequency,
                     int historyState, int currentFlow, int urgencyStop, int pauseNum, int manualBack, int lowPowerBack,
                     int forceBack, int outStation, int endSleep, int backBaseRetries, int backBasePointArrived,
                     int stationArrived, int rechargeRetries, int closeMechanism, int openMechanism, int errorCode,
                     const std::string &errorMsg);
    };
}


#endif//APP_COMMUNICATION_CLEANHISTORY_MODEL_H