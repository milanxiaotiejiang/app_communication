#ifndef Switch
#define Switch

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <unistd.h>

enum MESSAGE_ID {
    GET_DEVICE_STATUS_ = 0, //获取机器状态
    SAVE_MAP_,              //保存地图
    GET_MULTI_MAPS_,        //获取地图列表
    CHANGE_MAP_,            //切换地图
    MODIFY_MAP_NAME,        //修改地图名称
    DELETE_MAP,             //删除地图
    EDIT_MAP_,              // 编辑地图禁行区
    GET_EDIT_MAP_,          //获取地图禁行区

    EXECUTE_TASK_,          //发布任务
    GET_TASK_LIST_,         //获取任务列表
    RUNNING_TASK,

    APP_SPOT_,              //继续清扫（与暂停对应）
    APP_PAUSE_,             //暂停清扫
    APP_CHARGE_,            //返回基站
    CHANGE_WORK_STATUS_,    //改变清扫模式
    CHANGE_AROM_STATUS_,    //改变香薰机状态

    CONTINUITY_TASK_LIST_,     //连续区域清扫
    GET_CONTINUITY_TASK_LIST_, //获取连续区域清扫
    POLYGON_TASK_LIST_,        //设置多边形区域
    GET_POLYGON_TASK_LIST_,    //获取多边形区域

    TEACH_MODE_START_,       //开启示教
    TEACH_MODE_STOP_,        //关闭示教
    TEACH_HEART_BEAT_,       //示教心跳包
    GET_TEACH_PATH_LIST_,    //获取示教列表
    GET_TEACH_PATH_DETAIL_,  //获取示教详细点位
    DELETE_TEACH_PATH_LIST_, //删除示教
    GET_FULL_PLAN_,          //获取全局规划路径

    TRY_TO_ENTER_,          //尝试进入某种模式
    FORCED_TO_ENTER_,       //强制进入某种模式
    WORK_TO_ENTER_,         //打开规划器，进入工作模式
    WORK_TO_MAP_APP_,       //工作模式请求地图
    CLEAN_HISTORY_REQUEST_, //请求清扫历史纪录

    COMBINATION_PART_ADD_,        //添加单个part
    COMBINATION_COMBINATION_ADD_, //添加单个combination；

    COMBINATION_PART_LIST_,        //读取part列表
    COMBINATION_COMBINATION_LIST_, //读取combination列表

    COMBINATION_COMBINATION_DETAILS_, //读取combination详情

    COMBINATION_PART_DELETE_,        //删除单个part
    COMBINATION_PART_DELETE_FORCE_,  //强制删除单个part
    COMBINATION_COMBINATION_DELETE_, //删除单个combination

    COMBINATION_PART_UPDATE_,        //更新单个part
    COMBINATION_COMBINATION_UPDATE_, //更新单个combination
    GET_FINISHED_POINT_,             //获取当前已经走过的点列
    IS_IN_BASEMENT_,                 //是否在基站

    SET_TIMER_,      //设置清扫计时器
    GET_TIMER_LIST_, //获取清扫计时器列表
    DEL_TIMER_,      //删除单个定时器
    UPD_TIMER_,      //修改清扫计时器
    GET_ROS_VERSION_,
    SAVE_LOCATION,
    SAVE_PROJECT,
    GET_LOCATION,
    GET_PROJECT,
    PAD_VERSION_INTO,

    GET_MACHINE_MODEL,
    MAIN_COMBINATION_WAY,
    CANCEL_MAIN_COMBINATION,

    NOTICE_LIST,
    GET_DEVICE_SECRET,
    TT_ERROR_CHECK,
    KNOB_CONTROL,
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

    MAP_OBSTACLES,
    MAP_FEASIBLE_ZONE,
    MAP_APPLY_INCREASE_AREA,

    SET_EXPLORER_ENERGY,
    GET_EXPLORER_ENERGY,

    AUTOMATIC_OILING,

    ADD_TASK,
    DELETE_TASK,
    LIST_TASK,
    QUERY_ID_TASK,

    BUILD_PRINCIPAL_TASK,
    CANCEL_PRINCIPAL_TASK,
    PRINCIPAL_TASK,

    CLEAR_CURRENT_LIST_TASK,

    MODIFY_TASK_NAME,
    MODIFY_TASK_RATE,
    MODIFY_TASK_WORK_STATUS,
    MODIFY_TASK_KNIFE,
    OPERATE_ADD_ZONE,
    OPERATE_DELETE_ZONE,
    OPERATE_MODIFY_ZONE,
    MODIFY_TASK_PARTITION,
    OPERATE_ADD_SUBREGION,
    OPERATE_DELETE_SUBREGION,

    ADD_TIMER_TASK,
    DELETE_TIMER_TASK,
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

    ID_MESSGAE_MAX_, //枚举类型中未定义的值会比前一个值大1
};

int switch_ID(const std::string &str);

pid_t getProcessPidByName(const char *proc_name);

#endif /*Switch*/