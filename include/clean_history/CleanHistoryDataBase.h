//
// Created by liquan on 22-12-1.
//

#ifndef APP_COMMUNICATION_CLEANHISTORYDATABASE_H
#define APP_COMMUNICATION_CLEANHISTORYDATABASE_H

#include "clean_history_model.h"
#include "db/path.h"
#include <vector>
#include <ros/package.h>
#include <ros/ros.h>
#include <sqlite_orm/sqlite_orm.h>

namespace clean_history_db {

    const std::string CLEAN_HISTORY_SQLITE_PATH = path::data_base_config_dir() + "cleanHistory.sqlite";

    using namespace sqlite_orm;

    class SqliteDataBase {
    public:
        static auto initStorage() {
            return make_storage(CLEAN_HISTORY_SQLITE_PATH,
                                make_table("clean_history",
                                           make_column("task_id", &CleanHistory::task_id_, primary_key()),
                                           make_column("mode", &CleanHistory::mode_),
                                           make_column("rate", &CleanHistory::rate_),
                                           make_column("sweep_status", &CleanHistory::sweep_status_),
                                           make_column("mop_status", &CleanHistory::mop_status_),
                                           make_column("vacuum_status", &CleanHistory::vacuum_status_),
                                           make_column("push_status", &CleanHistory::push_status_),
                                           make_column("aromatherapy_status", &CleanHistory::aromatherapy_status_),
                                           make_column("disinfect_status", &CleanHistory::disinfect_status_),
                                           make_column("launch_people", &CleanHistory::launch_people_),
                                           make_column("time_mode", &CleanHistory::time_mode_),
                                           make_column("launch_time", &CleanHistory::launch_time_),

                                           make_column("execute_time", &CleanHistory::execute_time_),
                                           make_column("clean_time", &CleanHistory::clean_time_),
                                           make_column("end_time", &CleanHistory::end_time_),
                                           make_column("clean_area", &CleanHistory::clean_area_),
                                           make_column("current_step", &CleanHistory::current_step_),
                                           make_column("current_frequency", &CleanHistory::current_frequency_),
                                           make_column("total_step", &CleanHistory::total_step_),
                                           make_column("total_frequency", &CleanHistory::total_frequency_),
                                           make_column("history_state", &CleanHistory::history_state_),

                                           make_column("current_flow", &CleanHistory::current_flow_),
                                           make_column("urgency_stop", &CleanHistory::urgency_stop_),
                                           make_column("pause_num", &CleanHistory::pause_num_),
                                           make_column("manual_back", &CleanHistory::manual_back_),
                                           make_column("low_power_back", &CleanHistory::low_power_back_),
                                           make_column("water_level_back", &CleanHistory::water_level_back_),
                                           make_column("force_back", &CleanHistory::force_back_),
                                           make_column("out_station", &CleanHistory::out_station_),
                                           make_column("end_sleep", &CleanHistory::end_sleep_),
                                           make_column("back_base_retries", &CleanHistory::back_base_retries_),
                                           make_column("back_base_point_arrived",
                                                       &CleanHistory::back_base_point_arrived_),
                                           make_column("station_arrived", &CleanHistory::station_arrived_),
                                           make_column("recharge_retries", &CleanHistory::recharge_retries_),
                                           make_column("close_mechanism", &CleanHistory::close_mechanism_),
                                           make_column("open_mechanism", &CleanHistory::open_mechanism_),
                                           make_column("oper_event", &CleanHistory::oper_event_),
                                           make_column("error_code", &CleanHistory::error_code_),
                                           make_column("error_code2", &CleanHistory::error_code2_),
                                           make_column("error_msg", &CleanHistory::error_msg_)));
        }
    };

    using Storage = decltype(SqliteDataBase::initStorage());

    class CleanHistoryDataBase {
    private:
        Storage clean_history_storage_ = SqliteDataBase::initStorage();
    public:
        static auto &instance() {
            static CleanHistoryDataBase obj;
            return obj;
        }

        //初始化清洁历史数据库
        bool initalize() {
            clean_history_storage_.sync_schema();
        }

        //添加一条清洁历史
        bool addCleanHistory(CleanHistory clean_history) {
            clean_history_storage_.replace(clean_history);
        }

        //清除所有清洁历史
        bool removeAll() {
            clean_history_storage_.remove_all<CleanHistory>();
        }

        //更新清洁历史
        bool updateHistory(CleanHistory clean_history) {
            clean_history_storage_.replace(clean_history);
        }

        //获取所有清洁历史
        std::vector<CleanHistory> getAll() {
            return clean_history_storage_.get_all<CleanHistory>();
        }

        //获取task_id的清洁历史
        CleanHistory getCleanHistory(std::string task_id) {
            return clean_history_storage_.get_all<CleanHistory>(where(c(&CleanHistory::task_id_) == task_id))[0];
        }

        //获取所有未完成的历史
        std::vector<CleanHistory> getErrorHistory() {
            return clean_history_storage_.get_all<CleanHistory>(
                    where(c(&CleanHistory::history_state_) == 1 or
                          c(&CleanHistory::history_state_) == 0));
        }
    };
} // clean_history_db

#endif //APP_COMMUNICATION_CLEANHISTORYDATABASE_H
