//
// Created by Looper on 2023/3/7.
//

#include "future/node/node_control_subscribe.h"
#include "future/node/node_control.h"
#include "task/manager/NodeWorkModeManager.h"

NodeControlSubscribe::NodeControlSubscribe(ros::NodeHandle handle) {
    sub_flag_ = handle.subscribe("/node_control", 1, &NodeControlSubscribe::subscribeCallback, this);
}

void NodeControlSubscribe::subscribeCallback(const std_msgs::Int32 &flag_result) {
    auto flag = flag_result.data;
    LOG(INFO) << "NodeControlSubscribe : " << flag;
    if (flag == WorkMode::SLEEPING) {
        NodeControl::instance().changeSleepMode();
    } else if (flag == WorkMode::WORKING) {
        NodeControl::instance().changeWorkMode();
    } else if (flag == WorkMode::MAPPING) {
        NodeControl::instance().changeMapMode();
    }
}