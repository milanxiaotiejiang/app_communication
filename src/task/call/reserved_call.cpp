//
// Created by Looper on 2023/2/14.
//

#include "task/call/reserved_call.h"
#include "clean_history/CleanHistoryCenter.h"
#include "manager/InternalEventPubManager.h"
#include "db/property_data_base.h"

using namespace internal_event;
using namespace clean_history_db;

void ReservedCall::handleManualOperation() {
    switch (epoll_manual) {
        case loop::manual_epoll::manual_back:
        case loop::manual_epoll::manual_force_back:
            InternalEventPubManager::get_instance()->pubOper(MANUAL_BACK_TO_BASE);
            CleanHistoryCenter::instance().manualBack();
            break;
        case loop::manual_epoll::manual_task_over: {
            std::string error_string = "任务手动打断";
            int error_code = 3226;
            std::string error_code2 = "CCR_226";
            make_tuple(error_code, error_string, error_code2);
            CleanHistoryCenter::instance().successComplete(error_code, error_string, error_code2);
            break;
        }
        default:
            break;
    }
    AsyncTaskCall::handleManualOperation();
}

void ReservedCall::handleSpecialOperation() {
    switch (epoll_special) {
        case loop::special_epoll::special_low_battery: {
            InternalEventPubManager::get_instance()->pubOper(LOW_BATTERY_BACK_CHARGE);
            CleanHistoryCenter::instance().lowPowerBack();
            break;
        }
        case loop::special_epoll::special_branch_water: {
            InternalEventPubManager::get_instance()->pubOper(CLEAN_WATER_LEVEL_CHECK_FAILED);
            CleanHistoryCenter::instance().equipmentErrorBack(true, false, false);
            break;
        }
        case loop::special_epoll::special_sewage_water: {
            InternalEventPubManager::get_instance()->pubOper(DIRTY_WATER_LEVEL_CHECK_FAILED);
            CleanHistoryCenter::instance().equipmentErrorBack(false, true, false);
            break;
        }
        case loop::special_epoll::special_branch_sewage_water: {
            InternalEventPubManager::get_instance()->pubOper(CLEAN_WATER_LEVEL_CHECK_FAILED);
            InternalEventPubManager::get_instance()->pubOper(DIRTY_WATER_LEVEL_CHECK_FAILED);
            CleanHistoryCenter::instance().equipmentErrorBack(true, true, false);
            break;
        }
        case loop::special_epoll::special_dust_push_anomaly: {
            InternalEventPubManager::get_instance()->pubOper(MOTOR_ERROR_RECOVERY_FAILED);
            CleanHistoryCenter::instance().equipmentErrorBack(false, false, true);
            break;
        }
        default:
            break;
    }
    AsyncTaskCall::handleSpecialOperation();
}

void ReservedCall::handleErrorOperation() {
    switch (epoll_error) {
        case loop::error_epoll::error_manual_clean_start:
            if (isRegularTask(currentFlow())) {
                InternalEventPubManager::get_instance()->pubOper(ENTER_MANUAL_CLEAN_MODE);
                CleanHistoryCenter::instance().enterManualCleanMode();
            }
            break;
        case loop::error_epoll::error_manual_clean_end:
            break;
        case loop::error_epoll::error_lift:
            InternalEventPubManager::get_instance()->pubAlarm(SelfCheckErrorType::LIFT_FAILED);
            break;
        case loop::error_epoll::error_unrecoverable:
            InternalEventPubManager::get_instance()->pubAlarm(SelfCheckErrorType::LASER_RESTART_FAILED);
            CleanHistoryCenter::instance().laserInterrupt();
            break;
        default:
            LOG_IF(INFO, DEBUG_TASK) << "AsyncTaskCall handleErrorOperation : " << epoll_error << " ...";
            break;
    }
    AsyncTaskCall::handleErrorOperation();
}

void ReservedCall::handleStop() {
    if (first_urgency_stop) {
        first_urgency_stop = false;
    } else {
        switch (urgency_stop) {
            case loop::urgency_stop::trigger_urgency_stop:
                CleanHistoryCenter::instance().addUrgencyStop();//历史记录增加，急停一次
                InternalEventPubManager::get_instance()->pubOper(URGENCY_STOP);
                break;
            case loop::urgency_stop::recovery_urgency_stop:
                break;
            case loop::urgency_stop::release_urgency_stop:
                CleanHistoryCenter::instance().cancelUrgencyStop();
                InternalEventPubManager::get_instance()->pubOper(CANCEL_URGENCY_STOP);
                break;
            default:
                break;
        }
    }
    AsyncTaskCall::handleStop();
}

void ReservedCall::handleExecuteTask(const RealTask &task) {
    notifier.triggerTaskStart(task);
    InternalEventPubManager::get_instance()->taskStart(task.getId());
    CleanHistoryCenter::instance().executeTask(task);
    AsyncTaskCall::handleExecuteTask(task);
}

void ReservedCall::handleFlowBlock(const RealBlock &block) {
    if (block.id == FLOW_SEIZE_SEAT) {
        setFlow(event::flow::out_base_station);
    } else if (block.id == FLOW_OUT_STATION) {
        CleanHistoryCenter::instance().setOutStation(block.arrive ? SUCCEED : FAIL);
    } else if (block.id == FLOW_END_SLEEP) {
        CleanHistoryCenter::instance().setEndSleep(block.arrive ? SUCCEED : FAIL);
    } else if (block.id == FLOW_IN_BASE_POINT) {
        CleanHistoryCenter::instance().setBackBasePointArrived(
                block.arrive ? SUCCEED :
                (backBaseRetryCount < MAX_BASE_POINT_RETRY_COUNT ? (int) backBaseRetryCount : FAIL));
    } else if (block.id == FLOW_IN_STATION) {
        CleanHistoryCenter::instance().setStationArrived(
                block.arrive ? SUCCEED :
                (rechargeRetryCount < MAX_RECHARGE_RETRY_COUNT) ? (int) rechargeRetryCount : FAIL);
    } else if (block.id == FLOW_CLOSE_MECHANISM) {
        CleanHistoryCenter::instance().setCloseMechanism(block.arrive ? SUCCEED : FAIL);
    } else if (block.id == FLOW_OPEN_MECHANISM) {
        CleanHistoryCenter::instance().setOpenMechanism(block.arrive ? SUCCEED : FAIL);
    }
    HeadTailPointCall::handleFlowBlock(block);
}

void ReservedCall::processControl(const RealBlock &block) {
    HeadTailPointCall::processControl(block);
}

void ReservedCall::handlePlannerBlock(const RealBlock &block) {
    if (block.id < 0) {
        return;
    }
    if (block.taskId.empty()) {
        return;
    }
    auto plannerPoints = block.plannerPoints;
    if (plannerPoints.empty()) {
        return;
    }
    int current_step = block.already_step + block.timely_step;
    if (current_step > plannerPoints.size()) {
        return;
    }
    auto point = plannerPoints[current_step];

    //当前进度和清洁面积更新到历史记录中
    CleanHistoryCenter::instance().updateCleanHistory(block, point);

    AsyncTaskCall::handlePlannerBlock(block);
}

void ReservedCall::feedBackPose(const geometry_msgs::Pose &pose) {
    notifier.triggerTaskProgress(pose);
}

void ReservedCall::forceInterruptTask(event::SB sb) {
    int errorId = FLOW_ERROR_UNRECOVERABLE;
    switch (epoll_error) {
        case loop::error_epoll::error_lift:
            errorId = FLOW_ERROR_LIFT;
            break;
    }
    auto error_pair = generateErrorByRealPoint(errorId);
    CleanHistoryCenter::instance().errorComplete(
            std::get<0>(error_pair), std::get<1>(error_pair), std::get<2>(error_pair)
    );

    AsyncTaskCall::forceInterruptTask(sb);

    garbage(sb);
}

void ReservedCall::softwareInterruptTask(const RealBlock &block) {
    auto error_pair = generateErrorByRealPoint(block.id);
    CleanHistoryCenter::instance().errorComplete(
            std::get<0>(error_pair), std::get<1>(error_pair), std::get<2>(error_pair)
    );

    garbage(event::SB::sb_software);
}

void ReservedCall::goodGame(event::GG gg) {
    notifier.triggerTaskEnd();
    runTask;
    InternalEventPubManager::get_instance()->taskStop(runTaskId());
    CleanHistoryCenter::instance().complete();
    AsyncTaskCall::goodGame(gg);
}

void ReservedCall::garbage(event::SB sb) {
    InternalEventPubManager::get_instance()->pubAlarm(SelfCheckErrorType::SOFTWARE_INTERRUPT);
    InternalEventPubManager::get_instance()->taskStop(runTaskId());
    AsyncTaskCall::garbage(sb);
}

std::tuple<int, std::string, std::string> ReservedCall::generateErrorByRealPoint(int errorId) {
    std::string error_string;
    int error_code;
    std::string error_code2;
    switch (errorId) {
        case FLOW_SEIZE_SEAT:
            error_string = "默认状态下出错";
            error_code = 3210;
            error_code2 = "CCR_210";
            break;
        case FLOW_OPEN_MECHANISM:
            error_string = "开启清洁机构失败";
            error_code = 3211;
            error_code2 = "CCR_211";
            break;
        case FLOW_CLOSE_MECHANISM:
            error_string = "关闭清洁机失败";
            error_code = 3212;
            error_code2 = "CCR_212";
            break;
        case FLOW_OUT_STATION:
            error_string = "出站时失败";
            error_code = 3213;
            error_code2 = "CCR_213";
            break;
        case FLOW_END_SLEEP:
            error_string = "结束睡眠模式失败";
            error_code = 3214;
            error_code2 = "CCR_214";
            break;
        case FLOW_IN_BASE_POINT:
            error_string = "返回基站摆渡点失败";
            error_code = 3215;
            error_code2 = "CCR_215";
            break;
        case FLOW_IN_STATION:
            error_string = "基站对接失败";
            error_code = 3216;
            error_code2 = "CCR_216";
            break;
        case FLOW_ERROR_LIFT:
            error_string = "触发电梯";
            error_code = 3332;
            error_code2 = "CCR_332";
            break;
        case FLOW_ERROR_UNRECOVERABLE:
            error_string = "未知错误";
            error_code = 3220;
            error_code2 = "CCR_220";
            break;
        default:
            error_string = "未知错误";
            error_code = 3200 - errorId;
            std::string base_string = "CCR_";
            std::string flow_string = std::to_string(200 - errorId);
            error_code2 = base_string + flow_string;

            break;
    }
    return make_tuple(error_code, error_string, error_code2);
}

void ReservedCall::recordMotorError() {
    InternalEventPubManager::get_instance()->pubOper(MOTOR_ERROR_RECOVERY_SCCEED);
}

void ReservedCall::recordMopError() {
    InternalEventPubManager::get_instance()->pubOper(MOP_ERROR_RECOVERY_SCCEED);
}

void ReservedCall::recordHlsError(int error_event) {
    switch (error_event) {
        case 1:
            InternalEventPubManager::get_instance()->pubAlarm(SelfCheckErrorType::HLS_ERROR_1);
            break;
        case 2:
            InternalEventPubManager::get_instance()->pubAlarm(SelfCheckErrorType::HLS_ERROR_2);
            break;
        case 3:
            InternalEventPubManager::get_instance()->pubAlarm(SelfCheckErrorType::HLS_ERROR_3);
            break;
        case 4:
            InternalEventPubManager::get_instance()->pubAlarm(SelfCheckErrorType::HLS_ERROR_4);
            break;
        case 5:
            InternalEventPubManager::get_instance()->pubAlarm(SelfCheckErrorType::HLS_ERROR_5);
            break;
        case 6:
            InternalEventPubManager::get_instance()->pubAlarm(SelfCheckErrorType::HLS_ERROR_6);
            break;
        case 7:
            InternalEventPubManager::get_instance()->pubAlarm(SelfCheckErrorType::HLS_ERROR_7);
            break;
        case 8:
            InternalEventPubManager::get_instance()->pubAlarm(SelfCheckErrorType::HLS_ERROR_8);
            break;
        case 9:
            InternalEventPubManager::get_instance()->pubAlarm(SelfCheckErrorType::HLS_ERROR_9);
            break;
        case 10:
            InternalEventPubManager::get_instance()->pubAlarm(SelfCheckErrorType::HLS_ERROR_10);
            break;
        case 11:
            InternalEventPubManager::get_instance()->pubAlarm(SelfCheckErrorType::HLS_ERROR_11);
            break;
        case 12:
            InternalEventPubManager::get_instance()->pubAlarm(SelfCheckErrorType::HLS_ERROR_12);
            break;
        case 13:
            InternalEventPubManager::get_instance()->pubAlarm(SelfCheckErrorType::HLS_ERROR_13);
            break;
        case 14:
            InternalEventPubManager::get_instance()->pubAlarm(SelfCheckErrorType::HLS_ERROR_14);
            break;
        case 15:
            InternalEventPubManager::get_instance()->pubAlarm(SelfCheckErrorType::HLS_ERROR_15);
            break;
    }
//    InternalEventPubManager::get_instance()->pubAlarm(SelfCheckErrorType::HLS_ERROR);
}

void ReservedCall::recordLaserError(std::string error_event) {
    if (error_event == "laser_scan_4014") {
        InternalEventPubManager::get_instance()->pubAlarm(SelfCheckErrorType::LASER_RESTART_START);
    } else if (error_event == "laser_scan_4015") {
        InternalEventPubManager::get_instance()->pubAlarm(SelfCheckErrorType::LASER_RESTART_SUCCEED);
    }
}



