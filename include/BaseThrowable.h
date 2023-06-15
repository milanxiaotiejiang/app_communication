//
// Created by mi on 2022/9/5.
// app::exception e(error::make_error_code(error::custom));
//

#ifndef APP_COMMUNICATION_BASETHROWABLE_H
#define APP_COMMUNICATION_BASETHROWABLE_H

#include "string"
#include "exception"
#include "utility"
#include "system_error"

namespace error {
    /**
     * 添加请追加
     * * 为其他地方需要根据此 value 的 code 值判断
     */
    enum value {
        general = 1,
        operation_failed,
        acquisition_file_failed,
        open_file_fail,
        duplicate_path_file_name,
        add_failed,
        delete_failed,
        save_failed,
        create_file_fail,
        create_file_timer_fail,
        open_file_timer_fail,
        save_file_timer_failed,
        file_acquisition_failed,
        current_in_task,
        failed_to_open_teaching_file,
        machine_is_charging,
        global_cleanup_failed,
        path_generation_failed,
        combination_path_execution_failed,
        unknown_command,
        machine_is_in_emergency_stop,
        machine_is_in_emergency_stop_resume,
        machine_is_in_emergency_stop_recharged,
        parameter_error,
        regional_index_not_exist,
        failed_open_partition_file,
        state_cannot_be_paused,
        the_robot_is_charging,
        ave_location_info_file_fail,
        combined_duplicate_name,
        cannot_access_view_part_list,
        unable_to_get_combination,
        there_are_unmatched_parts,
        failed_to_open_version_file,
        duplicate_file_name,
        failed_to_obtain_the_path_file,
        failed_same_name_as_the_combined_path,
        robot_hw_info_parsing_failed,
        robot_hw_info_not_exist,
        room_initialize_fail,
        room_array_out_of_bounds,
        room_not_neighbor,
        room_mb_file_open_fail,
        room_mb_file_read_fail,
        room_mb_file_load_fail,
        room_no_partition,
        room_no_find_room_id,
        room_has_too_small_room,
        room_both_ends_of_the_split_line_are_in_the_room,
        room_the_dividing_line_does_not_pass_through_the_room,
        exploration_initialize_fail,
        exploration_room_is_too_small,
        exploration_obstacles_around_the_base_station,
        exploration_segmentation_planning_fail,
        exploration_path_planning_failed,
        task_planner_failed_to_start,
        task_mode_no_find,
        task_rectangle_zoned_size_error,
        full_cleaning_mode_error,
        dispatcher_task_low_rsoc,
        dispatcher_task_work_mode_mapping,
        combination_point_get_view_part_fail,
        combination_brief_get_fail,
        fullclean_brief_get_fail,
        combination_pointlist_load_failed,
        combination_viewpartlist_empty,
        combination_pointlist_empty,
        already_returning_to_the_base_station,
        already_in_the_base_station,
        pause_is_not_supported,
        it_is_currently_suspended,
        not_paused_status,
        the_current_state_is_uncontrollable,
        machine_is_in_emergency_stop_command_not_supported,
        machine_is_in_manual_mode_command_not_supported,
        operation_failure_please_restart_the_machine,
        the_current_task_is_not_completed,
        not_supported_temporarily,
        already_in_manual_clean_mode,
        not_in_manual_clean_mode,
        manual_in_the_base_station,
        current_in_manual_clean_mode,
        clean_water_level_check_failed,
        dirty_water_level_check_failed,
        operation_is_not_allowed_in_the_station,
        operation_not_allowed_in_outbound,
        in_creating_map,
        coverage_path_overtime,
        failed_to_parse_speed_related_files,
        failed_to_parse_fall_prevention_related_files,
        mode_switching_is_not_supported,
        create_map_fail,
        create_map_fail_to_sleep,
        the_main_task_is_not_set,
        invalid_sweep_status,
        invalid_mop_status,
        invalid_vacuum_status,
        invalid_push_status,
        invalid_aromatherapy_status,
        invalid_disinfect_status,
        invalid_name,
        invalid_rate,
        invalid_mode,
        invalid_source,
        invalid_zones,
        invalid_subregions,
        no_task_for_return_base_station,
        add_zone_fail,
        add_subregion_fail,
        cannot_switch_to_the_current_map,
        map_id_does_not_exist,
        no_run_task,
        dispatcher_maintenance_mode,
        map_creation_needs_to_start_at_the_base_station,
        the_map_needs_to_be_saved_at_the_base_station_location,
        quit_map_needs_to_be_saved_at_the_base_station_location,
        please_ensure_to_start_end_the_mapping_at_the_base_station,
        area_too_small
    };// enum value

    class category : public std::error_category {
    public:
        category() = default;

        const char *name() const noexcept override {
            return "app_communication";
        }

        std::string message(int value) const override {
            switch (value) {
                case error::general:
                    return "Generic error";
                case error::operation_failed:
                    return "操作失败";
                case error::acquisition_file_failed:
                    return "文件获取失败";
                case error::open_file_fail:
                    return "无法打开文件";
                case error::duplicate_path_file_name:
                    return "路径文件名重复";
                case error::add_failed:
                    return "添加失败";
                case error::delete_failed:
                    return "删除失败";
                case error::save_failed:
                    return "保存失败";
                case error::create_file_fail:
                    return "创建文件失败";
                case error::create_file_timer_fail:
                    return "创建定时器文件失败";
                case error::open_file_timer_fail:
                    return "打开定时器文件失败";
                case error::save_file_timer_failed:
                    return "保存定时器文件失败";
                case error::file_acquisition_failed:
                    return "该part在组合路径中已被使用";
                case error::failed_to_open_teaching_file:
                    return "无法打开示教文件";
                case error::machine_is_charging:
                    return "当前机器正在充电";
                case error::global_cleanup_failed:
                    return "Global cleanup failed";
                case error::path_generation_failed:
                    return "Path generation failed";
                case error::combination_path_execution_failed:
                    return "组合路径执行失败";
                case error::unknown_command:
                    return "Unknown command";
                case error::machine_is_in_emergency_stop:
                    return "当前机器处于急停状态，无法分配任务";
                case error::machine_is_in_emergency_stop_resume:
                    return "机器处于紧急暂停状态，无法继续";
                case error::machine_is_in_emergency_stop_recharged:
                    return "机器处于急停状态，无法充电";
                case error::parameter_error:
                    return "Parameter error";
                case error::regional_index_not_exist:
                    return "区域索引不存在";
                case error::failed_open_partition_file:
                    return "无法打开分区文件";
                case error::state_cannot_be_paused:
                    return "无法暂停当前状态";
                case error::the_robot_is_charging:
                    return "机器人正在充电";
                case error::ave_location_info_file_fail:
                    return "位置文件保存失败";
                case error::combined_duplicate_name:
                    return "组合路径名称已存在";
                case error::cannot_access_view_part_list:
                    return "无法解析";
                case error::unable_to_get_combination:
                    return "无法获取当前组合";
                case error::there_are_unmatched_parts:
                    return "存在已删除的不匹配区域";
                case error::current_in_task:
                    return "当前有未完成任务，请先返回基站！";
                case error::failed_to_open_version_file:
                    return "无法打开版本文件";
                case error::duplicate_file_name:
                    return "文件重名,保存失败";
                case error::failed_to_obtain_the_path_file:
                    return "路径文件获取失败,无法检验重名，保存失败";
                case error::failed_same_name_as_the_combined_path:
                    return "文件与组合路径重名,保存失败";
                case error::robot_hw_info_parsing_failed:
                    return "robot_hw_info parsing failed";
                case error::robot_hw_info_not_exist:
                    return "robot_hw_info not exist";
                case error::room_initialize_fail:
                    return "分区初始化失败";
                case error::room_array_out_of_bounds:
                    return "数组越界，请检查参数";
                case error::room_not_neighbor:
                    return "不是相邻区域，请选择相邻区域操作";
                case error::room_mb_file_open_fail:
                    return "打开分区文件失败";
                case error::room_mb_file_read_fail:
                    return "读取分区文件失败";
                case error::room_mb_file_load_fail:
                    return "加载分区文件失败，已重置分区";
                case error::room_no_partition:
                    return "还未进行分区";
                case error::room_no_find_room_id:
                    return "找不到参数中的房间";
                case error::room_has_too_small_room:
                    return "拆分后包含太小的房间";
                case error::room_both_ends_of_the_split_line_are_in_the_room:
                    return "分割线两端在房间内";
                case error::room_the_dividing_line_does_not_pass_through_the_room:
                    return "分割线不经过房间";
                case error::exploration_initialize_fail:
                    return "全覆盖初始化失败";
                case error::exploration_room_is_too_small:
                    return "房间太小，无法规划全覆盖路径";
                case error::exploration_obstacles_around_the_base_station:
                    return "基站周围有障碍物";
                case error::exploration_segmentation_planning_fail:
                    return "分区全覆盖路径规划失败";
                case error::exploration_path_planning_failed:
                    return "路径规划失败";
                case error::task_planner_failed_to_start:
                    return "规划器启动失败";
                case error::task_mode_no_find:
                    return "找不到相对应的mode";
                case error::task_rectangle_zoned_size_error:
                    return "矩形区域参数错误";
                case error::full_cleaning_mode_error:
                    return "全覆盖清洁模式出错";
                case error::dispatcher_task_low_rsoc:
                    return "电量过低";
                case error::dispatcher_task_work_mode_mapping:
                    return "当前为建图模式，不支持任务分发";
                case error::combination_point_get_view_part_fail:
                    return "获取View Part List失败";
                case error::combination_brief_get_fail:
                    return "获取combinationbrief失败";
                case error::combination_pointlist_load_failed:
                    return "加载组合路径失败";
                case error::combination_viewpartlist_empty:
                    return "组合路径为空";
                case error::combination_pointlist_empty:
                    return "组合路径 pointlist 为空";
                case error::already_returning_to_the_base_station:
                    return "正在返回基站，请勿重复点击";
                case error::already_in_the_base_station:
                    return "已经在基站位置了";
                case error::pause_is_not_supported:
                    return "不支持暂停";
                case error::it_is_currently_suspended:
                    return "当前已经为暂停状态";
                case error::not_paused_status:
                    return "不是暂停状态";
                case error::the_current_state_is_uncontrollable:
                    return "当前状态不可控制";
                case error::machine_is_in_emergency_stop_command_not_supported:
                    return "急停状态，不支持本次指令";
                case error::machine_is_in_manual_mode_command_not_supported:
                    return "手动模式，不支持本次指令";
                case error::operation_failure_please_restart_the_machine:
                    return "运行故障，请重启机器";
                case error::the_current_task_is_not_completed:
                    return "暂不支持任务下发";
                case error::not_supported_temporarily:
                    return "暂不支持";
                case error::fullclean_brief_get_fail:
                    return "获取fullclean失败";
                case error::already_in_manual_clean_mode:
                    return "已经在手动模式中";
                case error::not_in_manual_clean_mode:
                    return "当前不在手动模式中";
                case error::manual_in_the_base_station:
                    return "请推回基站关闭手动模式";
                case error::current_in_manual_clean_mode:
                    return "手动模式中，任务无法启动";
                case error::clean_water_level_check_failed:
                    return "清水箱空，任务无法启动";
                case error::dirty_water_level_check_failed:
                    return "污水箱满，任务无法启动";
                case error::operation_is_not_allowed_in_the_station:
                    return "进站中不允许操作";
                case error::operation_not_allowed_in_outbound:
                    return "出站中不允许操作";
                case error::in_creating_map:
                    return "地图创建中";
                case error::coverage_path_overtime:
                    return "获取全覆盖路径超时";
                case error::failed_to_parse_speed_related_files:
                    return "解析速度相关文件失败";
                case error::failed_to_parse_fall_prevention_related_files:
                    return "解析防跌落相关文件失败";
                case error::mode_switching_is_not_supported:
                    return "暂不支持模式切换，请稍后调用";
                case error::create_map_fail:
                    return "保存地图失败，请稍后调用";
                case error::create_map_fail_to_sleep:
                    return "保存地图失败，请重新启动";
                case error::the_main_task_is_not_set:
                    return "The main task is not set";
                case error::invalid_sweep_status:
                    return "Invalid sweep_status";
                case error::invalid_mop_status:
                    return "Invalid mop_status";
                case error::invalid_vacuum_status:
                    return "Invalid vacuum_status";
                case error::invalid_push_status:
                    return "Invalid push_status";
                case error::invalid_aromatherapy_status:
                    return "Invalid aromatherapy_status";
                case error::invalid_disinfect_status:
                    return "Invalid disinfect_status";
                case error::invalid_name:
                    return "非法名称（名称由中英文、数字、下划线组成的3-30个字符）";
                case error::invalid_rate:
                    return "Invalid rate";
                case error::invalid_mode:
                    return "Invalid mode";
                case error::invalid_source:
                    return "Invalid source";
                case error::invalid_zones:
                    return "Invalid zones";
                case error::invalid_subregions:
                    return "Invalid subregions";
                case error::no_task_for_return_base_station:
                    return "当前无任务，返回基站需要拍急停后推回基站位置";
                case error::add_zone_fail:
                    return "矩形框添加失败";
                case error::add_subregion_fail:
                    return "区域添加失败";
                case error::cannot_switch_to_the_current_map:
                    return "不能切换为当前地图";
                case error::map_id_does_not_exist:
                    return "不存在此地图";
                case error::no_run_task:
                    return "no run task";
                case error::dispatcher_maintenance_mode:
                    return "维护模式不能启动任务";
                case error::map_creation_needs_to_start_at_the_base_station:
                    return "请在基站创建地图";
                case error::the_map_needs_to_be_saved_at_the_base_station_location:
                    return "地图保存需要在基站位置";
                case error::quit_map_needs_to_be_saved_at_the_base_station_location:
                    return "请在基站位置退出建图模式";
                case error::please_ensure_to_start_end_the_mapping_at_the_base_station:
                    return "开始 / 结束建图请保证在基站";
                case error::area_too_small:
                    return "建图面积过小，请重新保存";
                default:
                    return "Unknown";
            }
        }

    };

    inline const std::error_category &get_category() {
        static category instance;
        return instance;
    }

    inline std::error_code make_error_code(error::value e) {
        return std::error_code(static_cast<int>(e), get_category());
    }
}
namespace app {

    class exception : public std::exception {
    public:
        explicit exception(
                std::string const &msg,
                std::error_code ec = make_error_code(error::general)
        ) : m_msg(msg.empty() ? ec.message() : msg), m_code(ec) {}

        explicit exception(std::error_code ec) : m_msg(ec.message()), m_code(ec) {}

        ~exception() throw() {}

        virtual char const *what() const throw() {
            return m_msg.c_str();
        }

        std::error_code code() const throw() {
            return m_code;
        }

        const std::string m_msg;
        std::error_code m_code;
    };

}

#endif //APP_COMMUNICATION_BASETHROWABLE_H
