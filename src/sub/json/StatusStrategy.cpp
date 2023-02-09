//
// Created by lijiang on 2021/12/20.
//

#include "sub/json/StatusStrategy.h"
#include "task/manager/manual.h"
#include "task/manager/NativeSystemManager.h"

void StatusPauseStrategy::handler() {
    ManualManager::instance().pause();
}

void StatusResumeStrategy::handler() {
    ManualManager::instance().resume();
}

void StatusChargeStrategy::handler() {
    ManualManager::instance().backToBase(false);
}

void EnterManualStrategy::handler() {
    ManualManager::instance().enter_manul_mode();
}

void QuitManualStrategy::handler() {
    ManualManager::instance().quit_manual_mode();
}

void ShutDownStrategy::handler() {
    ManualManager::instance().shutdown();
}

void RebootStrategy::handler() {
    ManualManager::instance().reboot();
}

void EmergencyStopStrategy::handler() {
    ZooInnerStatus::instance().setUrgencyStopStatus(true);
}

void ReleaseEmergencyStopStrategy::handler() {
    ZooInnerStatus::instance().setUrgencyStopStatus(false);
}

void UnrecoverableErrorStrategy::handler() {
    NativeSystemManager::instance().motorErrorEvent(2);
}
