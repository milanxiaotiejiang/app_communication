/*used for change string format id to int format so that we can use switch to
choose a corresponding function author -jiang
*/

#include "tool/Switch.h"
#include <iostream>

using namespace std;


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


int switch_ID(const std::string str) {

    if (str == "get_device_status") {
        return GET_DEVICE_STATUS_;
    }
    if (str == "app_along_clean") {
        return APP_ALONG_CLEAN_;
    }
    if (str == "save_map") {
        return SAVE_MAP_;
    }
    if (str == "get_multi_maps") {
        return GET_MULTI_MAPS_;
    }
    if (str == "change_map") {
        return CHANGE_MAP_;
    }
    if (str == "edit_map") {
        return EDIT_MAP_;
    }
    if (str == "get_edit_map") {
        return GET_EDIT_MAP_;
    }
    if (str == "execute_task") {
        return EXECUTE_TASK_;
    }
    if (str == "get_task_list") {
        return GET_TASK_LIST_;
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
        printf("change_aromatherapy_state!!!!!!!!!!!!!!!!!!!! \n");
        return CHANGE_AROM_STATUS_;
    }
    if (str == "continuity_task_list") {
        return CONTINUITY_TASK_LIST_;
    }
    if (str == "get_continuity_task_list") {
        return GET_CONTINUITY_TASK_LIST_;
    }
    if (str == "polygon_task_list") {
        return POLYGON_TASK_LIST_;
    }
    if (str == "get_polygon_task_list") {
        return GET_POLYGON_TASK_LIST_;
    }
    if (str == "open_self_cleaning") {
        return OPEN_SELF_CLEANING_;
    }
    if (str == "teach_mode_start") {
        return TEACH_MODE_START_;
    }
    if (str == "teach_mode_stop") {
        return TEACH_MODE_STOP_;
    }
    if (str == "teach_heart_beat") {
        return TEACH_HEART_BEAT_;
    }
    if (str == "get_teach_path_list") {
        return GET_TEACH_PATH_LIST_;
    }
    if (str == "get_teach_path_detail") {
        return GET_TEACH_PATH_DETAIL_;
    }
    if (str == "delete_teach_path_list") {
        return DELETE_TEACH_PATH_LIST_;
    }
    if (str == "get_full_plan") {
        return GET_FULL_PLAN_;
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
    if (str == "work_to_grid_map_app") {
        return WORK_TO_GRID_MAP_APP_;
    }
    if (str == "clean_history_list") {
        return CLEAN_HISTORY_REQUEST_;
    }
    if (str == "combination_part_add") {
        return COMBINATION_PART_ADD_;
    }
    if (str == "combination_combination_add") {
        return COMBINATION_COMBINATION_ADD_;
    }
    if (str == "combination_part_list") {
        return COMBINATION_PART_LIST_;
    }
    if (str == "combination_combination_list") {
        return COMBINATION_COMBINATION_LIST_;
    }
    if (str == "combination_combination_details") {
        return COMBINATION_COMBINATION_DETAILS_;
    }
    if (str == "combination_part_delete") {
        return COMBINATION_PART_DELETE_;
    }
    if (str == "combination_part_delete_force") {
        return COMBINATION_PART_DELETE_FORCE_;
    }
    if (str == "combination_combination_delete") {
        return COMBINATION_COMBINATION_DELETE_;
    }
    if (str == "combination_part_update") {
        return COMBINATION_PART_UPDATE_;
    }
    if (str == "combination_combination_update") {
        return COMBINATION_COMBINATION_UPDATE_;
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
    if (str == "player_recruit_voice") {
        return PLAY_VOICE_;
    }
    if (str == "light_belt_mode") {
        return LIGHT_BELT_MODE_;
    }
    if (str == "open_machine_drawer") {
        return OPEN_MACHINE_DRAWER_;
    }
    if (str == "power_reduction_1") {
        return SET_POWER_REDUCTION;
    }
    if (str == "update_timer") {
        return UPD_TIMER_;
    }
    if (str == "delete_timer") {
        // printf("delete_timer delete_timer delete_timer delete_timer  \n");
        return DEL_TIMER_;
    }
    if (str == "get_timer_list") {
        return GET_TIMER_LIST_;
    }
    if (str == "set_timer") {
        return SET_TIMER_;
    }
    if (str == "work_to_grid_map_app") {
        return WORK_TO_GRID_MAP_APP_;
    }
    if (str == "save_location") {
        return SAVE_LOCATION;
    }
    if (str == "get_location") {
        return GET_LOCATION;
    }
    if (str == "save_project") {
        return SAVE_PROJECT;
    }
    if (str == "get_project") {
        return GET_PROJECT;
    }
    if (str == "pad_version_into") {
        return PAD_VERSION_INTO;
    }
    if (str == "get_machine_model") {
        return GET_MACHINE_MODEL;
    }
    if (str == "main_combination_way") {
        return MAIN_COMBINATION_WAY;
    }
    if (str == "get_material_status") {
        return GET_MATERIAL_STATUS;
    }
    if (str == "cancel_main_combination") {
        return CANCEL_MAIN_COMBINATION;
    }
    if (str == "notice_list") {
        return NOTICE_LIST;
    }
    if (str == "get_device_secret") {
        return GET_DEVICE_SECRET;
    }
    if (str == "tt_error_check") {
        return TT_ERROR_CHECK;
    }
    if (str == "knob_control") {
        return KNOB_CONTROL;
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

    if (str == "ota_start") {
        return OTA_CORE;
    }
    if (str == "manual_push_start") {
        return MANUAL_PUSH_START;
    }
    if (str == "manual_push_reset") {
        return MANUAL_PUSH_RESET;
    }
    if (str == "manual_push_save") {
        return MANUAL_PUSH_SAVE;
    }
    if (str == "get_full_cleaning_mode") {
        return GET_FULL_CLEANING_MODE;
    }
    if (str == "set_full_cleaning_mode") {
        return SET_FULL_CLEANING_MODE;
    }
    if (str == "get_full_cleaning_list") {
        return GET_FULL_CLEAN_LIST;
    }
    if (str == "full_cleaning_add") {
        return FULL_CLEANING_ADD;
    }
    if (str == "full_cleaning_update") {
        return FULL_CLEANING_UPDATE;
    }
    if (str == "full_cleaning_delete") {
        return FULL_CLEANING_DELETE;
    }
    if (str == "full_cleaning_detail") {
        return FULL_CLEANING_DETAIL;
    }
    if (str == "full_cleaning_main") {
        return FULL_CLEANING_MAIN;
    }
    if (str == "full_cleaning_main_cancel") {
        return FULL_CLEANING_MAIN_CANCEL;
    }
    if (str == "enter_manual_mode") {
        return ENTER_MANUAL_MODE;
    }
    if (str == "quit_manual_mode") {
        return QUIT_MANUAL_MODE;
    }
    if (str == "shutdown") {
        return SHUTDOWN;
    }
    if (str == "reboot") {
        return REBOOT;
    }

    return -1;
}
