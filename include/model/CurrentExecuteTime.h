#ifndef APP_COMMUNICATION_CURRENT_EXECUTE_TIME_H
#define APP_COMMUNICATION_CURRENT_EXECUTE_TIME_H
/**
 * @file CurrentExecuteTime.cpp
 * @author QuanLi (you@domain.com)
 * @brief 用于检测本次任务执行时间
 * @version 0.1
 * @date 2022-04-18
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "pub/PubInner.h"
#include "pub/PubOut.h"
#include "ros/ros.h"
#include <thread>
#include <time.h>
#include <sys/time.h>


class CurrentExecuteTime {
private:
    long m_execute_time;        //记录开始时间
    long m_current_execute_time;//记录当前执行时间
    ros::NodeHandle handle;
    PubInner pubInner;
    PubOut pubOut;

    void ThreadCreate();// 用于创建线程
    void ThreadHandle();// 线程中执行的内容，即自检。

public:
    CurrentExecuteTime(ros::NodeHandle handle, PubInner pubinner, PubOut pubout);

    ~CurrentExecuteTime();

    void setExecuteTime(const long &execut_time) {
        m_execute_time = execut_time;
    }

    const long &getExecuteTime() {
        return m_execute_time;
    }

    void setCurrentExecuteTime(const long &current_execut_time) {
        m_current_execute_time = current_execut_time;
    }

    const long &getCurrentExecuteTime() {
        return m_current_execute_time;
    }
};

#endif