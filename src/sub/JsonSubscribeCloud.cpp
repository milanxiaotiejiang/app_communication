/*
* @Author: siyu.zhu
* @Date: 2022-05-18 05:45:28
* @Last Modified by: siyu.zhu
* @Last Modified time: 2022-05-18 06:04:30
*/

#include "sub/JsonSubscribeCloud.h"
#include "net/WsServerManager.h"
#include "sub/json/DeviceStrategy.h"
#include "sub/json/MapStrategy.h"
#include "sub/json/TaskStrategy.h"
#include <sub/json/GetCleanHistoryStrategy.h>
#include <sub/json/ModeStrategy.h>
#include <sub/json/StatusStrategy.h>
#include <sub/json/otaStrategy.h>
#include <utility>

#include "simulation.h"

#include "sub/json/MaterialStrategy.h"
#include "sub/json/CloudDeviceStrategy.h"
#include "sub/json/KnobControlStrategy.h"
#include "manager/cloud_robot_control.h"
#include "sub/json/DBTaskStrategy.h"
#include "exploration/ExplorationStrategy.h"
#include "sub/json/GateStrategy.h"


JsonSubscribeCloud::JsonSubscribeCloud(ros::NodeHandle handle) : handle(handle) {
    service = handle.advertiseService("robot_control_srv", &JsonSubscribeCloud::function, this);
}

JsonSubscribeCloud::~JsonSubscribeCloud() {
}

bool JsonSubscribeCloud::function(clean_msgs::robot_control::Request &req, clean_msgs::robot_control::Response &res) {

    json jdecodeMsg = json::parse(req.req);
    json jdecode = {};
    jdecode["topic"] = "robot_control_srv";
    jdecode["op"] = "publish";
    jdecode["msg"] = jdecodeMsg;
    // auto entrance = jdecode.get<Entrance>();
    auto requestModel = jdecode.get<RequestModel<Entrance>>();
    auto entrance = requestModel.getMsg();

//    LOG_IF(INFO, DEBUG_REQUEST) << "JsonSubscribeCloud method : " << entrance.getMethod();
    int start_time = ros::Time::now().sec;

    MessageBaseStrategy *messageStrategy = nullptr;

    switch (switch_ID(entrance.getMethod())) {
        case GET_DEVICE_STATUS_:
            messageStrategy = new GetDeviceStatusStrategyV2();
            break;
        case GET_MULTI_MAPS_:
            messageStrategy = new GetMultiMapsStrategy();
            break;

        case EDIT_MAP_:
            messageStrategy = new EditMapStrategy();
            break;
        case GET_EDIT_MAP_:
            messageStrategy = new GetEditMapStrategy();
            break;

        case APP_SPOT_:
            messageStrategy = new StatusResumeStrategy();
            break;
        case APP_PAUSE_:
            messageStrategy = new StatusPauseStrategy();
            break;
        case APP_CHARGE_:
            messageStrategy = new StatusChargeStrategy();
            break;
        case CHANGE_AROM_STATUS_:
            messageStrategy = new ChangeAromStatusStrategy();
            break;
        case CLEAN_HISTORY_REQUEST_:
            messageStrategy = new GetCloudCleanHistoryStrategy();
            break;
        case IS_IN_BASEMENT_:
            messageStrategy = new IsInBasementStrategy();
            break;
        case GET_ROS_VERSION_:
            messageStrategy = new GetRosVersionStrategy();
            break;

        case OTA_CORE:
        case OTA_PAD:
        case OTA_LOWER:
            messageStrategy = new otaStrategy();
            break;

            //20230509
        case ADD_TASK:
            messageStrategy = new AddTaskStrategy();
            break;
        case DELETE_TASK:
            messageStrategy = new DeleteTaskStrategy();
            break;
        case DELETE_MULTIPLE_TASK:
            messageStrategy = new DeleteMultipleTaskStrategy();
            break;
        case LIST_TASK:
            messageStrategy = new ListTaskStrategy();
            break;
        case QUERY_ID_TASK:
            messageStrategy = new QueryIdTaskStrategy();
            break;

        case BUILD_PRINCIPAL_TASK:
            messageStrategy = new BuildPrincipalTaskStrategy();
            break;
        case CANCEL_PRINCIPAL_TASK:
            messageStrategy = new CancelPrincipalTaskStrategy();
            break;
        case PRINCIPAL_TASK:
            messageStrategy = new PrincipalTaskStrategy();
            break;

        case BUILD_RAIN_SNOW_TASK:
            messageStrategy = new BuildRainSnowTaskStrategy();
            break;
        case CANCEL_RAIN_SNOW_TASK:
            messageStrategy = new CancelRainSnowTaskStrategy();
            break;

        case CLEAR_CURRENT_LIST_TASK:
            messageStrategy = new ClearCurrentListTaskStrategy();
            break;

        case MODIFY_TASK_NAME:
            messageStrategy = new ModifyTaskNameStrategy();
            break;
        case MODIFY_TASK_RATE:
            messageStrategy = new ModifyTaskRateStrategy();
            break;
        case MODIFY_TASK_WORK_STATUS:
            messageStrategy = new ModifyTaskWorkStatusStrategy();
            break;
        case MODIFY_TASK_KNIFE:
            messageStrategy = new ModifyTaskKnifeStrategy();
            break;
        case MODIFY_COMPLETE_TASK:
            messageStrategy = new ModifyCompleteTaskStrategy();
            break;
        case OPERATE_ADD_ZONE:
            messageStrategy = new OperateAddZoneStrategy();
            break;
        case OPERATE_DELETE_ZONE:
            messageStrategy = new OperateDeleteZoneStrategy();
            break;
        case OPERATE_MODIFY_ZONE:
            messageStrategy = new OperateModifyZoneStrategy();
            break;
        case MODIFY_TASK_PARTITION:
            messageStrategy = new ModifyTaskPartitionStrategy();
            break;
        case OPERATE_ADD_SUBREGION:
            messageStrategy = new OperateAddSubregionStrategy();
            break;
        case OPERATE_DELETE_SUBREGION:
            messageStrategy = new OperateDeleteSubregionStrategy();
            break;

        case ADD_TIMER_TASK:
            messageStrategy = new AddTimerTaskStrategy();
            break;
        case DELETE_TIMER_TASK:
            messageStrategy = new DeleteTimerTaskStrategy();
            break;
        case DELETE_MULTIPLE_TIMER_TASK:
            messageStrategy = new DeleteMultipleTimerTaskStrategy();
            break;
        case LIST_TIMER_TASK:
            messageStrategy = new ListTimerTaskStrategy();
            break;
        case MODIFY_TIMER_TASK:
            messageStrategy = new ModifyTimerTaskStrategy();
            break;
        case MODIFY_TIMER_NAME:
            messageStrategy = new ModifyTimerNameStrategy();
            break;

        case EXPLORATION_TASK:
            messageStrategy = new ExplorationTaskStrategy();
            break;
        case PERFORM_TASK:
            messageStrategy = new PerformTaskStrategy();
            break;

        case GET_RAIN_SNOW:
            messageStrategy = new GetRainSnowStrategy();
            break;
        case SET_RAIN_SNOW:
            messageStrategy = new SetRainSnowStrategy();
            break;

        case START_MAP:
            messageStrategy = new StartMapStrategy();
            break;
        case END_MAP:
            messageStrategy = new EndMapStrategy();
            break;
        case MAP_FEASIBLE_ZONE:
            messageStrategy = new MapFeasibleZoneStrategy();
            break;
        case MANUAL_PUSH_START:
            messageStrategy = new ManualPushStartStrategy();
            break;
        case MANUAL_PUSH_RESET:
            messageStrategy = new ManualPushResetStrategy();
            break;
        case TRY_TO_ENTER_:
            messageStrategy = new RobotTryEnterModeStrategy();
            break;
        case FORCED_TO_ENTER_:
            messageStrategy = new RobotForceEnterModeStrategy();
            break;
        case WORK_TO_ENTER_:
            messageStrategy = new RobotPreparetoWorkStrategy();
            break;
        case WORK_TO_MAP_APP_:
            messageStrategy = new MapPreparetoWorkStrategy();
            break;

        case ADD_GATE:
            messageStrategy = new AddGateStrategy();
            break;
        case DELETE_GATE:
            messageStrategy = new DeleteGateStrategy();
            break;
        case PURGE_GATE:
            messageStrategy = new PurgeGateStrategy();
            break;
        case MODIFY_GATE:
            messageStrategy = new ModifyGateStrategy();
            break;
        case LIST_GATE:
            messageStrategy = new ListGateStrategy();
            break;
        case QUERY_ID_GATE:
            messageStrategy = new QueryIdGateStrategy();
            break;

        case MULTIPLE_MODIFY_GATE:
            messageStrategy = new MultipleModifyGateStrategy();
            break;
        case MULTIPLE_LIST_GATE:
            messageStrategy = new MultipleListGateStrategy();
            break;
        case MULTIPLE_PURGE_GATE:
            messageStrategy = new MultiplePurgeGateStrategy();
            break;
    }
    if (messageStrategy != nullptr) {

        MessageContext messageContext = MessageContext(messageStrategy);

        messageContext.startDateProgressing(MessageSource::Cloud, jdecode);

        int end_time = ros::Time::now().sec;
//        LOG_IF(INFO, DEBUG_REQUEST) << "----------------" << "JsonSubscribeCloud end : " << entrance.getMethod() << " "
//                                    << end_time - start_time << " s " << "----------------";

        res.resp = CloudRobotControl::instance().useInfo();
        CloudRobotControl::instance().reset();
        delete messageStrategy;
    }


    return true;
}
