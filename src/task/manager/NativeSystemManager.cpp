//
// Created by Looper on 2022/11/29.
//

#include "task/manager/NativeSystemManager.h"
#include "model/SelfCheckErrorType.h"
#include "manager/InternalEventPubManager.h"

void NativeSystemManager::urgencyStop(bool isUrgencyStop) {
    asyncTaskCall->executeUrgencyStop(isUrgencyStop);
}

void NativeSystemManager::lowBatteryToBackBase() {
    asyncTaskCall->forceBackToBase(loop::special_epoll::special_low_battery);
}

void NativeSystemManager::urgencyStopAndCharge() {
    asyncTaskCall->urgencyStopAndCharge();
}

void NativeSystemManager::waterLevelToBackBase(loop::special_epoll operation) {
    asyncTaskCall->forceBackToBase(operation);
}

void NativeSystemManager::motorErrorEvent(int error_event) {
    if (error_event == 1) {
        internal_event::InternalEventPubManager::get_instance()->pubOper(internal_event::MOTOR_ERROR_RECOVERY_SCCEED);
    } else {
        asyncTaskCall->forceBackToBase(loop::special_epoll::special_dust_push_anomaly);
    }
}

void NativeSystemManager::wetMopErrorEvent(int error_event) {
    if (error_event == 1) {
//        internal_event::InternalEventPubManager::get_instance()->pubOper(internal_event::MOP_ERROR_RECOVERY_SCCEED);
        asyncTaskCall->forceBackToBase(loop::special_epoll::special_wet_tow_anomaly);
    }
}

void NativeSystemManager::hlsErrorEvent(int error_event) {
    if (error_event > 0) {
        switch (error_event) {
            case 1:
                internal_event::InternalEventPubManager::get_instance()->pubAlarm(SelfCheckErrorType::HLS_ERROR_1);
                break;
            case 2:
                internal_event::InternalEventPubManager::get_instance()->pubAlarm(SelfCheckErrorType::HLS_ERROR_2);
                break;
            case 3:
                internal_event::InternalEventPubManager::get_instance()->pubAlarm(SelfCheckErrorType::HLS_ERROR_3);
                break;
            case 4:
                internal_event::InternalEventPubManager::get_instance()->pubAlarm(SelfCheckErrorType::HLS_ERROR_4);
                break;
            case 5:
                internal_event::InternalEventPubManager::get_instance()->pubAlarm(SelfCheckErrorType::HLS_ERROR_5);
                break;
            case 6:
                internal_event::InternalEventPubManager::get_instance()->pubAlarm(SelfCheckErrorType::HLS_ERROR_6);
                break;
            case 7:
                internal_event::InternalEventPubManager::get_instance()->pubAlarm(SelfCheckErrorType::HLS_ERROR_7);
                break;
            case 8:
                internal_event::InternalEventPubManager::get_instance()->pubAlarm(SelfCheckErrorType::HLS_ERROR_8);
                break;
            case 9:
                internal_event::InternalEventPubManager::get_instance()->pubAlarm(SelfCheckErrorType::HLS_ERROR_9);
                break;
            case 10:
                internal_event::InternalEventPubManager::get_instance()->pubAlarm(SelfCheckErrorType::HLS_ERROR_10);
                break;
            case 11:
                internal_event::InternalEventPubManager::get_instance()->pubAlarm(SelfCheckErrorType::HLS_ERROR_11);
                break;
            case 12:
                internal_event::InternalEventPubManager::get_instance()->pubAlarm(SelfCheckErrorType::HLS_ERROR_12);
                break;
            case 13:
                internal_event::InternalEventPubManager::get_instance()->pubAlarm(SelfCheckErrorType::HLS_ERROR_13);
                break;
            case 14:
                internal_event::InternalEventPubManager::get_instance()->pubAlarm(SelfCheckErrorType::HLS_ERROR_14);
                break;
            case 15:
                internal_event::InternalEventPubManager::get_instance()->pubAlarm(SelfCheckErrorType::HLS_ERROR_15);
                break;
        }
//    InternalEventPubManager::get_instance()->pubAlarm(SelfCheckErrorType::HLS_ERROR);

        asyncTaskCall->executeElectricMove();
    }
}

void NativeSystemManager::laserErrorEvent(const std::string &error_event) {
    if (error_event == "laser_scan_4016") {
        asyncTaskCall->executeUnrecoverableError();
    } else if (error_event == "laser_scan_4014") {
        internal_event::InternalEventPubManager::get_instance()->pubAlarm(SelfCheckErrorType::LASER_RESTART_START);
    } else if (error_event == "laser_scan_4015") {
        internal_event::InternalEventPubManager::get_instance()->pubAlarm(SelfCheckErrorType::LASER_RESTART_SUCCEED);
    }
}