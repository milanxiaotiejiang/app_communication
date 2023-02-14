//
// Created by Looper on 2022/11/29.
//

#include "task/manager/NativeSystemManager.h"

void NativeSystemManager::urgencyStop(bool isUrgencyStop) {
    asyncTaskCall->executeUrgencyStop(isUrgencyStop);
}

void NativeSystemManager::lowBatteryToBackBase() {
    asyncTaskCall->forceBackToBase(0);
}

void NativeSystemManager::urgencyStopAndCharge() {
    asyncTaskCall->urgencyStopAndCharge();
}

void NativeSystemManager::waterLevelToBackBase(int event) {
    asyncTaskCall->forceBackToBase(event);
}

void NativeSystemManager::motorErrorEvent(int error_event) {
    if (error_event == 1) {
        asyncTaskCall->recordMotorError();
    } else {
        asyncTaskCall->forceBackToBase(4);
    }
}
void NativeSystemManager::hlsErrorEvent(int error_event){
  if (error_event == 1) {
    asyncTaskCall->recordHlsError();
  } 
}


void NativeSystemManager::laserErrorEvent(std::string error_event) {
    if (error_event == "laser_scan_4016") {
        asyncTaskCall->executeUnrecoverableError();
    } else {
        asyncTaskCall->recordLaserError(error_event);
    }
}