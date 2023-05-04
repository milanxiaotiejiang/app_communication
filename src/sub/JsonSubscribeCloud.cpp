/*
* @Author: siyu.zhu
* @Date: 2022-05-18 05:45:28
* @Last Modified by: siyu.zhu
* @Last Modified time: 2022-05-18 06:04:30
*/

#include "sub/JsonSubscribeCloud.h"
#include "net/WsServerManager.h"
#include "sub/json/TTStrategy.h"
#include "sub/json/CombinationStartegy.h"
#include "sub/json/DeviceStrategy.h"
#include "sub/json/MapStrategy.h"
#include "sub/json/TaskStrategy.h"
#include "sub/json/TeachModeStrategy.h"
#include "sub/json/TimerStrategy.h"
#include "sub/json/ViewPartStrategy.h"
#include <sub/json/GetCleanHistoryStrategy.h>
#include <sub/json/ModeStrategy.h>
#include <sub/json/StatusStrategy.h>
#include <sub/json/otaStrategy.h>
#include <utility>

#include "glog/logging.h"
#include "sub/json/LocationStrategy.h"
#include "sub/json/ProjectStrategy.h"

#include "sub/json/MaterialStrategy.h"
#include "sub/json/NoticeStrategy.h"
#include "sub/json/CloudDeviceStrategy.h"
#include "sub/json/KnobControlStrategy.h"
#include "manager/cloud_robot_control.h"


JsonSubscribeCloud::JsonSubscribeCloud(ros::NodeHandle handle) : handle(handle) {
    // sub_json_ = handle.subscribe(APP_JSON_V2, 1, &JsonSubscribeCloud::subscribeCallback, this);
    service = handle.advertiseService("robot_control_srv", &JsonSubscribeCloud::function,
                                      this);//写明服务的处理函数 handle_function cloud_srvs是service的名称
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
    // LOG(ERROR) << "JsonSubscribeCloud method : " << entrance.getMethod();

    MessageBaseStrategy *messageStrategy = nullptr;

    switch (switch_ID(entrance.getMethod())) {
        case GET_DEVICE_STATUS_:
            messageStrategy = new GetDeviceStatusStrategyV2();
            break;
        case GET_MULTI_MAPS_:
            messageStrategy = new GetMultiMapsStrategy();
            break;
        case GET_EDIT_MAP_:
            messageStrategy = new GetEditMapStrategy();
            break;
        case GET_TASK_LIST_:
            messageStrategy = new GetTaskListStrategyV2();
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
        case COMBINATION_PART_LIST_:
            messageStrategy = new CombinationPartListStrategyV2();
            break;
        case COMBINATION_COMBINATION_LIST_:
            messageStrategy = new CombinationCombinationListStrategyV2();
            break;
        case COMBINATION_COMBINATION_DETAILS_:
            messageStrategy = new CombinationCombinationDetailsStrategyV2();
            break;
        case COMBINATION_PART_DELETE_:
            messageStrategy = new CombinationPartDeleteStrategy();
            break;
        case COMBINATION_PART_DELETE_FORCE_:
            messageStrategy = new CombinationPartDeleteForceStrategy();
            break;
        case COMBINATION_COMBINATION_DELETE_:
            messageStrategy = new CombinationCombinationDeleteStrategy();
            break;
        case IS_IN_BASEMENT_:
            messageStrategy = new IsInBasementStrategy();
            break;
        case GET_ROS_VERSION_:
            messageStrategy = new GetRosVersionStrategy();
            break;

        case UPD_TIMER_:
            messageStrategy = new UpdateTimerStrategy();
            break;
        case SET_TIMER_:
            messageStrategy = new SetTimerStrategy();
            break;
        case GET_TIMER_LIST_:
            messageStrategy = new GetTimerListStrategy();
            break;
        case DEL_TIMER_:
            messageStrategy = new DelTimerStrategy();
            break;
        case SAVE_LOCATION:
            messageStrategy = new LocationStrategy();
            break;
        case GET_LOCATION:
            messageStrategy = new getLocationStrategy();
            break;
        case OTA_CORE:
        case OTA_PAD:
        case OTA_LOWER:
            messageStrategy = new otaStrategy();
            break;

    }
    if (messageStrategy != nullptr) {

        MessageContext messageContext = MessageContext(messageStrategy);

        messageContext.startDateProgressing(MessageSource::Cloud, jdecode);

        res.resp = CloudRobotControl::instance().useInfo();
        CloudRobotControl::instance().reset();
        delete messageStrategy;
    }


    return true;
}
