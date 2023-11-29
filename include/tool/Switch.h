#ifndef Switch
#define Switch

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <unistd.h>

enum MESSAGE_ID {
    GET_DEVICE_STATUS_ = 0, //获取机器状态
    START_MAP,              //保存地图
    END_MAP,              //保存地图
    GET_MULTI_MAPS_,        //获取地图列表
    CHANGE_MAP_,            //切换地图
    MODIFY_MAP_NAME,        //修改地图名称
    DELETE_MAP,             //删除地图
    EDIT_MAP_,              // 编辑地图禁行区
    GET_EDIT_MAP_,          //获取地图禁行区

    RUNNING_TASK,

    APP_SPOT_,              //继续清扫（与暂停对应）
    APP_PAUSE_,             //暂停清扫
    APP_CHARGE_,            //返回基站
    CHANGE_WORK_STATUS_,    //改变清扫模式
    CHANGE_AROM_STATUS_,    //改变香薰机状态

    TRY_TO_ENTER_,          //尝试进入某种模式
    FORCED_TO_ENTER_,       //强制进入某种模式
    WORK_TO_ENTER_,         //打开规划器，进入工作模式
    WORK_TO_MAP_APP_,       //工作模式请求地图
    CLEAN_HISTORY_REQUEST_, //请求清扫历史纪录

    GET_FINISHED_POINT_,             //获取当前已经走过的点列
    IS_IN_BASEMENT_,                 //是否在基站

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

    ENTER_MANUAL_MODE,//进入手动模式
    QUIT_MANUAL_MODE,//退出手动模式

    EMERGENCY_STOP,//急停
    RELEASE_EMERGENCY_STOP,//解急停

    UNRECOVERABLE_ERROR,

    SHUTDOWN,//关机
    REBOOT,//重启

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

    MAP_OBSTACLES,
    MAP_FEASIBLE_ZONE,
    MAP_APPLY_INCREASE_AREA,

    SET_EXPLORER_ENERGY,
    GET_EXPLORER_ENERGY,

    AUTOMATIC_OILING,

    ADD_TASK,
    DELETE_TASK,
    DELETE_MULTIPLE_TASK,
    LIST_TASK,
    QUERY_ID_TASK,

    BUILD_PRINCIPAL_TASK,
    CANCEL_PRINCIPAL_TASK,
    PRINCIPAL_TASK,

    BUILD_RAIN_SNOW_TASK,
    CANCEL_RAIN_SNOW_TASK,
    RAIN_SNOW_TASK,

    CLEAR_CURRENT_LIST_TASK,

    MODIFY_TASK_NAME,
    MODIFY_TASK_RATE,
    MODIFY_TASK_WORK_STATUS,
    MODIFY_TASK_KNIFE,
    MODIFY_COMPLETE_TASK,
    OPERATE_ADD_ZONE,
    OPERATE_DELETE_ZONE,
    OPERATE_MODIFY_ZONE,
    MODIFY_TASK_PARTITION,
    OPERATE_ADD_SUBREGION,
    OPERATE_DELETE_SUBREGION,

    ADD_TIMER_TASK,
    DELETE_TIMER_TASK,
    DELETE_MULTIPLE_TIMER_TASK,
    LIST_TIMER_TASK,
    MODIFY_TIMER_TASK,
    MODIFY_TIMER_NAME,

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

    OPEN_GATE_SETTING,                      //启动闸机设置
    CLOSE_GATE_SETTING,                     //关闭闸机设置



    ID_MESSGAE_MAX_, //枚举类型中未定义的值会比前一个值大1
};

int switch_ID(const std::string &str);

pid_t getProcessPidByName(const char *proc_name);

#endif /*Switch*/