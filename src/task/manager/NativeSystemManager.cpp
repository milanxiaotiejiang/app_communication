//
// Created by Looper on 2022/11/29.
//

#include "task/manager/NativeSystemManager.h"
#include "model/SelfCheckErrorType.h"
#include "manager/elevator_control.h"

void NativeSystemManager::urgencyStop(bool isUrgencyStop) {
    asyncTaskCall->executeUrgencyStop(isUrgencyStop);
    ElevatorControlManager::instance().setUrgencyStop(isUrgencyStop);
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
        asyncTaskCall->executeElectricMove();
    }
}

void NativeSystemManager::laserErrorEvent(const std::string &error_event) {
    if (error_event == "laser_scan_4016") {
        asyncTaskCall->executeUnrecoverableError();
    }
}