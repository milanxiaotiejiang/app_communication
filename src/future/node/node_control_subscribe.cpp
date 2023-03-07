//
// Created by Looper on 2023/3/7.
//

#include "future/node/node_control_subscribe.h"
#include "future/node/node_control.h"

NodeControlSubscribe::NodeControlSubscribe(ros::NodeHandle handle) {
    sub_flag_ = handle.subscribe("/tt_node_control", 1, &NodeControlSubscribe::subscribeCallback, this);
}

void NodeControlSubscribe::subscribeCallback(const std_msgs::Int32 &flag_result) {
    auto flag = flag_result.data;
    LOG(INFO) << "NodeControlSubscribe : " << flag;
    if (flag == 0) {
        NodeControl::instance().changeSleepMode();
    } else if (flag == 1) {
        NodeControl::instance().changeWorkMode();
    } else if (flag == 2) {
        NodeControl::instance().changeMapMode();
    }
}