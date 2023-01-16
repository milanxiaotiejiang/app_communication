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


JsonSubscribeCloud::JsonSubscribeCloud(ros::NodeHandle handle, PubInner pubInner, PubOut pubOut)
        : handle(handle),
          pubInner(std::move(pubInner)),
          pubOut(std::move(pubOut)) {
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
//        case APP_ALONG_CLEAN_:
//            messageStrategy = new AppAlongCleanStrategy();
//            break;
//        case SAVE_MAP_:
//            messageStrategy = new SaveMapStrategy();
//            break;
        case GET_MULTI_MAPS_:
            messageStrategy = new GetMultiMapsStrategy();
            break;
//        case CHANGE_MAP_:
//            messageStrategy = new ChangeMapStrategy();
//            break;
//        case EDIT_MAP_:
//            messageStrategy = new EditMapStrategy();
//            break;
        case GET_EDIT_MAP_:
            messageStrategy = new GetEditMapStrategy();
            break;
//        case EXECUTE_TASK_:
//            messageStrategy = new ExecuteTaskStrategy();
//            break;
        case GET_TASK_LIST_:
            messageStrategy = new GetTaskListStrategyV2();
            break;
//        case GET_FINISHED_POINT_:
//            messageStrategy = new GetFinishedPointStrategy();
//            break;
        case APP_SPOT_:
            messageStrategy = new StatusResumeStrategy();
            break;
        case APP_PAUSE_:
            messageStrategy = new StatusPauseStrategy();
            break;
        case APP_CHARGE_:
            messageStrategy = new StatusChargeStrategy();
            break;
//        case CHANGE_WORK_STATUS_:
//            messageStrategy = new ChangeWorkModeStrategy();
//            break;
        case CHANGE_AROM_STATUS_:
            messageStrategy = new ChangeAromStatusStrategy();
            break;
//        case CONTINUITY_TASK_LIST_:
//            messageStrategy = new SetContinuityTaskStrategy();
//            break;
//        case POLYGON_TASK_LIST_:
//            messageStrategy = new SetPolygonTaskStrategy();
//            break;

//        case OPEN_SELF_CLEANING_:
//            messageStrategy = new SelfCleanStrategy();
//            break;
//        case ROBOT_RELOCATION_:
//            messageStrategy = new RobotRelocateStrategy();
//            break;

//        case TEACH_MODE_START_:
//            messageStrategy = new StartTeachModeStrategy();
//            break;
//        case TEACH_MODE_STOP_:
//            messageStrategy = new StopTeachModeStrategy();
//            break;
//        case TEACH_HEART_BEAT_:
//            messageStrategy = new HeartBeatofTeachModeStrategy();
//            break;
        case GET_TEACH_PATH_LIST_:
            messageStrategy = new GetTeachModeListStrategy();
            break;
        case GET_TEACH_PATH_DETAIL_:
            messageStrategy = new GetTeachModeDetialStrategy();
            break;
        case DELETE_TEACH_PATH_LIST_:
            messageStrategy = new DeleteTeachModeStrategy();
            break;
//        case GET_FULL_PLAN_:
//            messageStrategy = new GetFullPlanStrategy();
//            break;
//        case ROBOT_MOVE_:
//            messageStrategy = new RobotMoveStrategy();
//            break;

//        case TRY_TO_ENTER_:
//            messageStrategy = new RobotTryEnterModeStrategy();
//            break;
//        case FORCED_TO_ENTER_:
//            messageStrategy = new RobotForceEnterModeStrategy();
//            break;
//        case WORK_TO_ENTER_:
//            messageStrategy = new RobotPreparetoWorkStrategy();
//            break;
//        case WORK_TO_MAP_APP_:
//            messageStrategy = new MapPreparetoWorkStrategy();
//            break;
        case CLEAN_HISTORY_REQUEST_:
            messageStrategy = new GetCloudCleanHistoryStrategy();
            break;
//        case COMBINATION_PART_ADD_:
//            messageStrategy = new CombinationPartAddStrategy();
//            break;
//        case COMBINATION_COMBINATION_ADD_:
//            messageStrategy = new CombinationCombinationAddStartegy();
//            break;
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
//        case COMBINATION_PART_UPDATE_:
//            messageStrategy = new CombinationPartUpdateStrategy();
//            break;
//        case COMBINATION_COMBINATION_UPDATE_:
//            messageStrategy = new CombinationCombinationUpdateStrategy();
//            break;
        case IS_IN_BASEMENT_:
            messageStrategy = new IsInBasementStrategy();
            break;
        case GET_ROS_VERSION_:
            messageStrategy = new GetRosVersionStrategy();
            break;
//        case PLAY_VOICE_:
//            messageStrategy = new PlayerRecruitVoiceStrategy();
//            break;
//        case LIGHT_BELT_MODE_:
//            messageStrategy = new LightBeltModeStrategy();
//            break;
//        case OPEN_MACHINE_DRAWER_:
//            messageStrategy = new OpenMachineDrawerStrategy();
//            break;
//        case SET_POWER_REDUCTION:
//            messageStrategy = new PowerReductionStrategy();
//            break;


        case UPD_TIMER_:
            messageStrategy = new UpdateTimerStrategy();
            break;
        case SET_TIMER_:
            messageStrategy = new AddTimerStrategy();
            break;
        case GET_TIMER_LIST_:
            messageStrategy = new GetTimerListStrategy();
            break;
        case DEL_TIMER_:
            messageStrategy = new DeleteTimerStrategy();
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

        res.resp = PublishOutManager::instance().getPubOut()->robot_result;//pubOut.robot_result;
     //   LOG(ERROR) << "JsonSubscribeCloud method : " << res.resp;
        //pubOut.robot_result = "{}";
        PublishOutManager::instance().getPubOut()->robot_result = "{}";
        delete messageStrategy;
    }


    return true;
}
