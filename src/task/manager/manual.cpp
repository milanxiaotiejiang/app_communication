//
// Created by admin1 on 22-11-25.
//

#include "task/manager/manual.h"
#include "manager/PublishInnerManager.h"
#include "task/manager/MechanismManager.h"

void ManualManager::backToBase(bool force) {
    asyncTaskCall->manualBackToBase(force);
}

void ManualManager::resume() {
    asyncTaskCall->manualResume();
}

void ManualManager::pause() {
    asyncTaskCall->manualPause();
}

void ManualManager::enter_manul_mode() {
    asyncTaskCall->enterManual();
}

void ManualManager::quit_manual_mode() {
    asyncTaskCall->quitManual();
}

void ManualManager::shutdown() {
    std_msgs::Int32 msg;
    msg.data = 1;
    MechanismManager::instance().resetWorkStatus();
    sleep(1);
    PublishInnerManager::instance().publishShutDown(msg);
}

void ManualManager::reboot() {
    std_msgs::Int32 msg;
    msg.data = 1;
    MechanismManager::instance().resetWorkStatus();
    sleep(1);
    PublishInnerManager::instance().publishReboot(msg);
}

bool ManualManager::taskRunning() {
    return asyncTaskCall->taskRunning();
}

RealTask ManualManager::runningTask() const {
    return asyncTaskCall->runningTask();
}

std::vector<RealTask> ManualManager::runTaskList() {
    return asyncTaskCall->runTaskList();
}

std::vector<PointProgressVo> ManualManager::runTaskPointList() {
    return asyncTaskCall->runTaskPointList();
}
