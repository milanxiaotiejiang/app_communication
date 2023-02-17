//
// Created by Looper on 2023/2/14.
//

#include "task/call/reserved_call.h"
#include "clean_history/CleanHistoryCenter.h"
#include "manager/InternalEventPubManager.h"

using namespace internal_event;
using namespace clean_history_db;

void ReservedCall::handleManualOperation() {
    switch (epoll_manual) {
        case loop::manual_epoll::manual_back:
        case loop::manual_epoll::manual_force_back:
            InternalEventPubManager::get_instance()->pubOper(MANUAL_BACK_TO_BASE);
            CleanHistoryCenter::instance().manualBack();
            break;
        case loop::manual_epoll::manual_task_over:
            break;
        default:
            break;
    }
    AsyncTaskCall::handleManualOperation();
}

void ReservedCall::handleSpecialOperation() {
    switch (epoll_special) {
        case loop::special_epoll::special_low_battery:
            InternalEventPubManager::get_instance()->pubOper(LOW_BATTERY_BACK_CHARGE);
            CleanHistoryCenter::instance().lowPowerBack();
            break;
        case loop::special_epoll::special_branch_water: {
            InternalEventPubManager::get_instance()->pubOper(CLEAN_WATER_LEVEL_CHECK_FAILED);
            SpecialInfo si;
            si.clean_water_level_check_failed_ = true;
            CleanHistoryCenter::instance().equipmentErrorBack(si);
            break;
        }
        case loop::special_epoll::special_sewage_water: {
            InternalEventPubManager::get_instance()->pubOper(DIRTY_WATER_LEVEL_CHECK_FAILED);
            SpecialInfo si;
            si.dirty_water_level_check_failed_ = true;
            CleanHistoryCenter::instance().equipmentErrorBack(si);
            break;
        }
        case loop::special_epoll::special_branch_sewage_water: {
            InternalEventPubManager::get_instance()->pubOper(CLEAN_WATER_LEVEL_CHECK_FAILED);
            InternalEventPubManager::get_instance()->pubOper(DIRTY_WATER_LEVEL_CHECK_FAILED);
            SpecialInfo si;
            si.clean_water_level_check_failed_ = true;
            si.dirty_water_level_check_failed_ = true;
            CleanHistoryCenter::instance().equipmentErrorBack(si);
            break;
        }
        case loop::special_epoll::special_dust_push_anomaly:
            InternalEventPubManager::get_instance()->pubOper(MOTOR_ERROR_RECOVERY_FAILED);
            break;
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
        case loop::error_epoll::error_unrecoverable:
            InternalEventPubManager::get_instance()->pubAlarm(SelfCheckErrorType::LASER_RESTART_FAILED);
            CleanHistoryCenter::instance().laserInterrupt();
            break;
        default:
            LOG(INFO) << "AsyncTaskCall handleErrorOperation : " << epoll_error << " ...";
            break;
    }
    AsyncTaskCall::handleErrorOperation();
}

void ReservedCall::handleStop() {
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
    AsyncTaskCall::handleStop();
}

void ReservedCall::handleExecuteTask(const RealTask &task) {
    InternalEventPubManager::get_instance()->taskStart(task.getId());
    CleanHistoryCenter::instance().executeTask(task);
    AsyncTaskCall::handleExecuteTask(task);
}

void ReservedCall::handleFlowPoint(const RealPoint &point) {
    if (point.getId() == FLOW_SEIZE_SEAT) {
        setFlow(event::flow::out_base_station);
    } else if (point.getId() == FLOW_OUT_STATION) {
        CleanHistoryCenter::instance().setOutStation(point.realError.arrive ? SUCCEED : FAIL);
    } else if (point.getId() == FLOW_END_SLEEP) {
        CleanHistoryCenter::instance().setEndSleep(point.realError.arrive ? SUCCEED : FAIL);
    } else if (point.getId() == FLOW_IN_BASE_POINT) {
        CleanHistoryCenter::instance().setBackBasePointArrived(
                point.realError.arrive ? SUCCEED :
                (backBaseRetryCount < MAX_BASE_POINT_RETRY_COUNT ? (int) backBaseRetryCount : FAIL));
    } else if (point.getId() == FLOW_IN_STATION) {
        CleanHistoryCenter::instance().setStationArrived(
                point.realError.arrive ? SUCCEED :
                (rechargeRetryCount < MAX_RECHARGE_RETRY_COUNT) ? (int) rechargeRetryCount : FAIL);
    } else if (point.getId() == FLOW_CLOSE_MECHANISM) {
        CleanHistoryCenter::instance().setCloseMechanism(point.realError.arrive ? SUCCEED : FAIL);
    } else if (point.getId() == FLOW_OPEN_MECHANISM) {
        CleanHistoryCenter::instance().setOpenMechanism(point.realError.arrive ? SUCCEED : FAIL);
    }
    HeadTailPointCall::handleFlowPoint(point);
}

void ReservedCall::processControl(const RealPoint &point) {
    HeadTailPointCall::processControl(point);
}

void ReservedCall::handlePlannerPoint(const RealPoint &point) {
    //当前进度和清洁面积更新到历史记录中
    CleanHistoryCenter::instance().updateCleanHistory(point);

    AsyncTaskCall::handlePlannerPoint(point);
}

void ReservedCall::softwareInterruptTask(const RealPoint &point) {
    auto error_pair = generateErrorByRealPoint(point);
    CleanHistoryCenter::instance().errorComplete(
            std::get<0>(error_pair), std::get<1>(error_pair), std::get<2>(error_pair)
    );
    garbage();
}

void ReservedCall::goodGame() {
    runTask;
    InternalEventPubManager::get_instance()->taskStop(runTask.getId());
    CleanHistoryCenter::instance().complete();
    AsyncTaskCall::goodGame();
}

void ReservedCall::garbage() {
    InternalEventPubManager::get_instance()->pubAlarm(SelfCheckErrorType::SOFTWARE_INTERRUPT);
    InternalEventPubManager::get_instance()->taskStop(runTask.getId());
    AsyncTaskCall::garbage();
}

std::tuple<int, std::string, std::string> ReservedCall::generateErrorByRealPoint(const RealPoint &real_point) {
    std::string error_string;
    int error_code;
    std::string error_code2;
    switch (real_point.getId()) {
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
        default:
            error_string = "未知错误";
            error_code = 3200 - real_point.getId();
            std::string base_string = "CCR_";
            std::string flow_string = to_string(200 - real_point.getId());
            error_code2 = base_string + flow_string;

            break;
    }
    return make_tuple(error_code, error_string, error_code2);
}

void ReservedCall::recordMotorError() {
    InternalEventPubManager::get_instance()->pubOper(MOTOR_ERROR_RECOVERY_SCCEED);
}

void ReservedCall::recordHlsError() {
    InternalEventPubManager::get_instance()->pubAlarm(SelfCheckErrorType::HLS_ERROR);
}

void ReservedCall::recordLaserError(std::string error_event) {
    if (error_event == "laser_scan_4014") {
        InternalEventPubManager::get_instance()->pubAlarm(SelfCheckErrorType::LASER_RESTART_START);
    } else if (error_event == "laser_scan_4015") {
        InternalEventPubManager::get_instance()->pubAlarm(SelfCheckErrorType::LASER_RESTART_SUCCEED);
    }
}


