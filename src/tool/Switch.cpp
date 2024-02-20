/*used for change string format id to int format so that we can use switch to
choose a corresponding function author -jiang
*/

#include "tool/Switch.h"
#include <iostream>

pid_t getProcessPidByName(const char *proc_name) {
    FILE *fp;
    char buf[100];
    char cmd[200] = {'\0'};
    pid_t pid = -1;
    sprintf(cmd, "pidof %s", proc_name);

    if ((fp = popen(cmd, "r")) != NULL) {
        if (fgets(buf, 255, fp) != NULL) {
            pid = atoi(buf);
        }
    }

    printf("pid = %d \n", pid);

    pclose(fp);
    return pid;
}


int switch_ID(const std::string &str) {

    if (str == "get_device_status") {
        return GET_DEVICE_STATUS_;
    }
    if (str == "start_map") {
        return START_MAP;
    }
    if (str == "end_map") {
        return END_MAP;
    }
    if (str == "get_multi_maps") {
        return GET_MULTI_MAPS_;
    }
    if (str == "change_map") {
        return CHANGE_MAP_;
    }
    if (str == "modify_map_name") {
        return MODIFY_MAP_NAME;
    }
    if (str == "delete_map") {
        return DELETE_MAP;
    }

    if (str == "edit_map") {
        return EDIT_MAP_;
    }
    if (str == "get_edit_map") {
        return GET_EDIT_MAP_;
    }
    if (str == "multiple_edit_map") {
        return MULTIPLE_EDIT_MAP;
    }
    if (str == "multiple_get_edit_map") {
        return MULTIPLE_GET_EDIT_MAP;
    }

    if (str == "running_task") {
        return RUNNING_TASK;
    }
    if (str == "app_spot") {
        return APP_SPOT_;
    }
    if (str == "app_pause") {
        return APP_PAUSE_;
    }
    if (str == "app_charge") {
        return APP_CHARGE_;
    }
    if (str == "change_work_status") {
        return CHANGE_WORK_STATUS_;
    }
    if (str == "change_aromatherapy_state") {
        return CHANGE_AROM_STATUS_;
    }
    if (str == "try_to_enter") {
        return TRY_TO_ENTER_;
    }
    if (str == "forced_to_enter") {
        return FORCED_TO_ENTER_;
    }
    if (str == "work_to_enter") {
        return WORK_TO_ENTER_;
    }
    if (str == "work_to_map_app") {
        return WORK_TO_MAP_APP_;
    }
    if (str == "clean_history_list") {
        return CLEAN_HISTORY_REQUEST_;
    }
    if (str == "get_finished_point") {
        return GET_FINISHED_POINT_;
    }
    if (str == "is_in_basement") {
        return IS_IN_BASEMENT_;
    }
    if (str == "get_ros_version") {
        return GET_ROS_VERSION_;
    }
    if (str == "pad_version_into") {
        return PAD_VERSION_INTO;
    }
    if (str == "get_machine_model") {
        return GET_MACHINE_MODEL;
    }
    if (str == "get_device_secret") {
        return GET_DEVICE_SECRET;
    }
    if (str == "collect_dust") {
        return COLLECT_DUST;
    }

    if (str == "room_map_data") {
        return ROOM_MAP_DATA;
    }
    if (str == "room_merge") {
        return ROOM_MERGE;
    }
    if (str == "room_segmentation") {
        return ROOM_SEGMENTATION;
    }
    if (str == "room_reset") {
        return ROOM_RESET;
    }
    if (str == "room_rename") {
        return ROOM_RENAME;
    }
    if (str == "room_auto") {
        return ROOM_AUTO;
    }
    if (str == "region_exploration") {
        return REGION_EXPLORATION;
    }
    if (str == "get_plan_param") {
        return GET_PLAN_PARAM;
    }
    if (str == "set_plan_param") {
        return SET_PLAN_PARAM;
    }
    if (str == "reset_plan_param") {
        return RESET_PLAN_PARAM;
    }

    if (str == "ota_start") {
        return OTA_CORE;
    }
    if (str == "manual_push_start") {
        return MANUAL_PUSH_START;
    }
    if (str == "manual_push_reset") {
        return MANUAL_PUSH_RESET;
    }

    if (str == "enter_manual_mode") {
        return ENTER_MANUAL_MODE;
    }
    if (str == "quit_manual_mode") {
        return QUIT_MANUAL_MODE;
    }

    if (str == "emergency_stop") {
        return EMERGENCY_STOP;
    }
    if (str == "release_emergency_stop") {
        return RELEASE_EMERGENCY_STOP;
    }
    if (str == "unrecoverable_error") {
        return UNRECOVERABLE_ERROR;
    }

    if (str == "shutdown") {
        return SHUTDOWN;
    }
    if (str == "reboot") {
        return REBOOT;
    }

    if (str == "get_robot_params") {
        return GET_ROBOT_PARAMS;
    }
    if (str == "set_robot_params") {
        return SET_ROBOT_PARAMS;
    }
    if (str == "get_hot_wind_mode") {
        return GET_HOT_WIND_MODE;
    }
    if (str == "set_hot_wind_mode") {
        return SET_HOT_WIND_MODE;
    }
    if (str == "get_rain_snow") {
        return GET_RAIN_SNOW;
    }
    if (str == "set_rain_snow") {
        return SET_RAIN_SNOW;
    }

    if (str == "get_collect_dust") {
        return GET_COLLECT_DUST;
    }
    if (str == "set_collect_dust") {
        return SET_COLLECT_DUST;
    }
    if (str == "get_auto_oil") {
        return GET_AUTO_OIL;
    }
    if (str == "set_auto_oil") {
        return SET_AUTO_OIL;
    }
    if (str == "get_maintenance_start_time") {
        return GET_MAINTENANCE_START_TIME;
    }
    if (str == "set_maintenance_start_time") {
        return SET_MAINTENANCE_START_TIME;
    }
    if (str == "get_test_cloud_interactive_environment") {
        return GET_TEST_CLOUD_INTERACTIVE_ENVIRONMENT;
    }
    if (str == "set_test_cloud_interactive_environment") {
        return SET_TEST_CLOUD_INTERACTIVE_ENVIRONMENT;
    }

    if (str == "map_obstacles") {
        return MAP_OBSTACLES;
    }
    if (str == "map_feasible_zone") {
        return MAP_FEASIBLE_ZONE;
    }
    if (str == "multiple_map_obstacles") {
        return MULTIPLE_MAP_OBSTACLES;
    }
    if (str == "multiple_map_feasible_zone") {
        return MULTIPLE_MAP_FEASIBLE_ZONE;
    }
    if (str == "map_apply_increase_area") {
        return MAP_APPLY_INCREASE_AREA;
    }

    if (str == "set_explorer_energy") {
        return SET_EXPLORER_ENERGY;
    }
    if (str == "get_explorer_energy") {
        return GET_EXPLORER_ENERGY;
    }
    if (str == "automatic_oiling") {
        return AUTOMATIC_OILING;
    }

    if (str == "add_task") {
        return ADD_TASK;
    }
    if (str == "multiple_add_task") {
        return MULTIPLE_ADD_TASK;
    }
    if (str == "delete_task") {
        return DELETE_TASK;
    }
    if (str == "delete_multiple_task") {
        return DELETE_MULTIPLE_TASK;
    }
    if (str == "multiple_delete_task") {
        return MULTIPLE_DELETE_TASK;
    }
    if (str == "list_task") {
        return LIST_TASK;
    }
    if (str == "multiple_list_task") {
        return MULTIPLE_LIST_TASK;
    }
    if (str == "multiple_whole_list_task") {
        return MULTIPLE_WHOLE_LIST_TASK;
    }
    if (str == "query_id_task") {
        return QUERY_ID_TASK;
    }
    if (str == "build_principal_task") {
        return BUILD_PRINCIPAL_TASK;
    }
    if (str == "cancel_principal_task") {
        return CANCEL_PRINCIPAL_TASK;
    }
    if (str == "principal_task") {
        return PRINCIPAL_TASK;
    }
    if (str == "multiple_principal_task") {
        return MULTIPLE_PRINCIPAL_TASK;
    }

    if (str == "build_rain_snow_task") {
        return BUILD_RAIN_SNOW_TASK;
    }
    if (str == "cancel_rain_snow_task") {
        return CANCEL_RAIN_SNOW_TASK;
    }
    if (str == "rain_snow_task") {
        return RAIN_SNOW_TASK;
    }
    if (str == "multiple_rain_snow_task") {
        return MULTIPLE_RAIN_SNOW_TASK;
    }

    if (str == "clear_current_list_task") {
        return CLEAR_CURRENT_LIST_TASK;
    }

    if (str == "modify_task_name") {
        return MODIFY_TASK_NAME;
    }
    if (str == "modify_task_rate") {
        return MODIFY_TASK_RATE;
    }
    if (str == "modify_task_work_status") {
        return MODIFY_TASK_WORK_STATUS;
    }
    if (str == "modify_task_knife") {
        return MODIFY_TASK_KNIFE;
    }
    if (str == "modify_complete_task") {
        return MODIFY_COMPLETE_TASK;
    }
    if (str == "operate_add_zone") {
        return OPERATE_ADD_ZONE;
    }
    if (str == "operate_delete_zone") {
        return OPERATE_DELETE_ZONE;
    }
    if (str == "operate_modify_zone") {
        return OPERATE_MODIFY_ZONE;
    }
    if (str == "modify_task_partition") {
        return MODIFY_TASK_PARTITION;
    }
    if (str == "operate_add_subregion") {
        return OPERATE_ADD_SUBREGION;
    }
    if (str == "operate_delete_subregion") {
        return OPERATE_DELETE_SUBREGION;
    }

    if (str == "add_timer_task") {
        return ADD_TIMER_TASK;
    }
    if (str == "multiple_add_timer_task") {
        return MULTIPLE_ADD_TIMER_TASK;
    }
    if (str == "delete_timer_task") {
        return DELETE_TIMER_TASK;
    }
    if (str == "delete_multiple_timer_task") {
        return DELETE_MULTIPLE_TIMER_TASK;
    }
    if (str == "multiple_delete_timer_task") {
        return MULTIPLE_DELETE_TIMER_TASK;
    }
    if (str == "list_timer_task") {
        return LIST_TIMER_TASK;
    }
    if (str == "multiple_list_timer_task") {
        return MULTIPLE_LIST_TIMER_TASK;
    }
    if (str == "multiple_whole_list_timer_task") {
        return MULTIPLE_WHOLE_LIST_TIMER_TASK;
    }
    if (str == "modify_timer_task") {
        return MODIFY_TIMER_TASK;
    }
    if (str == "multiple_modify_timer_task") {
        return MULTIPLE_MODIFY_TIMER_TASK;
    }
    if (str == "modify_timer_name") {
        return MODIFY_TIMER_NAME;
    }
    if (str == "list_timer_task_build") {
        return LIST_TIMER_TASK_BUILD;
    }

    if (str == "exploration_task") {
        return EXPLORATION_TASK;
    }
    if (str == "perform_task") {
        return PERFORM_TASK;
    }

    if (str == "get_consumable") {
        return GET_CONSUMABLE;
    }
    if (str == "reset_consumable") {
        return RESET_CONSUMABLE;
    }

    if (str == "hot_wind_mode") {
        return HOT_WIND_MODE;
    }
    if (str == "hot_wind_mode_status") {
        return HOT_WIND_MODE_STATUS;
    }
    if (str == "maintenance_mode") {
        return MAINTENANCE_MODE;
    }
    if (str == "maintenance_mode_status") {
        return MAINTENANCE_MODE_STATUS;
    }
    if (str == "set_base_station") {
        return SET_BASE_STATION;
    }
    if (str == "get_base_station") {
        return GET_BASE_STATION;
    }

    if (str == "open_self_check") {
        return OPEN_SELF_CHECK;
    }
    if (str == "close_self_check") {
        return CLOSE_SELF_CHECK;
    }

    if (str == "factory_reset") {
        return FACTORY_RESET;
    }

    if (str == "add_gate") {
        return ADD_GATE;
    }
    if (str == "delete_gate") {
        return DELETE_GATE;
    }
    if (str == "purge_gate") {
        return PURGE_GATE;
    }
    if (str == "modify_gate") {
        return MODIFY_GATE;
    }
    if (str == "list_gate") {
        return LIST_GATE;
    }
    if (str == "query_id_gate") {
        return QUERY_ID_GATE;
    }
    if (str == "multiple_modify_gate") {
        return MULTIPLE_MODIFY_GATE;
    }
    if (str == "multiple_list_gate") {
        return MULTIPLE_LIST_GATE;
    }
    if (str == "multiple_purge_gate") {
        return MULTIPLE_PURGE_GATE;
    }

    if (str == "open_gate_setting") {
        return OPEN_GATE_SETTING;
    }
    if (str == "close_gate_setting") {
        return CLOSE_GATE_SETTING;
    }

    if (str == "add_build") {
        return ADD_BUILD;
    }
    if (str == "delete_build") {
        return DELETE_BUILD;
    }
    if (str == "modify_build_name") {
        return MODIFY_BUILD_NAME;
    }
    if (str == "modify_build_elevator_address") {
        return MODIFY_BUILD_ELEVATOR_ADDRESS;
    }
    if (str == "list_build") {
        return LIST_BUILD;
    }

    if (str == "modify_map_base_station") {
        return MODIFY_MAP_BASE_STATION;
    }
    if (str == "modify_map_floor") {
        return MODIFY_MAP_FLOOR;
    }
    if (str == "modify_map_elevator") {
        return MODIFY_MAP_ELEVATOR;
    }
    if (str == "modify_map_elevator_point") {
        return MODIFY_MAP_ELEVATOR_POINT;
    }
    if (str == "modify_map_elevator_rect") {
        return MODIFY_MAP_ELEVATOR_RECT;
    }

    if (str == "list_map_for_build") {
        return LIST_MAP_FOR_BUILD;
    }

    if (str == "attach_build_map") {
        return ATTACH_BUILD_MAP;
    }

    if (str == "map_for_id") {
        return MAP_FOR_ID;
    }

    if (str == "tt_elevator") {
        return TT_ELEVATOR;
    }

    return -1;
}
