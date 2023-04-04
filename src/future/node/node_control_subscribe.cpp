//
// Created by Looper on 2023/3/7.
//

#include "future/node/node_control_subscribe.h"
#include "future/node/node_control.h"
#include "task/manager/NodeWorkModeManager.h"

NodeControlSubscribe::NodeControlSubscribe(ros::NodeHandle handle) {
    sub_node_control_ = handle.subscribe("/node_control", 1, &NodeControlSubscribe::nodeControlSubscribeCallback, this);
    sub_initial_pose_ = handle.subscribe("/initialpose", 1, &NodeControlSubscribe::initialPoseSubscribeCallback, this);
}

void NodeControlSubscribe::nodeControlSubscribeCallback(const std_msgs::Int32 &flag_result) {
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

void
NodeControlSubscribe::initialPoseSubscribeCallback(const geometry_msgs::PoseWithCovarianceStampedConstPtr &pose) {
    auto init_pose = pose->pose.pose;
    LOG(INFO) << "给定一个指定的位置来重定位carto,x : " << init_pose.position.x << " y: " << init_pose.position.y;
    //关闭cartographer
    NodeControl::instance().shutdownCartoNodeOnly();
    //设定初始位姿
    NodeControl::instance().paramPose("/set_initial_pose", init_pose);
}
