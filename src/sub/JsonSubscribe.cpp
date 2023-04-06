//
// Created by lijiang on 2021/12/18.
//

#include "sub/JsonSubscribe.h"

#include "net/WsServerManager.h"
#include "sub/json/TTStrategy.h"
#include "sub/json/CombinationStartegy.h"
#include "sub/json/DeviceStrategy.h"
#include "sub/json/MapStrategy.h"
#include "sub/json/ProjectStrategy.h"
#include "sub/json/TaskStrategy.h"
#include "sub/json/TeachModeStrategy.h"
#include "sub/json/TimerStrategy.h"
#include "sub/json/ViewPartStrategy.h"
#include <sub/json/GetCleanHistoryStrategy.h>
#include <sub/json/LocationStrategy.h>
#include <sub/json/ModeStrategy.h>
#include <sub/json/StatusStrategy.h>
#include "sub/json/MaterialStrategy.h"
#include "sub/json/NoticeStrategy.h"
#include "sub/json/CloudDeviceStrategy.h"
#include "sub/json/KnobControlStrategy.h"
#include "sub/json/FullCleaningModeStrategy.h"
#include "exploration/ExplorationStrategy.h"
#include "segmentation/SegmentationStrategy.h"
#include <utility>

#include "glog/logging.h"
#include "simulation.h"
#include "sub/json/DBTaskStrategy.h"

JsonSubscribe::JsonSubscribe(ros::NodeHandle handle) : handle(handle) {
    sub_json_ = handle.subscribe(APP_JSON, 3, &JsonSubscribe::subscribeCallback, this);
}

JsonSubscribe::~JsonSubscribe() {}

void JsonSubscribe::subscribeCallback(const std_msgs::String &result) {
    LOG(INFO) << "subscribeCallback : " << syscall(SYS_gettid) << " " << result.data;

    json jDecode = json::parse(result.data);

    auto requestModel = jDecode.get<RequestModel<Entrance>>();
    auto entrance = requestModel.getMsg();

    MessageBaseStrategy *messageStrategy = nullptr;

//    LOG(INFO) << "----------------" << "JsonSubscribe start : " << entrance.getMethod() << "----------------";
    int start_time = ros::Time::now().sec;

    switch (switch_ID(entrance.getMethod())) {
        case GET_DEVICE_STATUS_:
            messageStrategy = new GetDeviceStatusStrategy();
            break;
        case APP_ALONG_CLEAN_:
            messageStrategy = new AppAlongCleanStrategy();
            break;
        case SAVE_MAP_:
            messageStrategy = new SaveMapStrategy();
            break;
        case GET_MULTI_MAPS_:
            messageStrategy = new GetMultiMapsStrategy();
            break;
        case CHANGE_MAP_:
            messageStrategy = new ChangeMapStrategy();
            break;
        case EDIT_MAP_:
            messageStrategy = new EditMapStrategy();
            break;
        case GET_EDIT_MAP_:
            messageStrategy = new GetEditMapStrategy();
            break;
        case EXECUTE_TASK_:
            messageStrategy = new ExecuteTaskStrategy();
            break;
        case GET_TASK_LIST_:
            messageStrategy = new GetTaskListStrategy();
            break;
        case GET_FINISHED_POINT_:
            messageStrategy = new GetFinishedPointStrategy();
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
        case CHANGE_WORK_STATUS_:
            messageStrategy = new ChangeWorkModeStrategy();
            break;
        case CHANGE_AROM_STATUS_:
            messageStrategy = new ChangeAromStatusStrategy();
            break;
        case OPEN_SELF_CLEANING_:
            messageStrategy = new SelfCleanStrategy();
            break;

        case TEACH_MODE_START_:
            messageStrategy = new StartTeachModeStrategy();
            break;
        case TEACH_MODE_STOP_:
            messageStrategy = new StopTeachModeStrategy();
            break;
        case TEACH_HEART_BEAT_:
            messageStrategy = new HeartBeatofTeachModeStrategy();
            break;
        case GET_TEACH_PATH_LIST_:
            messageStrategy = new GetTeachModeListStrategy();
            break;
        case GET_TEACH_PATH_DETAIL_:
            messageStrategy = new GetTeachModeDetialStrategy();
            break;
        case DELETE_TEACH_PATH_LIST_:
            messageStrategy = new DeleteTeachModeStrategy();
            break;

        case GET_FULL_PLAN_:
            messageStrategy = new GetFullPlanStrategy();
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
        case CLEAN_HISTORY_REQUEST_:
            messageStrategy = new GetCleanHistoryStrategy();
            break;
        case COMBINATION_PART_ADD_:
            messageStrategy = new CombinationPartAddStrategy();
            break;
        case COMBINATION_COMBINATION_ADD_:
            messageStrategy = new CombinationCombinationAddStartegy();
            break;
        case COMBINATION_PART_LIST_:
            messageStrategy = new CombinationPartListStrategy();
            break;
        case COMBINATION_COMBINATION_LIST_:
            messageStrategy = new CombinationCombinationListStrategy();
            break;
        case COMBINATION_COMBINATION_DETAILS_:
            messageStrategy = new CombinationCombinationDetailsStrategy();
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
        case COMBINATION_PART_UPDATE_:
            messageStrategy = new CombinationPartUpdateStrategy();
            break;
        case COMBINATION_COMBINATION_UPDATE_:
            messageStrategy = new CombinationCombinationUpdateStrategy();
            break;
        case IS_IN_BASEMENT_:
            messageStrategy = new IsInBasementStrategy();
            break;
        case GET_ROS_VERSION_:
            messageStrategy = new GetRosVersionStrategy();
            break;
        case PLAY_VOICE_:
            messageStrategy = new PlayerRecruitVoiceStrategy();
            break;
        case LIGHT_BELT_MODE_:
            messageStrategy = new LightBeltModeStrategy();
            break;
        case OPEN_MACHINE_DRAWER_:
            messageStrategy = new OpenMachineDrawerStrategy();
            break;
        case SET_POWER_REDUCTION:
            messageStrategy = new PowerReductionStrategy();
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
        case SAVE_PROJECT:
            messageStrategy = new ProjectStrategy();
            break;
        case PAD_VERSION_INTO:
            messageStrategy = new PadVersionStrategy();
            break;
        case GET_MACHINE_MODEL:
            messageStrategy = new MachineModelStrategy();
            break;
        case MAIN_COMBINATION_WAY:
            messageStrategy = new CombinationMainStrategy();
            break;
        case GET_MATERIAL_STATUS:
            messageStrategy = new GetMaterialStrategy();
            break;
        case CANCEL_MAIN_COMBINATION:
            messageStrategy = new CancelCombinationMainStrategy();
            break;
        case NOTICE_LIST:
            messageStrategy = new NoticeListStrategy();
            break;
        case GET_DEVICE_SECRET:
            messageStrategy = new GetDeviceSecretStrategy();
            break;
        case TT_ERROR_CHECK:
            messageStrategy = new TTErrorCheck();
            break;
        case KNOB_CONTROL:
            messageStrategy = new KnobControlStrategy();
            break;
        case ROOM_MAP_DATA:
            messageStrategy = new RoomMapDataStrategy();
            break;
        case ROOM_MERGE:
            messageStrategy = new RoomMergeStrategy();
            break;
        case ROOM_SEGMENTATION:
            messageStrategy = new RoomSegmentationStrategy();
            break;
        case ROOM_RESET:
            messageStrategy = new RoomResetStrategy();
            break;
        case ROOM_RENAME:
            messageStrategy = new RoomRenameStrategy();
            break;
        case ROOM_AUTO:
            messageStrategy = new AutoSegmentationStrategy();
            break;
        case REGION_EXPLORATION:
            messageStrategy = new ExplorationRoomStrategy();
            break;
        case GET_PLAN_PARAM:
            messageStrategy = new PlanParamGetStrategy();
            break;
        case SET_PLAN_PARAM:
            messageStrategy = new PlanParamSetStrategy();
            break;
        case RESET_PLAN_PARAM:
            messageStrategy = new PlanParamResetStrategy();
            break;
        case MANUAL_PUSH_START:
            messageStrategy = new ManualPushStartStrategy();
            break;
        case MANUAL_PUSH_RESET:
            messageStrategy = new ManualPushResetStrategy();
            break;
        case MANUAL_PUSH_SAVE:
            messageStrategy = new ManualPushSaveStrategy();
            break;
        case GET_FULL_CLEANING_MODE:
            messageStrategy = new GetFullCleaningModeStrategy();
            break;
        case SET_FULL_CLEANING_MODE:
            messageStrategy = new SetFullCleaningModeStrategy();
            break;
        case GET_FULL_CLEAN_LIST:

        case FULL_CLEANING_ADD:
            messageStrategy = new FullCLeaningAddStrategy();
            break;
        case FULL_CLEANING_UPDATE:
            messageStrategy = new FullCleaningUpdateStrategy();
            break;
        case FULL_CLEANING_DELETE:
            messageStrategy = new FullCLeaningDeleteStrategy();
            break;
        case FULL_CLEANING_DETAIL:
            messageStrategy = new FullCleaningDetailsStrategy();
            break;
        case FULL_CLEANING_MAIN:
            messageStrategy = new FullCleaningMainStrategy();
            break;
        case FULL_CLEANING_MAIN_CANCEL:
            messageStrategy = new CancelFullCleaningMainStrategy();
            break;
        case ENTER_MANUAL_MODE:
            messageStrategy = new EnterManualStrategy();
            break;
        case QUIT_MANUAL_MODE:
            messageStrategy = new QuitManualStrategy();
            break;
        case EMERGENCY_STOP:
            messageStrategy = new EmergencyStopStrategy();
            break;
        case RELEASE_EMERGENCY_STOP:
            messageStrategy = new ReleaseEmergencyStopStrategy();
            break;
        case UNRECOVERABLE_ERROR:
            messageStrategy = new UnrecoverableErrorStrategy();
            break;
        case SHUTDOWN:
            messageStrategy = new ShutDownStrategy();
            break;
        case REBOOT:
            messageStrategy = new RebootStrategy();
            break;
        case GET_ROBOT_PARAMS:
            messageStrategy = new GetRobotParamsStrategy();
            break;
        case SET_ROBOT_PARAMS:
            messageStrategy = new SetRobotParamsStrategy();
            break;
        case GET_HOT_WIND_MODE:
            messageStrategy = new GetHotWindModeStrategy();
            break;
        case SET_HOT_WIND_MODE:
            messageStrategy = new SetHotWindModeStrategy();
            break;
        case MAP_OBSTACLES:
            messageStrategy = new MapObstaclesStrategy();
            break;
        case MAP_FEASIBLE_ZONE:
            messageStrategy = new MapFeasibleZoneStrategy();
            break;
        case SET_EXPLORER_ENERGY:
            messageStrategy = new SetExplorerEnergyStrategy();
            break;
        case GET_EXPLORER_ENERGY:
            messageStrategy = new GetExplorerEnergyStrategy();
            break;
        case AUTOMATIC_OILING:
            messageStrategy = new AutomaticOilingStrategy();
            break;

        case ADD_TASK:
            messageStrategy = new AddTaskStrategy();
            break;
        case DELETE_TASK:
            messageStrategy = new DeleteTaskStrategy();
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

        case ADD_TIMER_TASK:
            messageStrategy = new AddTimerTaskStrategy();
            break;
        case DELETE_TIMER_TASK:
            messageStrategy = new DeleteTimerTaskStrategy();
            break;
        case LIST_TIMER_TASK:
            messageStrategy = new ListTimerTaskStrategy();
            break;

        case EXPLORATION_TASK:
            messageStrategy = new ExplorationTaskStrategy();
            break;
        case PERFORM_TASK:
            messageStrategy = new PerformTaskStrategy();
            break;

        case GET_CONSUMABLE:
            messageStrategy = new GetConsumableStrategy();
            break;
        case RESET_CONSUMABLE:
            messageStrategy = new ResetConsumableStrategy();
            break;
    }
    if (messageStrategy != nullptr) {
        MessageContext messageContext = MessageContext(messageStrategy);
        messageContext.startDateProgressing(MessageSource::WebSocket, jDecode);
        delete messageStrategy;
    }

    int end_time = ros::Time::now().sec;
    LOG(INFO) << "----------------" << "JsonSubscribe end : " << entrance.getMethod() << " "
              << end_time - start_time << " s " << "----------------";

}
