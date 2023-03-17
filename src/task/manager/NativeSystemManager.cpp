//
// Created by Looper on 2022/11/29.
//

#include "task/manager/NativeSystemManager.h"

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
        asyncTaskCall->recordMotorError();
    } else {
        asyncTaskCall->forceBackToBase(loop::special_epoll::special_dust_push_anomaly);
    }
}

void NativeSystemManager::hlsErrorEvent(int error_event) {
    if (error_event == 1) {
        asyncTaskCall->recordHlsError();
    }
}

void NativeSystemManager::laserErrorEvent(const std::string &error_event) {
    if (error_event == "laser_scan_4016") {
        asyncTaskCall->executeUnrecoverableError();
    } else {
        asyncTaskCall->recordLaserError(error_event);
    }
}