/**
 * @file MoveBaseRecoveryFailure.h
 * @author Quan Li (you@domain.com)
 * @brief 监听movebase 发来的跳下一个点的信号
 * @version 0.1
 * @date 2022-04-19
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef APP_COMMUNICATION_MOVEBASE_RECOVERY_FAILURE_H
#define APP_COMMUNICATION_MOVEBASE_RECOVERY_FAILURE_H

#include "net/WsServerManager.h"
#include "tool/Variable.h"
#include <ros/ros.h>

class MoveBaseRecoveryFailureSubscribe {
private:
    ros::NodeHandle m_handle;
    ros::Subscriber m_sub_movebase_recovery_failure_;

public:
    MoveBaseRecoveryFailureSubscribe(ros::NodeHandle handle);

    ~MoveBaseRecoveryFailureSubscribe();

    void MoveBaseRecoveryFailureCallback(const std_msgs::String::ConstPtr &msg);
};


#endif