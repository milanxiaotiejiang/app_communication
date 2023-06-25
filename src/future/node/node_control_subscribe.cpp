//
// Created by Looper on 2023/3/7.
//

#include "future/node/node_control_subscribe.h"
#include "future/node/node_control.h"
#include "task/manager/NodeWorkModeManager.h"

NodeControlSubscribe::NodeControlSubscribe(ros::NodeHandle handle) {
    sub_node_control_ = handle.subscribe("/node_control", 1, &NodeControlSubscribe::nodeControlSubscribeCallback, this);
}

void NodeControlSubscribe::nodeControlSubscribeCallback(const std_msgs::Int32 &flag_result) {
    auto flag = flag_result.data;
    LOG_IF(INFO, DEBUG_NODE) << "NodeControlSubscribe : " << flag;
    if (flag == WorkMode::SLEEPING) {
        NodeControl::instance().changeSleepMode();
    } else if (flag == WorkMode::WORKING) {
        NodeControl::instance().changeWorkMode();
    } else if (flag == WorkMode::MAPPING) {
        NodeControl::instance().changeMapMode();
    }
}
