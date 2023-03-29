//
// Created by Looper on 2022/12/8.
//

#ifndef APP_COMMUNICATION_CLEANHISTORYVO_H
#define APP_COMMUNICATION_CLEANHISTORYVO_H

#include <string>
#include "clean_history_model.h"

namespace clean_history_db {
    class CleanHistoryVo {
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
        int clean_area_{0};//清扫面积
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
        int force_back_{0};//强制返回基站
        int out_station_{WAITING};//出站状态 0未执行到，1正常，-1异常
        int end_sleep_{WAITING};//结束睡眠模式是否成功
        int back_base_retries_{0};//返回基站重试次数
        int back_base_point_arrived_{WAITING};//是否成功回到摆渡点
        int station_arrived_{WAITING};//是否成功回到基站
        int recharge_retries_{0};//回充重复次数
        int close_mechanism_{WAITING};//关闭清洁机构是否成功
        int open_mechanism_{WAITING};//打开清洁机构是否成功

        //错误信息
        int error_code_{0};//错误代码
        std::string error_msg_{""};//错误信息
    public:
        CleanHistoryVo(const std::string &taskId, int mode, int rate, int sweepStatus, int mopStatus, int vacuumStatus,
                       int pushStatus, int aromatherapyStatus, int disinfectStatus, const std::string &launchPeople,
                       const std::string &timeMode, long launchTime, long executeTime, long cleanTime, long endTime,
                       int cleanArea, int currentStep, int currentFrequency, int totalStep, int totalFrequency,
                       int historyState, int currentFlow, int urgencyStop, int pauseNum, int manualBack,
                       int lowPowerBack, int forceBack, int outStation, int endSleep, int backBaseRetries,
                       int backBasePointArrived, int stationArrived, int rechargeRetries, int closeMechanism,
                       int openMechanism, int errorCode, const std::string &errorMsg);

        friend void to_json(json &j, const CleanHistory &b) {
            j = json{
                    {"task_id",                 b.task_id_},
                    {"mode",                    b.mode_},
                    {"rate",                    b.rate_},
                    {"sweep_status",            b.sweep_status_},
                    {"mop_status",              b.mop_status_},
                    {"vacuum_status",           b.vacuum_status_},
                    {"push_status",             b.push_status_},
                    {"aromatherapy_status",     b.aromatherapy_status_},
                    {"disinfect_status",        b.disinfect_status_},
                    {"launch_people",           b.launch_people_},
                    {"time_mode",               b.time_mode_},
                    {"launch_time",             b.launch_time_},

                    {"execute_time",            b.execute_time_},
                    {"clean_time",              b.clean_time_},
                    {"end_time",                b.end_time_},
                    {"clean_area",              b.clean_area_},
                    {"current_step",            b.current_step_},
                    {"current_frequency",       b.current_frequency_},
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

                    {"error_code",              b.error_code_},
                    {"error_msg",               b.error_msg_}
            };
        }

        friend void from_json(const json &j, CleanHistory &b) {
            j.at("task_id").get_to(b.task_id_);
            j.at("mode").get_to(b.mode_);
            j.at("rate").get_to(b.rate_);
            j.at("sweep_status").get_to(b.sweep_status_);
            j.at("mop_status").get_to(b.mop_status_);
            j.at("vacuum_status").get_to(b.vacuum_status_);
            j.at("push_status").get_to(b.push_status_);
            j.at("aromatherapy_status").get_to(b.aromatherapy_status_);
            j.at("disinfect_status").get_to(b.disinfect_status_);
            j.at("launch_people").get_to(b.launch_people_);
            j.at("time_mode").get_to(b.time_mode_);
            j.at("launch_time").get_to(b.launch_time_);

            j.at("execute_time").get_to(b.execute_time_);
            j.at("clean_time").get_to(b.clean_time_);
            j.at("end_time").get_to(b.end_time_);
            j.at("clean_area").get_to(b.clean_area_);
            j.at("current_step").get_to(b.current_step_);
            j.at("current_frequency").get_to(b.current_frequency_);
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

            j.at("error_code").get_to(b.error_code_);
            j.at("error_msg").get_to(b.error_msg_);


        }
    };

}
#endif //APP_COMMUNICATION_CLEANHISTORYVO_H
