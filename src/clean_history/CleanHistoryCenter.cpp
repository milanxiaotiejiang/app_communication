//
// Created by liquan on 22-12-1.
//

#include "clean_history/CleanHistoryCenter.h"
#include "model/InternalEvent.h"
#include "db/SqliteDataBase.h"
#include "db/property_data_base.h"

namespace clean_history_db {
    bool CleanHistoryCenter::initialize() {
        std::unique_lock<std::mutex> lock(history_update_mutex_);
        CleanHistoryDataBase::instance().initalize();
        vector<CleanHistory> error_histories = CleanHistoryDataBase::instance().getErrorHistory();
        for (auto item: error_histories) {
            //更新结束时间
            //更新错误情况
            if (item.execute_time_ > 0) {
                item.end_time_ = item.execute_time_ + item.clean_time_ * 60 * 1000;
                int last_event = item.current_flow_;
                std::tuple<int, std::string, std::string> error_pair = generateErrorMessageFromLastFlow(last_event);
                item.error_code_ = std::get<0>(error_pair);
                item.error_msg_ = std::get<1>(error_pair);
                item.error_code2_ = std::get<2>(error_pair);
            } else {
                item.error_code_ = 3300;
                item.error_msg_ = "等待时被关机";
                item.error_code2_ = "CCR_300";
            }
            //完成
            item.history_state_ = history_state::error;
            //更新到数据库
            CleanHistoryDataBase::instance().updateHistory(item);
        }
        return true;
    }

    bool CleanHistoryCenter::addCleanHistory(const RealTask &task) {
//        std::cout << "add clean history" << std::endl;
//        std::cout << "task id " << task.getId() << std::endl;
        std::unique_lock<std::mutex> lock(history_update_mutex_);
        long launch_time;//记录时间
        time_t timep;
        time(&timep);
        launch_time = timep * 1000;//毫秒
        //根据当前任务生成一个CleanHistory
        std::string launch_people = task.getLaunchPeople();
        int mode = 0;
        if (task.isRenew()) {
            launch_people = task.getOnSource();
            if (task.getMode() == 0) {
                mode = 7;
            } else if (task.getMode() == 1) {
                mode = 6;
            } else if (task.getMode() == 2) {
                mode = 2;
            } else if (task.getMode() == 3) {
                mode = 3;
            }
        } else {
            mode = task.getMode();
        }
        CleanHistory new_clean_history(task.getId(), mode,
                                       task.getRate(), launch_people,
                                       task.getTimeMode(), launch_time);
        CleanHistoryDataBase::instance().addCleanHistory(new_clean_history);

        return true;
    }

    bool CleanHistoryCenter::launchFailed(const RealTask &task, const app::exception &e) {
        std::unique_lock<std::mutex> lock(history_update_mutex_);
        CleanHistory history = CleanHistoryDataBase::instance().getCleanHistory(task.getId());
        //设置错误码
        std::tuple<int, std::string, std::string> error_pair = generateErrorMessageFromException(e);
        history.error_code_ = std::get<0>(error_pair);//待定
        history.error_msg_ = std::get<1>(error_pair);   //待定
        history.error_code2_ = std::get<2>(error_pair); //待定
        history.history_state_ = history_state::error;

        CleanHistoryDataBase::instance().updateHistory(history);
    }

    std::tuple<int, std::string, std::string>
    CleanHistoryCenter::generateErrorMessageFromException(const app::exception &e) {
        int error_code;
        std::string error_msg = e.what();
        switch (e.code().value()) {
            case error::general:
                return make_tuple(3101, "Generic error", "CCR_101");
            case error::operation_failed:
                return make_tuple(3102, "操作失败", "CCR_102");
            case error::acquisition_file_failed:
                return make_tuple(3103, "文件获取失败", "CCR_103");
            case error::open_file_fail:
                return make_tuple(3104, "无法打开文件", "CCR_104");
            case error::duplicate_path_file_name:
                return make_tuple(3105, "路径文件名重复", "CCR_105");
            case error::add_failed:
                return make_tuple(3106, "添加失败", "CCR_106");
            case error::delete_failed:
                return make_tuple(3107, "删除失败", "CCR_107");
            case error::save_failed:
                return make_tuple(3108, "保存失败", "CCR_108");
            case error::create_file_fail:
                return make_tuple(3109, "创建文件失败", "CCR_109");
            case error::create_file_timer_fail:
                return make_tuple(3110, "创建定时器文件失败", "CCR_110");
            case error::open_file_timer_fail:
                return make_tuple(3111, "打开定时器文件失败", "CCR_111");
            case error::save_file_timer_failed:
                return make_tuple(3112, "保存定时器文件失败", "CCR_112");
            case error::file_acquisition_failed:
                return make_tuple(3113, "该part在组合路径中已被使用", "CCR_113");
            case error::current_in_task:
                return make_tuple(3114, "当前有未完成任务，请先返回基站！", "CCR_114");
            case error::failed_to_open_teaching_file:
                return make_tuple(3115, "无法打开示教文件", "CCR_115");
            case error::machine_is_charging:
                return make_tuple(3116, "当前机器正在充电", "CCR_116");
            case error::global_cleanup_failed:
                return make_tuple(3117, "Global cleanup failed", "CCR_117");
            case error::path_generation_failed:
                return make_tuple(3118, "Path generation failed", "CCR_118");
            case error::combination_path_execution_failed:
                return make_tuple(3119, "组合路径执行失败", "CCR_119");
            case error::unknown_command:
                return make_tuple(3120, "Unknown command", "CCR_120");
            case error::machine_is_in_emergency_stop:
                return make_tuple(3121, "当前机器处于急停状态，无法分配任务", "CCR_121");
            case error::machine_is_in_emergency_stop_resume:
                return make_tuple(3122, "机器处于紧急暂停状态，无法继续", "CCR_122");
            case error::machine_is_in_emergency_stop_recharged:
                return make_tuple(3123, "机器处于急停状态，无法充电", "CCR_123");
            case error::parameter_error:
                return make_tuple(3124, "Parameter error", "CCR_124");
            case error::regional_index_not_exist:
                return make_tuple(3125, "区域索引不存在", "CCR_125");
            case error::failed_open_partition_file:
                return make_tuple(3126, "无法打开分区文件", "CCR_126");
            case error::state_cannot_be_paused:
                return make_tuple(3127, "无法暂停当前状态", "CCR_127");
            case error::the_robot_is_charging:
                return make_tuple(3128, "机器人正在充电", "CCR_128");
            case error::ave_location_info_file_fail:
                return make_tuple(3129, "位置文件保存失败", "CCR_129");
            case error::combined_duplicate_name:
                return make_tuple(3130, "组合路径名称已存在", "CCR_130");
            case error::cannot_access_view_part_list:
                return make_tuple(3131, "无法解析", "CCR_131");
            case error::unable_to_get_combination:
                return make_tuple(3132, "无法获取当前组合", "CCR_132");
            case error::there_are_unmatched_parts:
                return make_tuple(3133, "存在已删除的不匹配区域", "CCR_133");
            case error::failed_to_open_version_file:
                return make_tuple(3134, "无法打开版本文件", "CCR_134");
            case error::duplicate_file_name:
                return make_tuple(3135, "文件重名,保存失败", "CCR_135");
            case error::failed_to_obtain_the_path_file:
                return make_tuple(3136, "路径文件获取失败,无法检验重名，保存失败", "CCR_136");
            case error::failed_same_name_as_the_combined_path:
                return make_tuple(3137, "文件与组合路径重名,保存失败", "CCR_137");
            case error::robot_hw_info_parsing_failed:
                return make_tuple(3138, "robot_hw_info parsing failed", "CCR_138");
            case error::robot_hw_info_not_exist:
                return make_tuple(3139, "robot_hw_info not exist", "CCR_139");
            case error::room_initialize_fail:
                return make_tuple(3140, "分区初始化失败", "CCR_140");
            case error::room_array_out_of_bounds:
                return make_tuple(3141, "数组越界，请检查参数", "CCR_141");
            case error::room_not_neighbor:
                return make_tuple(3142, "不是相邻区域，请选择相邻区域操作", "CCR_142");
            case error::room_mb_file_open_fail:
                return make_tuple(3143, "打开分区文件失败", "CCR_143");
            case error::room_mb_file_read_fail:
                return make_tuple(3144, "读取分区文件失败", "CCR_144");
            case error::room_mb_file_load_fail:
                return make_tuple(3145, "加载分区文件失败，已重置分区", "CCR_145");
            case error::room_no_partition:
                return make_tuple(3146, "还未进行分区", "CCR_146");
            case error::room_no_find_room_id:
                return make_tuple(3147, "找不到参数中的房间", "CCR_147");
            case error::room_has_too_small_room:
                return make_tuple(3148, "拆分后包含太小的房间", "CCR_148");
            case error::room_both_ends_of_the_split_line_are_in_the_room:
                return make_tuple(3149, "分割线两端在房间内", "CCR_149");
            case error::room_the_dividing_line_does_not_pass_through_the_room:
                return make_tuple(3150, "分割线不经过房间", "CCR_150");
            case error::exploration_initialize_fail:
                return make_tuple(3151, "全覆盖初始化失败", "CCR_151");
            case error::exploration_room_is_too_small:
                return make_tuple(3152, "房间太小，无法规划全覆盖路径", "CCR_152");
            case error::exploration_obstacles_around_the_base_station:
                return make_tuple(3153, "基站周围有障碍物", "CCR_153");
            case error::exploration_segmentation_planning_fail:
                return make_tuple(3154, "分区全覆盖路径规划失败", "CCR_154");
            case error::exploration_path_planning_failed:
                return make_tuple(3155, "路径规划失败", "CCR_155");
            case error::task_planner_failed_to_start:
                return make_tuple(3156, "规划器启动失败", "CCR_156");
            case error::task_mode_no_find:
                return make_tuple(3157, "找不到相对应的mode", "CCR_157");
            case error::task_rectangle_zoned_size_error:
                return make_tuple(3158, "矩形区域参数错误", "CCR_158");
            case error::full_cleaning_mode_error:
                return make_tuple(3159, "全覆盖清洁模式出错", "CCR_159");
            case error::dispatcher_task_low_rsoc:
                return make_tuple(3160, "电量过低", "CCR_160");
            case error::dispatcher_task_work_mode_mapping:
                return make_tuple(3161, "当前为建图模式，不支持任务分发", "CCR_161");
            case error::combination_point_get_view_part_fail:
                return make_tuple(3162, "获取View Part List失败", "CCR_162");
            case error::combination_brief_get_fail:
                return make_tuple(3163, "获取combinationbrief失败", "CCR_163");
            case error::fullclean_brief_get_fail:
                return make_tuple(3164, "获取fullclean失败", "CCR_164");
            case error::combination_pointlist_load_failed:
                return make_tuple(3165, "加载组合路径失败", "CCR_165");
            case error::combination_viewpartlist_empty:
                return make_tuple(3166, "组合路径为空", "CCR_166");
            case error::combination_pointlist_empty:
                return make_tuple(3167, "组合路径 pointlist 为空", "CCR_167");
            case error::already_returning_to_the_base_station:
                return make_tuple(3168, "正在返回基站，请勿重复点击", "CCR_168");
            case error::already_in_the_base_station:
                return make_tuple(3169, "已经在基站位置了", "CCR_169");
            case error::pause_is_not_supported:
                return make_tuple(3170, "不支持暂停", "CCR_170");
            case error::it_is_currently_suspended:
                return make_tuple(3171, "当前已经为暂停状态", "CCR_171");
            case error::not_paused_status:
                return make_tuple(3172, "不是暂停状态", "CCR_172");
            case error::the_current_state_is_uncontrollable:
                return make_tuple(3173, "当前状态不可控制", "CCR_173");
            case error::machine_is_in_emergency_stop_command_not_supported:
                return make_tuple(3174, "机器处于急停状态，不支持本次指令", "CCR_174");
            case error::the_current_task_is_not_completed:
                return make_tuple(3175, "当前任务未执行完成", "CCR_175");
            case error::not_supported_temporarily:
                return make_tuple(3176, "暂不支持", "CCR_176");
            case error::already_in_manual_clean_mode:
                return make_tuple(3177, "已经在手动模式中", "CCR_177");
            case error::not_in_manual_clean_mode:
                return make_tuple(3178, "当前不在手动模式中", "CCR_178");
            case error::current_in_manual_clean_mode:
                return make_tuple(3179, "手动模式中，任务无法启动", "CCR_179");
            case error::clean_water_level_check_failed:
                return make_tuple(3180, "清水箱空，任务无法启动", "CCR_180");
            case error::dirty_water_level_check_failed:
                return make_tuple(3181, "污水箱满，任务无法启动", "CCR_181");
            case error::dispatcher_maintenance_mode:
                return make_tuple(3182, "维护模式不能启动任务", "CCR_182");
            default:
                std::string base_string = "CCR_";
                std::string ex_string = to_string(100 + e.code().value());
                std::string error_code2 = base_string + ex_string;
                return make_tuple(e.code().value(), "未知错误", error_code2);
        }
    }


    void CleanHistoryCenter::upDateByRealTask(const RealTask &real_task) {
        std::unique_lock<std::mutex> lock(history_update_mutex_);
        CleanHistory history = CleanHistoryDataBase::instance().getCleanHistory(real_task.getId());
        //更新工作状态
        history.sweep_status_ = real_task.getWorkStatus().getSweepStatus();
        history.mop_status_ = real_task.getWorkStatus().getMopStatus();
        history.vacuum_status_ = real_task.getWorkStatus().getVacuumStatus();
        history.push_status_ = real_task.getWorkStatus().getPushStatus();
        history.aromatherapy_status_ = real_task.getWorkStatus().getAromatherapyStatus();
        history.disinfect_status_ = real_task.getWorkStatus().getDisinfectStatus();
        //更新遍数和点数
        if (!real_task.getPlanBlocks().empty()) {
            history.total_step_ = real_task.getTotalStep();
            history.total_frequency_ = real_task.getTotalFrequency();
        }
        //更新历史纪录
        CleanHistoryDataBase::instance().updateHistory(history);
    }

    bool CleanHistoryCenter::executeTask(const RealTask &real_task) {
        std::unique_lock<std::mutex> lock(history_update_mutex_);
        current_history_ = CleanHistoryDataBase::instance().getCleanHistory(real_task.getId());

        long execute_time;
        time_t timep;
        time(&timep);
        execute_time = timep * 1000;//毫秒
        current_history_.execute_time_ = execute_time;
        current_history_.history_state_ = history_state::executing;
        //更新到数据库
        CleanHistoryDataBase::instance().updateHistory(current_history_);
        return true;
    }

    bool CleanHistoryCenter::setCurrentFlow(int doing) {
        std::unique_lock<std::mutex> lock(history_update_mutex_);
        //更新当前流程
        if (current_history_.task_id_.empty()) {
            return false;
        }
        current_history_.current_flow_ = doing;
        CleanHistoryDataBase::instance().updateHistory(current_history_);
        return true;
    }

    bool CleanHistoryCenter::addUrgencyStop() {
        std::unique_lock<std::mutex> lock(history_update_mutex_);
        if (current_history_.task_id_.empty()) {
            return false;
        }
        current_history_.urgency_stop_++;
        current_history_.oper_event_.push_back(internal_event::URGENCY_STOP);
        CleanHistoryDataBase::instance().updateHistory(current_history_);
        return true;
    }

    bool CleanHistoryCenter::cancelUrgencyStop() {
        std::unique_lock<std::mutex> lock(history_update_mutex_);
        if (current_history_.task_id_.empty()) {
            return false;
        }
        current_history_.oper_event_.push_back(internal_event::CANCEL_URGENCY_STOP);
        CleanHistoryDataBase::instance().updateHistory(current_history_);
        return true;
    }

    bool CleanHistoryCenter::manualBack() {
        std::unique_lock<std::mutex> lock(history_update_mutex_);
        if (current_history_.task_id_.empty()) {
            return false;
        }
        current_history_.oper_event_.push_back(internal_event::MANUAL_BACK_TO_BASE);
        current_history_.manual_back_++;
        CleanHistoryDataBase::instance().updateHistory(current_history_);
        return true;
    }

    //添加一次暂停
    bool CleanHistoryCenter::addPause() {
        std::unique_lock<std::mutex> lock(history_update_mutex_);
        if (current_history_.task_id_.empty()) {
            return false;
        }
        current_history_.pause_num_++;
        CleanHistoryDataBase::instance().updateHistory(current_history_);
        return true;
    }

    bool CleanHistoryCenter::lowPowerBack() {
        std::unique_lock<std::mutex> lock(history_update_mutex_);
        if (current_history_.task_id_.empty()) {
            return false;
        }
        current_history_.oper_event_.push_back(internal_event::LOW_BATTERY_BACK_CHARGE);
        current_history_.low_power_back_++;
        CleanHistoryDataBase::instance().updateHistory(current_history_);
        return true;
    }

    bool CleanHistoryCenter::equipmentErrorBack(
            bool clean_water_level_check_failed_,
            bool dirty_water_level_check_failed_,
            bool motor_error_
    ) {
        std::unique_lock<std::mutex> lock(history_update_mutex_);
        if (current_history_.task_id_.empty()) {
            return false;
        }
        if (clean_water_level_check_failed_) {
            current_history_.oper_event_.push_back(internal_event::CLEAN_WATER_LEVEL_CHECK_FAILED);
        }
        if (dirty_water_level_check_failed_) {
            current_history_.oper_event_.push_back(internal_event::DIRTY_WATER_LEVEL_CHECK_FAILED);
        }
        if (motor_error_) {
            current_history_.oper_event_.push_back(internal_event::MOTOR_ERROR_RECOVERY_FAILED);
        }
        CleanHistoryDataBase::instance().updateHistory(current_history_);
        return true;
    }

    bool CleanHistoryCenter::forceBack() {
        std::unique_lock<std::mutex> lock(history_update_mutex_);
        if (current_history_.task_id_.empty()) {
            return false;
        }
        current_history_.force_back_++;
        CleanHistoryDataBase::instance().updateHistory(current_history_);
        return true;
    }

    bool CleanHistoryCenter::setOutStation(int state) {
        std::unique_lock<std::mutex> lock(history_update_mutex_);
        if (current_history_.task_id_.empty()) {
            return false;
        }
        current_history_.out_station_ = state;
        CleanHistoryDataBase::instance().updateHistory(current_history_);
        return true;
    }

    bool CleanHistoryCenter::setEndSleep(int state) {
        std::unique_lock<std::mutex> lock(history_update_mutex_);
        if (current_history_.task_id_.empty()) {
            return false;
        }
        current_history_.end_sleep_ = state;
        CleanHistoryDataBase::instance().updateHistory(current_history_);
        return true;
    }

    bool CleanHistoryCenter::setBackBaseRetries(int retries) {
        std::unique_lock<std::mutex> lock(history_update_mutex_);
        if (current_history_.task_id_.empty()) {
            return false;
        }
        current_history_.back_base_retries_ = retries;
        CleanHistoryDataBase::instance().updateHistory(current_history_);
        return true;
    }

    bool CleanHistoryCenter::updateCleanHistory(const RealBlock &realBlock, const RealPoint &realPoint) {
        std::unique_lock<std::mutex> lock(history_update_mutex_);
        if (current_history_.task_id_.empty()) {
            return false;
        }
        //更新执行时间
        long current_time;
        time_t timep;
        time(&timep);
        current_time = timep * 1000;//毫秒
        current_history_.clean_time_ = (current_time - current_history_.execute_time_) / 1000 / 60;
        //更新点位执行情况
        current_history_.current_frequency_ = realBlock.currentFrequency;
        current_history_.current_step_ = realPoint.id;
        //更新清洁面积
        current_history_.clean_area_ += (abs((double) realPoint.timeout - 5.0) / 20 * 0.35);
        //更新到数据库
        CleanHistoryDataBase::instance().updateHistory(current_history_);
        return true;
    }

    bool CleanHistoryCenter::setBackBasePointArrived(int state) {
        std::unique_lock<std::mutex> lock(history_update_mutex_);
        if (current_history_.task_id_.empty()) {
            return false;
        }
        current_history_.back_base_point_arrived_ = state;
        CleanHistoryDataBase::instance().updateHistory(current_history_);
        return true;
    }

    bool CleanHistoryCenter::setStationArrived(int state) {
        std::unique_lock<std::mutex> lock(history_update_mutex_);
        if (current_history_.task_id_.empty()) {
            return false;
        }
        current_history_.station_arrived_ = state;
        CleanHistoryDataBase::instance().updateHistory(current_history_);
        return true;
    }

    bool CleanHistoryCenter::setRechargeRetries(int retries) {
        std::unique_lock<std::mutex> lock(history_update_mutex_);
        if (current_history_.task_id_.empty()) {
            return false;
        }
        current_history_.recharge_retries_ = retries;
        CleanHistoryDataBase::instance().updateHistory(current_history_);
        return true;
    }

    //设置清洁机构关闭是否成功
    bool CleanHistoryCenter::setCloseMechanism(int state) {
        std::unique_lock<std::mutex> lock(history_update_mutex_);
        if (current_history_.task_id_.empty()) {
            return false;
        }
        current_history_.close_mechanism_ = state;
        CleanHistoryDataBase::instance().updateHistory(current_history_);
        return true;
    }

    //设置清洁机构打开是否成功
    bool CleanHistoryCenter::setOpenMechanism(int state) {
        std::unique_lock<std::mutex> lock(history_update_mutex_);
        if (current_history_.task_id_.empty()) {
            return false;
        }
        current_history_.open_mechanism_ = state;
        CleanHistoryDataBase::instance().updateHistory(current_history_);
        return true;
    }

    bool CleanHistoryCenter::resetCurrentTask() {
        return false;
    }

    //正常结束当前任务
    bool CleanHistoryCenter::complete() {
        std::unique_lock<std::mutex> lock(history_update_mutex_);
        if (current_history_.task_id_.empty()) {
            return false;
        }
        //设置结束时间
        long end_time;
        time_t timep;
        time(&timep);
        end_time = timep * 1000;//毫秒
        current_history_.end_time_ = end_time;
        for (auto item: current_history_.oper_event_) {
            if (item == internal_event::MOTOR_ERROR_RECOVERY_FAILED) {
                current_history_.error_code_ = 3218;
                current_history_.error_msg_ = "尘推堵转，任务提前结束";
                current_history_.error_code2_ = "CCR_218";
                current_history_.history_state_ = history_state::error;
            }
        }
        if (current_history_.error_code_ == 0) {
            current_history_.error_code_ = 0;
            current_history_.error_msg_ = "无错误";
            current_history_.error_code2_ = "CCR_000";
            current_history_.history_state_ = history_state::done;
        }
        CleanHistoryDataBase::instance().updateHistory(current_history_);

        long cleanTime = (current_history_.end_time_ - current_history_.execute_time_) / 1000;
        WorkStatus workStatus(
                current_history_.sweep_status_,
                current_history_.mop_status_,
                current_history_.vacuum_status_,
                current_history_.push_status_,
                current_history_.aromatherapy_status_,
                current_history_.disinfect_status_
        );
        updateProperty(workStatus, cleanTime);

        CleanHistory default_history;
        current_history_ = default_history;
        return true;
    }

    void CleanHistoryCenter::updateProperty(const WorkStatus &workStatus, long cleanTime) {
        PropertyDataBase::instance().updateConsumable(
                workStatus.getSweepStatus() > 0 ? cleanTime : 0,
                workStatus.getMopStatus() > 0 ? cleanTime : 0,
                workStatus.getVacuumStatus() > 0 ? cleanTime : 0,
                workStatus.getPushStatus() > 0 ? cleanTime : 0,
                workStatus.getAromatherapyStatus() > 0 ? cleanTime : 0,
                workStatus.getDisinfectStatus() > 0 ? cleanTime : 0
        );
    }

    //错误结束当前任务
    bool CleanHistoryCenter::errorComplete(int error_code, std::string error_string, std::string error_code2) {
        std::unique_lock<std::mutex> lock(history_update_mutex_);
        if (current_history_.task_id_.empty()) {
            return false;
        }
        long end_time;
        time_t timep;
        time(&timep);
        end_time = timep * 1000;//毫秒
        current_history_.end_time_ = end_time;
        current_history_.history_state_ = history_state::error;
        current_history_.error_code_ = error_code;
        current_history_.error_msg_ = error_string;
        current_history_.error_code2_ = error_code2;

        CleanHistoryDataBase::instance().updateHistory(current_history_);

        CleanHistory default_history;
        current_history_ = default_history;
        return true;
    }

    bool CleanHistoryCenter::successComplete(int error_code, std::string error_string, std::string error_code2) {
        std::unique_lock<std::mutex> lock(history_update_mutex_);
        if (current_history_.task_id_.empty()) {
            return false;
        }
        long end_time;
        time_t timep;
        time(&timep);
        end_time = timep * 1000;//毫秒
        current_history_.end_time_ = end_time;
        current_history_.history_state_ = history_state::done;
        current_history_.error_code_ = error_code;
        current_history_.error_msg_ = error_string;
        current_history_.error_code2_ = error_code2;

        CleanHistoryDataBase::instance().updateHistory(current_history_);

        long cleanTime = (current_history_.end_time_ - current_history_.execute_time_) / 1000;
        WorkStatus workStatus(
                current_history_.sweep_status_,
                current_history_.mop_status_,
                current_history_.vacuum_status_,
                current_history_.push_status_,
                current_history_.aromatherapy_status_,
                current_history_.disinfect_status_
        );
        updateProperty(workStatus, cleanTime);

        CleanHistory default_history;
        current_history_ = default_history;
        return true;
    }

    bool CleanHistoryCenter::laserInterrupt() {
        std::unique_lock<std::mutex> lock(history_update_mutex_);
        if (current_history_.task_id_.empty()) {
            return false;
        }
        long end_time;
        time_t timep;
        time(&timep);
        end_time = timep * 1000;//毫秒
        current_history_.end_time_ = end_time;
        current_history_.history_state_ = history_state::error;
        current_history_.error_code_ = 3219;
        current_history_.oper_event_.push_back(internal_event::LASER_ERROR_RECOVERY_FAILED);
        current_history_.error_msg_ = "激光雷达异常";
        current_history_.error_code2_ = "CCR_219";

        CleanHistoryDataBase::instance().updateHistory(current_history_);

        long cleanTime = (current_history_.end_time_ - current_history_.execute_time_) / 1000;
        WorkStatus workStatus(
                current_history_.sweep_status_,
                current_history_.mop_status_,
                current_history_.vacuum_status_,
                current_history_.push_status_,
                current_history_.aromatherapy_status_,
                current_history_.disinfect_status_
        );
        updateProperty(workStatus, cleanTime);

        CleanHistory default_history;
        current_history_ = default_history;
        return true;
    }

    bool CleanHistoryCenter::enterManualCleanMode() {
        std::unique_lock<std::mutex> lock(history_update_mutex_);
        if (current_history_.task_id_.empty()) {
            return false;
        }
        long end_time;
        time_t timep;
        time(&timep);
        end_time = timep * 1000; //毫秒
        current_history_.end_time_ = end_time;
        current_history_.error_code_ = 0;
        current_history_.error_msg_ = "无错误";
        current_history_.error_code2_ = "CCR_000";
        current_history_.history_state_ = history_state::done;
        current_history_.oper_event_.push_back(internal_event::ENTER_MANUAL_CLEAN_MODE);

        CleanHistoryDataBase::instance().updateHistory(current_history_);

        long cleanTime = (current_history_.end_time_ - current_history_.execute_time_) / 1000;
        WorkStatus workStatus(
                current_history_.sweep_status_,
                current_history_.mop_status_,
                current_history_.vacuum_status_,
                current_history_.push_status_,
                current_history_.aromatherapy_status_,
                current_history_.disinfect_status_
        );
        updateProperty(workStatus, cleanTime);

        CleanHistory default_history;
        current_history_ = default_history;
        return true;
    }

    bool CleanHistoryCenter::specialComplete() {
        std::unique_lock<std::mutex> lock(history_update_mutex_);
        if (current_history_.task_id_.empty()) {
            return false;
        }
        long end_time;
        time_t timep;
        time(&timep);
        end_time = timep * 1000;//毫秒
        current_history_.end_time_ = end_time;
        current_history_.error_code_ = 0;
        current_history_.error_msg_ = "无错误";
        current_history_.error_code2_ = "CCR_000";
        current_history_.history_state_ = history_state::done;

        CleanHistoryDataBase::instance().updateHistory(current_history_);

        long cleanTime = (current_history_.end_time_ - current_history_.execute_time_) / 1000;
        WorkStatus workStatus(
                current_history_.sweep_status_,
                current_history_.mop_status_,
                current_history_.vacuum_status_,
                current_history_.push_status_,
                current_history_.aromatherapy_status_,
                current_history_.disinfect_status_
        );
        updateProperty(workStatus, cleanTime);

        CleanHistory default_history;
        current_history_ = default_history;
        return true;
    }

    std::tuple<int, std::string, std::string> CleanHistoryCenter::generateErrorMessageFromLastFlow(int flow) {
        int error_code;
        std::string error_msg;
        switch (flow) {
            case event::flow::waiting_for_task:
                return make_tuple(3300, "等待时被关机", "CCR_300");
            case event::flow::out_base_station:
                return make_tuple(3301, "出站时被关机", "CCR_301");
            case event::flow::switch_node_work_mode:
                return make_tuple(3302, "切换工作模式时被关机", "CCR_302");
            case event::flow::preliminary_preparation_completed:
                return make_tuple(3303, "转场时被关机", "CCR_303");
            case event::flow::ensure_move_to_start_point:
                return make_tuple(3304, "转场时被关机", "CCR_304");
            case event::flow::cleaning_mechanism_ready:
                return make_tuple(3305, "转场时被关机", "CCR_305");
//            case event::flow::again_move_to_start_point:
//                return make_tuple(3307, "转场时被关机", "CCR_307");
//            case event::flow::again_prepare_cleaning_mechanism:
//                return make_tuple(3308, "转场时被关机", "CCR_308");
            case event::flow::flowing_water_production:
                return make_tuple(3306, "清洁时被关机", "CCR_306");
            case event::flow::flowing_water_execution_completed:
                return make_tuple(3309, "清洁时被关机", "CCR_309");
            case event::flow::arrive_base_point_success:
                return make_tuple(3310, "自动返回基站时被关机", "CCR_310");
            case event::flow::try_recharging_again:
                return make_tuple(3311, "自动返回基站时被关机", "CCR_311");
            case event::flow::try_move_base_point_again:
                return make_tuple(3312, "自动返回基站时被关机", "CCR_312");
            case event::flow::arrive_base_station_success:
                return make_tuple(3313, "自动返回基站时被关机", "CCR_313");
//            case event::flow::manual_over_and_move_base_point:
//                return make_tuple(3314, "手动返回基站时被关机", "CCR_314");
//            case event::flow::manual_back_try_move_base_point:
//                return make_tuple(3315, "手动返回基站时被关机", "CCR_315");
//            case event::flow::manual_base_point_and_close_mechanism:
//                return make_tuple(3316, "手动返回基站时被关机", "CCR_316");
//            case event::flow::manual_mechanism_close_and_charging:
//                return make_tuple(3317, "手动返回基站时被关机", "CCR_317");
//            case event::flow::manual_over_success:
//                return make_tuple(3318, "手动返回基站时被关机", "CCR_318");
//            case event::flow::manual_task_pause:
//                return make_tuple(3319, "手动暂停后被关机", "CCR_319");
//            case event::flow::force_over_and_move_base_point:
//                return make_tuple(3320, "强制返回基站时被关机", "CCR_320");
//            case event::flow::force_back_try_move_base_point:
//                return make_tuple(3321, "强制返回基站时被关机", "CCR_321");
//            case event::flow::force_base_point_and_close_mechanism:
//                return make_tuple(3322, "强制返回基站时被关机", "CCR_322");
//            case event::flow::force_mechanism_close_and_charging:
//                return make_tuple(3323, "强制返回基站时被关机", "CCR_323");
//            case event::flow::force_over_success:
//                return make_tuple(3324, "强制返回基站时被关机", "CCR_324");
//            case event::flow::force_task_pause:
//                return make_tuple(3325, "急停时被关机", "CCR_325");
//            case event::flow::manual_control_over_and_move_base_point:
//                return make_tuple(3326, "手动返回基站时被关机", "CCR_326");
//            case event::flow::manual_control_back_try_move_base_point:
//                return make_tuple(3327, "手动返回基站时被关机", "CCR_327");
//            case event::flow::manual_control_base_point_and_charging:
//                return make_tuple(3328, "手动返回基站时被关机", "CCR_328");
//            case event::flow::manual_control_over_success:
//                return make_tuple(3329, "手动返回基站时被关机", "CCR_329");
            case event::flow::hardware_interrupt_task:
                return make_tuple(3330, "硬件出错后被关机", "CCR_330");
            case event::flow::software_interrupt_task:
                return make_tuple(3331, "程序错误后被关机", "CCR_331");
            default:
                std::string base_string = "CCR_";
                std::string flow_string = to_string(300 + flow);
                std::string error_code2 = base_string + flow_string;
                return make_tuple(3300 + flow, "未知错误", error_code2);
        }
    }

    std::string CleanHistoryCenter::make_error_msg(int error_code) {
        return std::__cxx11::string();
    }
}// namespace clean_history_db