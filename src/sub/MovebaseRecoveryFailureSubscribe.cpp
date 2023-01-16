/**
 * @file MovebaseRecoveryFailureSubscribe.cpp
 * @author QuanLi (you@domain.com)
 * @brief 监听movebase 发来的跳下一个点的信号
 * @version 0.1
 * @date 2022-04-19
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "sub/MoveBaseRecoveryFailureSubscribe.h"

MoveBaseRecoveryFailureSubscribe::MoveBaseRecoveryFailureSubscribe(ros::NodeHandle handle) : m_handle(handle) {
    m_sub_movebase_recovery_failure_ = handle.subscribe(MOVEBASERECOVERYFAILURE, 1,
                                                        &MoveBaseRecoveryFailureSubscribe::MoveBaseRecoveryFailureCallback,
                                                        this);
}

MoveBaseRecoveryFailureSubscribe::~MoveBaseRecoveryFailureSubscribe() {
}

void MoveBaseRecoveryFailureSubscribe::MoveBaseRecoveryFailureCallback(const std_msgs::String::ConstPtr &msg) {
//    if (!Variable::get_instance()->getMoveBaseRecoveryFailure()) {
//        Variable::get_instance()->setMoveBaseRecoveryFailure(true);
//    }
}