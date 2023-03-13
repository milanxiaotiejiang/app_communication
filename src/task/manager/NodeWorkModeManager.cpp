//
// Created by admin1 on 22-11-25.
//

#include "task/manager/NodeWorkModeManager.h"
#include "future/node/node_control.h"

void NodeWorkModeManager::initialize(ros::NodeHandle handle) {
//    this->nodeHandle = handle;
    pub_node_ = handle.advertise<std_msgs::Int32>("/node_control", 1);
}

WorkMode NodeWorkModeManager::getWorkMode() {
    if (NodeControl::instance().isSleep()) {
        return WorkMode::SLEEPING;
    }
    if (NodeControl::instance().isWork()) {
        return WorkMode::WORKING;
    }
    if (NodeControl::instance().isMap()) {
        return WorkMode::MAPPING;
    }
    return WorkMode::UNKNOWN;
}

void NodeWorkModeManager::setWorkMode(WorkMode mode) {
    std_msgs::Int32 message;
    if (mode == WorkMode::SLEEPING) {
        message.data = 0;
        pub_node_.publish(message);
    } else if (mode == WorkMode::WORKING) {
        message.data = 1;
        pub_node_.publish(message);
    } else if (mode == WorkMode::MAPPING) {
        message.data = 2;
        pub_node_.publish(message);
    } else {
        LOG(INFO) << "NodeWorkModeManager setWorkMode unknown " << mode;
    }
//    nodeHandle.setParam("/node_controller/work_mode", mode);
}

void NodeWorkModeManager::enterWorkMode(int enter) {
    if (enter == 0) {
        NodeWorkModeManager::instance().setWorkMode(WorkMode::MAPPING);
    } else if (enter == 2) {
        NodeWorkModeManager::instance().setWorkMode(WorkMode::WORKING);
    } else {
        LOG(INFO) << "NodeWorkModeManager enterWorkMode unknown " << enter;
    }
}
