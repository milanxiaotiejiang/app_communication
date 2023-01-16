//
// Created by admin1 on 22-11-25.
//

#include "task/manager/NodeWorkModeManager.h"

void NodeWorkModeManager::initialize(ros::NodeHandle handle) {
    this->nodeHandle = handle;
}

WorkMode NodeWorkModeManager::getWorkMode() {
    int work_mode;
    nodeHandle.getParam("/node_controller/work_mode", work_mode);
    if (work_mode == 0) {
        workMode = WorkMode::MAPPING;
    } else if (work_mode == 1) {
        workMode = WorkMode::SLEEPING;
    } else if (work_mode == 2) {
        workMode = WorkMode::WORKING;
    }
    return workMode;
}

void NodeWorkModeManager::setWorkMode(int mode) {
    nodeHandle.setParam("/node_controller/work_mode", mode);
}
