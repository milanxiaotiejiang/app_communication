#ifndef APP_COMMUNICATION_INTERNAL_EVENT_PUB_MANAGER_H
#define APP_COMMUNICATION_INTERNAL_EVENT_PUB_MANAGER_H

#include "model/InternalEvent.h"
#include "model/SelfCheckErrorType.h"
#include "PublishOutManager.h"
#include "net/base/RequestModel.h"
#include "net/WsServerManager.h"
#include <mutex>

namespace internal_event {
    class InternalEventPubManager {
    private:
        std::mutex internal_mutex_;

        const AlarmEvent manual_back_to_base = AlarmEvent("alarm", 1001, "COR_101", "手动返回基站", "");
        const AlarmEvent urgency_stop = AlarmEvent("alarm", 1002, "COR_102", "急停", "");
        const AlarmEvent manual_new_task = AlarmEvent("alarm", 1003, "COR_103", "手动下发新任务", "");
        const AlarmEvent nebular_operation = AlarmEvent("alarm", 1004, "COR_104", "云端操作", "");
        const AlarmEvent cancel_urgency_stop = AlarmEvent("alarm", 1007, "COR_107", "解除急停", "");
        const AlarmEvent low_battery_back_charge = AlarmEvent("alarm", 1008, "COR_108", "低电量回充", "");
        const AlarmEvent enter_manual_clean_mode = AlarmEvent("alarm", 1009, "COR_109", "进入手动模式", "");
        const AlarmEvent clean_water_level_check_failed = AlarmEvent("alarm", 1010, "COR_110", "清水箱空", "");
        const AlarmEvent dirty_water_level_check_failed = AlarmEvent("alarm", 1011, "COR_111", "污水箱满", "");


        const AlarmEvent global_planner_create_failed = AlarmEvent("alarm", 3000, "COR_300",
                                                                   "无法创建global_planner，初始化失败",
                                                                   "");
        const AlarmEvent local_planner_create_failed = AlarmEvent("alarm", 3001, "COR_301",
                                                                  "无法创建local_planner, 初始化失败",
                                                                  "");
        const AlarmEvent illegal_quaternion = AlarmEvent("alarm", 3002, "COR_302", "目标点方向四元数非法，无法抵达", "");
        const AlarmEvent global_cost_map_get_failed = AlarmEvent("alarm", 3003, "COR_303",
                                                                 "全局路径规划器无法获得全局代价地图", "");
        const AlarmEvent get_current_pose_failed = AlarmEvent("alarm", 3004, "COR_304",
                                                              "全局路径获取机器人当前位姿失败，导致规划失败", "");
        const AlarmEvent global_plan_failed = AlarmEvent("alarm", 3005, "COR_305", "全局规划失败，无法计算出可用路径",
                                                         "");
        const AlarmEvent set_local_plan_failed = AlarmEvent("alarm", 3006, "COR_306", "局部规划器无法设置全局路径", "");
        const AlarmEvent local_control_failed = AlarmEvent("alarm", 3007, "COR_307", "局部规划失败，无法计算出可用速度",
                                                           "");
        const AlarmEvent planning_recovery_failed_and_skip = AlarmEvent("alarm", 3010, "COR_310",
                                                                        "全局规划脱困失败，跳点", "");
        const AlarmEvent local_control_failed_and_skip = AlarmEvent("alarm", 3011, "COR_311", "局部规划脱困失败，跳点",
                                                                    "");
        const AlarmEvent oscillation_recovery_failed_and_skip = AlarmEvent("alarm", 3012, "COR_312",
                                                                           "震荡脱困失败，跳点", "");
        const AlarmEvent sonar_recovery_failed_and_skip = AlarmEvent("alarm", 3013, "COR_313", "超声脱困失败，跳点", "");
        const AlarmEvent prohibition_recovery_failed_and_skip = AlarmEvent("alarm", 3014, "COR_314",
                                                                           "禁区脱困始终无法找到可达点",
                                                                           "");
        const AlarmEvent sonar_recovery_time_out = AlarmEvent("alarm", 3015, "COR_315", "超声脱困超时", "");


        const AlarmEvent camera1_alarm_event = AlarmEvent("alarm", 4001, "COR_401", "缺少点云数据1（上银牛）", "");
        const AlarmEvent camera2_alarm_event = AlarmEvent("alarm", 4002, "COR_402", "缺少点云数据2（下银牛）", "");
        const AlarmEvent imu_alarm_event = AlarmEvent("alarm", 4003, "COR_403", "缺少imu原始数据（imu_raw)", "");
        const AlarmEvent laser_scan_alarm_event = AlarmEvent("alarm", 4004, "COR_404", "缺少激光雷达原始数据", "");
        const AlarmEvent odom_alarm_event = AlarmEvent("alarm", 4005, "COR_405", "里程计数据发生跳变", "");
        const AlarmEvent ultraSonic1_alarm_event = AlarmEvent("alarm", 4006, "COR_406", "1号超声波连续误触", "");
        const AlarmEvent ultraSonic2_alarm_event = AlarmEvent("alarm", 4007, "COR_407", "2号超声波连续误触", "");
        const AlarmEvent bump1_alarm_event = AlarmEvent("alarm", 4008, "COR_408", "碰撞1连续误触", "");
        const AlarmEvent bump2_alarm_event = AlarmEvent("alarm", 4009, "COR_409", "碰撞2连续误触", "");
        const AlarmEvent bump3_alarm_event = AlarmEvent("alarm", 4010, "COR_410", "碰撞3连续误触", "");
        const AlarmEvent bump4_alarm_event = AlarmEvent("alarm", 4011, "COR_411", "碰撞4连续误触", "");
        const AlarmEvent bms_alarm_event = AlarmEvent("alarm", 4012, "COR_412", "电量异常跳变", "");
        const AlarmEvent bias_detect_alarm_event = AlarmEvent("alarm", 6001, "COR_601", "定位丢失", "");
        const AlarmEvent tracked_pose_alarm_event = AlarmEvent("alarm", 6002, "COR_602", "位置跳变", "");

        const AlarmEvent not_at_base_point = AlarmEvent("alarm", 5001, "COR_501", "开始回充不在摆渡点附近", "");
        const AlarmEvent out_station_error = AlarmEvent("alarm", 5010, "COR_502", "机器出站异常（发速度机器不动）", "");

        const AlarmEvent software_interrupt = AlarmEvent("alarm", 7001, "COR_701", "程序错误", "");
        //雷达重启相关
        const AlarmEvent laser_restart_start = AlarmEvent("alarm", 4014, "COR_414", "雷达电压不足", "");
        const AlarmEvent laser_restart_succeed = AlarmEvent("alarm", 4015, "COR_415", "雷达重启后数据恢复", "");
        const AlarmEvent laser_restart_failed = AlarmEvent("alarm", 3219, "CCR_219", "雷达连续重启失败任务提前结束",
                                                           "");

        const AlarmEvent lift_failed = AlarmEvent("alarm", 3332, "CCR_332", "机器处于电梯上", "");

        //尘推电机堵转
        const AlarmEvent motor_error_recovery_scceed = AlarmEvent("alarm", 4013, "COR_413", "尘推转速异常并成功恢复",
                                                                  "");
        const AlarmEvent motor_error_recovery_failed = AlarmEvent("alarm", 3218, "CCR_218", "尘推堵转，任务提前结束",
                                                                  "");
        const AlarmEvent mop_error_recovery_scceed = AlarmEvent("alarm", 4431, "COR_431", "湿拖堵转，任务提前结束", "");

        //底盘电机失能
        const AlarmEvent hls_error_1 = AlarmEvent("alarm", 4016, "COR_416", "电机失能，编码器故障ABZ报警", "");
        const AlarmEvent hls_error_2 = AlarmEvent("alarm", 4017, "COR_417", "电机失能，编码器故障UVW报警", "");
        const AlarmEvent hls_error_3 = AlarmEvent("alarm", 4018, "COR_418", "电机失能，位置超差", "");
        const AlarmEvent hls_error_4 = AlarmEvent("alarm", 4019, "COR_419", "电机失能，失速", "");
        const AlarmEvent hls_error_5 = AlarmEvent("alarm", 4020, "COR_420", "电机失能，电流采样（中点）故障", "");
        const AlarmEvent hls_error_6 = AlarmEvent("alarm", 4021, "COR_421", "电机失能，过载", "");
        const AlarmEvent hls_error_7 = AlarmEvent("alarm", 4022, "COR_422", "电机失能，欠压", "");
        const AlarmEvent hls_error_8 = AlarmEvent("alarm", 4023, "COR_423", "电机失能，过压", "");
        const AlarmEvent hls_error_9 = AlarmEvent("alarm", 4024, "COR_424", "电机失能，过流", "");
        const AlarmEvent hls_error_10 = AlarmEvent("alarm", 4025, "COR_425", "电机失能，放电报警瞬时功率大", "");
        const AlarmEvent hls_error_11 = AlarmEvent("alarm", 4026, "COR_426", "电机失能，放电回路频繁动作平均功率大", "");
        const AlarmEvent hls_error_12 = AlarmEvent("alarm", 4027, "COR_427", "电机失能，参数读写异常", "");
        const AlarmEvent hls_error_13 = AlarmEvent("alarm", 4028, "COR_428", "电机失能，输入口功能定义重复", "");
        const AlarmEvent hls_error_14 = AlarmEvent("alarm", 4029, "COR_429", "电机失能，通讯看门狗触发", "");
        const AlarmEvent hls_error_15 = AlarmEvent("alarm", 4030, "COR_430", "底盘电电机失能，电机过温报警机失能", "");

    public:
        static InternalEventPubManager *instance_;

        static InternalEventPubManager *get_instance() {
            if (instance_ == nullptr) {
                instance_ = new InternalEventPubManager;
            }
            return instance_;
        }

        void taskStart(std::string task_id) {
            StartEvent start_event("task_start", task_id);
            json json_result = start_event;
            std_msgs::String msg;
            msg.data.append(json_result.dump());
            {
                std::unique_lock<std::mutex> lock(internal_mutex_);
                PublishOutManager::instance().publishInternalEvent(msg);
            }

        }

        void taskStop(std::string task_id) {
            StartEvent start_event("task_stop", task_id);
            json json_result = start_event;
            std_msgs::String msg;
            msg.data.append(json_result.dump());
            {
                std::unique_lock<std::mutex> lock(internal_mutex_);
                PublishOutManager::instance().publishInternalEvent(msg);
            }
        }

        void workStatusUpdate(int event_code) {
            WorkStatusUpdate update_event("work_status_update", event_code);
            json json_result = update_event;
            std_msgs::String msg;
            msg.data.append(json_result.dump());
            {
                std::unique_lock<std::mutex> lock(internal_mutex_);
                PublishOutManager::instance().publishInternalEvent(msg);
            }
        }

        void pubAlarmEvent(const AlarmEvent &event) {
            PublishOutManager::instance().publishAlarm(event);

            json json_result = event;
            std_msgs::String msg;
            msg.data.append(json_result.dump());
            {
                std::unique_lock<std::mutex> lock(internal_mutex_);
                PublishOutManager::instance().publishInternalEvent(msg);
            }
        }

        void pubAlarm(SelfCheckErrorType error_type) {
            switch (error_type) {
                case CAMERA1_NO_DATA:
                    pubAlarmEvent(camera1_alarm_event);
                    break;
                case CAMERA2_NO_DATA:
                    pubAlarmEvent(camera2_alarm_event);
                    break;
                case IMU_NO_DATA:
                    pubAlarmEvent(imu_alarm_event);
                    break;
                case ODOM_NO_DATA:
                    pubAlarmEvent(odom_alarm_event);
                    break;
                case ULTRASONIC1_ABNORMAL_OVER_30_SECOND:
                    pubAlarmEvent(ultraSonic1_alarm_event);
                    break;
                case ULTRASONIC2_ABNORMAL_OVER_30_SECOND:
                    pubAlarmEvent(ultraSonic2_alarm_event);
                    break;
                case BUMP1_ABNORMAL_OVER_30_SECOND:
                    pubAlarmEvent(bump1_alarm_event);
                    break;
                case BUMP2_ABNORMAL_OVER_30_SECOND:
                    pubAlarmEvent(bump2_alarm_event);
                    break;
                case BUMP3_ABNORMAL_OVER_30_SECOND:
                    pubAlarmEvent(bump3_alarm_event);
                    break;
                case BUMP4_ABNORMAL_OVER_30_SECOND:
                    pubAlarmEvent(bump4_alarm_event);
                    break;
                case BMS_HOP:
                    pubAlarmEvent(bms_alarm_event);
                    break;
                    // case TRACKED_POSE_HOP:
                    //     pubAlarmEvent(tracked_pose_alarm_event);
                    //     break;
                case BIAS_DETECTED:
                    pubAlarmEvent(bias_detect_alarm_event);
                    break;
                case LASER_NO_DATA:
                    pubAlarmEvent(laser_scan_alarm_event);
                    break;
                    /////////////////////////////////////////////
                case GLOBAL_PLANNER_CREATE_FAILED:
                    pubAlarmEvent(global_planner_create_failed);
                    break;
                case LOCAL_PLANNER_CREATE_FAILED:
                    pubAlarmEvent(local_planner_create_failed);
                    break;
                case ILLEGAL_QUATERNION:
                    pubAlarmEvent(illegal_quaternion);
                    break;
                case GLOBAL_COST_MAP_GET_FAILED:
                    pubAlarmEvent(global_cost_map_get_failed);
                    break;
                case GET_CURRENT_POSE_FAILED:
                    pubAlarmEvent(get_current_pose_failed);
                    break;
                case GLOBAL_PLAN_FAILED:
                    pubAlarmEvent(global_plan_failed);
                    break;
                case SET_LOCAL_PLAN_FAILED:
                    pubAlarmEvent(set_local_plan_failed);
                    break;
                case LOCAL_CONTROL_FAILED:
                    pubAlarmEvent(local_control_failed);
                    break;
                case PLANNING_RECOVERY_FAILED_AND_SKIP:
                    pubAlarmEvent(planning_recovery_failed_and_skip);
                    break;
                case LOCAL_CONTROL_FAILED_AND_SKIP:
                    pubAlarmEvent(local_control_failed_and_skip);
                    break;
                case OSCILLATION_RECOVERY_FAILED_AND_SKIP:
                    pubAlarmEvent(oscillation_recovery_failed_and_skip);
                    break;
                case SONAR_RECOVERY_FAILED_AND_SKIP:
                    pubAlarmEvent(sonar_recovery_failed_and_skip);
                    break;
                case PROHIBITION_RECOVERY_FAILED_AND_SKIP:
                    pubAlarmEvent(prohibition_recovery_failed_and_skip);
                    break;
                case SONAR_RECOVERY_TIME_OUT:
                    pubAlarmEvent(sonar_recovery_time_out);
                    break;
                    /////////////////////
                case NOT_AT_BASE_POINT:
                    pubAlarmEvent(not_at_base_point);
                    break;
                case OUT_STATION_ERROR:
                    pubAlarmEvent(out_station_error);
                    break;
                case SOFTWARE_INTERRUPT:
                    pubAlarmEvent(software_interrupt);
                    break;
                case LASER_RESTART_START:
                    pubAlarmEvent(laser_restart_start);
                    break;
                case LASER_RESTART_SUCCEED:
                    pubAlarmEvent(laser_restart_succeed);
                    break;
                case LASER_RESTART_FAILED:
                    pubAlarmEvent(laser_restart_failed);
                    break;
                case LIFT_FAILED:
                    pubAlarmEvent(lift_failed);
                    break;
                case HLS_ERROR_1:
                    pubAlarmEvent(hls_error_1);
                    break;
                case HLS_ERROR_2:
                    pubAlarmEvent(hls_error_2);
                    break;
                case HLS_ERROR_3:
                    pubAlarmEvent(hls_error_3);
                    break;
                case HLS_ERROR_4:
                    pubAlarmEvent(hls_error_4);
                    break;
                case HLS_ERROR_5:
                    pubAlarmEvent(hls_error_5);
                    break;
                case HLS_ERROR_6:
                    pubAlarmEvent(hls_error_6);
                    break;
                case HLS_ERROR_7:
                    pubAlarmEvent(hls_error_7);
                    break;
                case HLS_ERROR_8:
                    pubAlarmEvent(hls_error_8);
                    break;
                case HLS_ERROR_9:
                    pubAlarmEvent(hls_error_9);
                    break;
                case HLS_ERROR_10:
                    pubAlarmEvent(hls_error_10);
                    break;
                case HLS_ERROR_11:
                    pubAlarmEvent(hls_error_11);
                    break;
                case HLS_ERROR_12:
                    pubAlarmEvent(hls_error_12);
                    break;
                case HLS_ERROR_13:
                    pubAlarmEvent(hls_error_13);
                    break;
                case HLS_ERROR_14:
                    pubAlarmEvent(hls_error_14);
                    break;
                case HLS_ERROR_15:
                    pubAlarmEvent(hls_error_15);
                    break;
                default:
                    break;
            }
        }

        void pubOperEvent(const OperEvent &event) {
            json json_result = event;
            std_msgs::String msg;
            msg.data.append(json_result.dump());
            {
                std::unique_lock<std::mutex> lock(internal_mutex_);
                PublishOutManager::instance().publishInternalEvent(msg);
            }
        }

        void pubOper(const int oper_type) {
            switch (oper_type) {
                case internal_event::MANUAL_BACK_TO_BASE:
                    pubAlarmEvent(manual_back_to_base);
                    break;
                case internal_event::URGENCY_STOP:
                    pubAlarmEvent(urgency_stop);
                    break;
                case internal_event::MANUAL_NEW_TASK:
                    pubAlarmEvent(manual_new_task);
                    break;
                case internal_event::NEBULAR_OPERATION:
                    pubAlarmEvent(nebular_operation);
                    break;
                case internal_event::CANCEL_URGENCY_STOP:
                    pubAlarmEvent(cancel_urgency_stop);
                    break;
                case internal_event::LOW_BATTERY_BACK_CHARGE:
                    pubAlarmEvent(low_battery_back_charge);
                    break;
                case internal_event::ENTER_MANUAL_CLEAN_MODE:
                    pubAlarmEvent(enter_manual_clean_mode);
                    break;
                case internal_event::CLEAN_WATER_LEVEL_CHECK_FAILED:
                    pubAlarmEvent(clean_water_level_check_failed);
                    break;
                case internal_event::DIRTY_WATER_LEVEL_CHECK_FAILED:
                    pubAlarmEvent(dirty_water_level_check_failed);
                    break;
                case internal_event::LASER_ERROR_RECOVERY_FAILED:
                    pubAlarmEvent(laser_restart_failed);
                    break;
                case internal_event::MOTOR_ERROR_RECOVERY_SCCEED:
                    pubAlarmEvent(motor_error_recovery_scceed);
                    break;
                case internal_event::MOTOR_ERROR_RECOVERY_FAILED:
                    pubAlarmEvent(motor_error_recovery_failed);
                    break;
                case internal_event::MOP_ERROR_RECOVERY_SCCEED:
                    pubAlarmEvent(mop_error_recovery_scceed);
                    break;
                default:
                    break;
            }
        }
    };
}// namespace internal_event

#endif//APP_COMMUNICATION_INTERNAL_EVENT_PUB_MANAGER_H