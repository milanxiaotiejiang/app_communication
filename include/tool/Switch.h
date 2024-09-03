#ifndef Switch
#define Switch

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <unistd.h>

enum MESSAGE_ID {
    GET_DEVICE_STATUS_ = 0,                 //获取机器状态
    START_MAP,                              //保存地图
    END_MAP,                                //保存地图
    GET_MULTI_MAPS_,                        //获取地图列表
    CHANGE_MAP_,                            //切换地图
    MODIFY_MAP_NAME,                        //修改地图名称
    DELETE_MAP,                             //删除地图

    EDIT_MAP_,                              //编辑地图禁行区（当前地图）
    GET_EDIT_MAP_,                          //获取地图禁行区（当前地图）
    MULTIPLE_EDIT_MAP,                      //编辑地图禁行区
    MULTIPLE_GET_EDIT_MAP,                  //获取地图禁行区

    RUNNING_TASK,

    APP_SPOT_,                              //继续清扫（与暂停对应）
    APP_PAUSE_,                             //暂停清扫
    APP_CHARGE_,                            //返回基站
    CHANGE_WORK_STATUS_,                    //改变清扫模式
    CHANGE_AROM_STATUS_,                    //改变香薰机状态

    TRY_TO_ENTER_,                          //尝试进入某种模式
    FORCED_TO_ENTER_,                       //强制进入某种模式
    WORK_TO_ENTER_,                         //打开规划器，进入工作模式
    WORK_TO_MAP_APP_,                       //工作模式请求地图

    GET_FINISHED_POINT_,                    //获取当前已经走过的点列
    IS_IN_BASEMENT_,                        //是否在基站

    GET_ROS_VERSION_,
    PAD_VERSION_INTO,

    GET_MACHINE_MODEL,

    GET_DEVICE_SECRET,
    COLLECT_DUST,

    ROOM_MAP_DATA,
    ROOM_MERGE,
    ROOM_SEGMENTATION,
    ROOM_RESET,
    ROOM_RENAME,
    ROOM_AUTO,
    REGION_EXPLORATION,
    GET_PLAN_PARAM,
    SET_PLAN_PARAM,
    RESET_PLAN_PARAM,

    OTA_CORE,
    OTA_PAD,
    OTA_LOWER,

    MANUAL_PUSH_START,
    MANUAL_PUSH_RESET,

    ENTER_MANUAL_MODE,                      //进入手动模式
    QUIT_MANUAL_MODE,                       //退出手动模式

    EMERGENCY_STOP,                         //急停
    RELEASE_EMERGENCY_STOP,                 //解急停

    UNRECOVERABLE_ERROR,

    SHUTDOWN,                               //关机
    REBOOT,                                 //重启

    GET_ROBOT_PARAMS,
    SET_ROBOT_PARAMS,
    GET_HOT_WIND_MODE,
    SET_HOT_WIND_MODE,

    GET_RAIN_SNOW,
    SET_RAIN_SNOW,

    GET_COLLECT_DUST,
    SET_COLLECT_DUST,
    GET_AUTO_OIL,
    SET_AUTO_OIL,
    GET_MAINTENANCE_START_TIME,
    SET_MAINTENANCE_START_TIME,
    GET_TEST_CLOUD_INTERACTIVE_ENVIRONMENT,
    SET_TEST_CLOUD_INTERACTIVE_ENVIRONMENT,

    MAP_OBSTACLES,                          //添加障碍物（当前地图）
    MAP_FEASIBLE_ZONE,                      //添加可行区（当前地图）
    MAP_APPLY_INCREASE_AREA,

    MULTIPLE_MAP_OBSTACLES,                 //添加障碍物（所有地图）
    MULTIPLE_MAP_FEASIBLE_ZONE,             //添加可行区（所有地图）

    SET_EXPLORER_ENERGY,
    GET_EXPLORER_ENERGY,

    AUTOMATIC_OILING,

    ADD_TASK,
    MULTIPLE_ADD_TASK,
    DELETE_TASK,                            //删除任务（当前地图、单个任务）
    DELETE_MULTIPLE_TASK,                   //删除多个任务（当前地图）
    MULTIPLE_DELETE_TASK,                   //删除任务（多地图、单个任务）
    LIST_TASK,                              //获取任务列表（当前地图）
    MULTIPLE_LIST_TASK,                     //获取任务列表（地图ID）
    MULTIPLE_WHOLE_LIST_TASK,               //获取任务列表（所有地图）
    QUERY_ID_TASK,
    ADD_DEFAULT_DELIVERY_TASK,

    BUILD_PRINCIPAL_TASK,                   //设置默认任务
    CANCEL_PRINCIPAL_TASK,                  //取消默认任务
    PRINCIPAL_TASK,                         //获取默认任务（当前地图）
    MULTIPLE_PRINCIPAL_TASK,                //获取默认任务（地图ID）

    BUILD_RAIN_SNOW_TASK,                   //设置雨雪任务
    CANCEL_RAIN_SNOW_TASK,                  //取消雨雪任务
    RAIN_SNOW_TASK,                         //获取雨雪任务（当前地图）
    MULTIPLE_RAIN_SNOW_TASK,                //获取雨雪任务（地图ID）

    CLEAR_CURRENT_LIST_TASK,

    MODIFY_TASK_NAME,
    MODIFY_TASK_RATE,
    MODIFY_TASK_WORK_STATUS,
    MODIFY_TASK_KNIFE,
    MODIFY_COMPLETE_TASK,                   //任务修改-单条全量数据
    OPERATE_ADD_ZONE,
    OPERATE_DELETE_ZONE,
    OPERATE_MODIFY_ZONE,
    MODIFY_TASK_PARTITION,
    OPERATE_ADD_SUBREGION,
    OPERATE_DELETE_SUBREGION,
    OPERATE_ADD_DELIVERY,
    OPERATE_DELETE_DELIVERY,
    OPERATE_MODIFY_DELIVERY,

    ADD_TIMER_TASK,                         //添加定时任务（当前地图）
    MULTIPLE_ADD_TIMER_TASK,                //添加定时任务（所有地图）
    DELETE_TIMER_TASK,                      //删除定时任务（当前地图、单个任务）
    DELETE_MULTIPLE_TIMER_TASK,             //删除多个定时任务（当前地图）
    MULTIPLE_DELETE_TIMER_TASK,             //删除定时任务（多地图，单个定时任务）
    LIST_TIMER_TASK,
    MULTIPLE_LIST_TIMER_TASK,               //地图ID的定时任务列表
    MULTIPLE_WHOLE_LIST_TIMER_TASK,         //设备的定时任务列表
    MODIFY_TIMER_TASK,
    MULTIPLE_MODIFY_TIMER_TASK,
    MODIFY_TIMER_NAME,
    LIST_TIMER_TASK_BUILD,

    EXPLORATION_TASK,
    PERFORM_TASK,

    GET_CONSUMABLE,
    RESET_CONSUMABLE,

    HOT_WIND_MODE,
    HOT_WIND_MODE_STATUS,
    MAINTENANCE_MODE,
    MAINTENANCE_MODE_STATUS,
    SET_BASE_STATION,
    GET_BASE_STATION,

    OPEN_SELF_CHECK,
    CLOSE_SELF_CHECK,

    FACTORY_RESET,

    ADD_GATE,                               //增加闸机（当前地图）
    DELETE_GATE,                            //根据闸机ID删除此闸机
    PURGE_GATE,                             //删除当前地图下方所有闸机
    MODIFY_GATE,                            //修改闸机（全量替换）
    LIST_GATE,                              //获取闸机列表（当前地图）
    QUERY_ID_GATE,                          //根据闸机ID查询详细信息
    MULTIPLE_MODIFY_GATE,                   //不切地图修改除点位外的其他信息
    MULTIPLE_LIST_GATE,                     //根据地图ID获取闸机列表
    MULTIPLE_PURGE_GATE,                    //根据地图ID删除下方所有闸机
    ADD_GATE_V2,                            //增加闸机
    MODIFY_GATE_V2,                         //修改闸机

    OPEN_GATE_SETTING,                      //启动闸机设置
    CLOSE_GATE_SETTING,                     //关闭闸机设置

    ADD_BUILD,                              //添加楼宇
    DELETE_BUILD,                           //根据id删除楼宇
    MODIFY_BUILD_NAME,                      //根据id修改楼宇名称
    MODIFY_BUILD_ELEVATOR_ADDRESS,
    LIST_BUILD,                             //查询所有楼宇

    MODIFY_MAP_BASE_STATION,                //修改地图是否有基站
    MODIFY_MAP_FLOOR,                       //修改地图楼层
    MODIFY_MAP_ELEVATOR,                    //修改地图中梯控点位开关
    MODIFY_MAP_ELEVATOR_POINT,              //修改地图中梯控点位
    MODIFY_MAP_ELEVATOR_RECT,

    LIST_MAP_FOR_BUILD,                     //根据楼宇id查询地图列表

    ATTACH_BUILD_MAP,                       //强制关联楼宇和地图

    MAP_FOR_ID,

    SEWAGE_PUMP_SWITCH,

    ABNORMAL,

    TT_ELEVATOR,

    ROBOT_MOVE,

    ID_MESSGAE_MAX_, //枚举类型中未定义的值会比前一个值大1
};

int switch_ID(const std::string &str);

pid_t getProcessPidByName(const char *proc_name);

#endif /*Switch*/