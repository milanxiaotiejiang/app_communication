/**
 * @file GetCleanHistoryStrategy.cpp
 * @author Quan Li (1204018828@qq.com)
 * @brief 获得清扫历史
 * @version 0.1
 * @date 2022-02-21
 * 
 * @copyright Copyright (c) 2022
 * 
 */


#include "sub/json/GetCleanHistoryStrategy.h"

vector<CleanHistory> GetCleanHistoryStrategy::handler(string params) {

    vector<clean_history_db::CleanHistory> clean_histories = clean_history_db::CleanHistoryCenter::instance().getAllCleanHistories();

    vector<CleanHistory> result;
    for (auto &item: clean_histories) {
        WorkStatus work_status(item.sweep_status_, item.mop_status_, item.vacuum_status_, item.push_status_,
                               item.aromatherapy_status_, item.disinfect_status_);
        bool is_complete = (item.history_state_ == clean_history_db::history_state::done);
        bool base_complete = (item.station_arrived_ > 0);
        long launch_time = item.launch_time_;
        long execute_time = item.launch_time_;
        long end_time = item.end_time_;
        if (end_time < 0) {
            end_time = execute_time;
        }
        int task_mode = item.mode_;
        std::string task_id = item.task_id_;
        int clean_area = (int) item.clean_area_;
        int clean_time = item.clean_time_;
        int error_code = item.error_code_;
        std::string error_msg = item.error_msg_;
        int task_type = item.launch_people_ == "admin1" ? 2 : 1;
        CleanHistory clean_history_vo(is_complete, base_complete, launch_time, execute_time, end_time, task_mode,
                                      task_id, work_status, clean_area, clean_time, error_code, error_msg, task_type);
        result.push_back(clean_history_vo);
    }
    if (result.size() > 0) {
        return result;
    } else {
        return std::vector<CleanHistory>();
    }
}

vector<CleanHistoryUpgrade> GetCloudCleanHistoryStrategy::handler(string params) {
    vector<clean_history_db::CleanHistory> clean_histories = clean_history_db::CleanHistoryCenter::instance().getAllCleanHistories();
    vector<CleanHistoryUpgrade> result;
    for (auto &item: clean_histories) {
        bool is_complete = (item.history_state_ == clean_history_db::history_state::done);
        bool base_complete = (item.station_arrived_ > 0);
        long launch_time = item.launch_time_;
        long execute_time = item.launch_time_;
        long end_time = item.end_time_;
        if (end_time < 0) {
            end_time = execute_time;
        }
        int task_mode = item.mode_;
        std::string task_id = item.task_id_;
        int clean_area = (int) item.clean_area_;
        int clean_time = item.clean_time_;
        int error_code = item.error_code_;
        std::string error_code2 = item.error_code2_;
        std::string error_msg = item.error_msg_;
        int task_type = item.launch_people_ == "admin1" ? 2 : 1;
        vector<char> oper_event_char = item.oper_event_;
        vector<int> oper_event_int;
        for (auto &item: oper_event_char) {
            oper_event_int.push_back((int) item);
        }
        std::vector<std::string> component_id_list({});
        int skip_path_point_count = 0;
        int rate = item.rate_;

        int sweep_status = item.sweep_status_;
        int mop_status = item.mop_status_;
        int vacuum_status = item.vacuum_status_;
        int push_status = item.push_status_;
        int aromatherapy_status = item.aromatherapy_status_;
        int disinfect_status = item.disinfect_status_;

        int current_step = item.current_step_;
        int current_frequency = item.current_frequency_;
        int total_step = item.total_step_;
        int total_frequency = item.total_frequency_;
        int history_state = item.history_state_;
        int current_flow = item.current_flow_;
        int urgency_stop = item.urgency_stop_;
        int pause_num = item.pause_num_;
        int manual_back = item.manual_back_;
        int low_power_back = item.low_power_back_;
        int force_back = item.force_back_;
        int out_station = item.out_station_;
        int end_sleep = item.end_sleep_;
        int back_base_retries = item.back_base_retries_;
        int back_base_point_arrived = item.back_base_point_arrived_;
        int station_arrived = item.station_arrived_;
        int recharge_retries = item.recharge_retries_;
        int close_mechanism = item.close_mechanism_;
        int open_mechanism = item.open_mechanism_;

        CleanHistoryUpgrade clean_history_vo(is_complete, launch_time, execute_time, end_time, task_mode, task_id,
                                             clean_area, clean_time, error_code, error_code2, error_msg, base_complete,
                                             task_type, component_id_list, skip_path_point_count, oper_event_int, rate,
                                             sweep_status, mop_status, vacuum_status, push_status,
                                             aromatherapy_status, disinfect_status, current_step, current_frequency,
                                             total_step, total_frequency, history_state, current_flow, urgency_stop,
                                             pause_num, manual_back, low_power_back, force_back, out_station, end_sleep,
                                             back_base_retries, back_base_point_arrived, station_arrived,
                                             recharge_retries, close_mechanism, open_mechanism);

        result.push_back(clean_history_vo);
    }
    if (result.size() > 0)//文件不为空
    {
        return result;
    } else {
        return std::vector<CleanHistoryUpgrade>();
    }
}
