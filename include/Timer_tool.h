//
// Created by lijiang on 2021/12/22.
//
#include "ros/ros.h"

#ifndef APP_COMMUNICATION_TIMER_TOOL_H
#define APP_COMMUNICATION_TIMER_TOOL_H


class Timer_tool {

private:
    bool is_paused = false;
    bool is_started = false;
    int WAIT_SEC;

    ros::Time restart;
    int sumTime;
    int durTimeBeforePause;

    ros::Time start0;//启动时计时
    int RunSumTime;
public:
    int getRunSumTime() const;

    bool isPaused() const;

    void setIsPaused(bool isPaused);

    bool isStarted() const;

    void setIsStarted(bool isStarted);

    int getWaitSec() const;

    void setWaitSec(int waitSec);

    int getSumTime();

    void startTimer();

    void startRunTimer();

    void stopTimer();

    void pauseTimer();

    void resumeTimer();

private:

    static Timer_tool *m_instance_ptr;
public:

    ~Timer_tool() = default;

    static Timer_tool *get_instance() {
        if (m_instance_ptr == nullptr) {
            m_instance_ptr = new Timer_tool;
        }
        return m_instance_ptr;
    }
};


#endif //APP_COMMUNICATION_TIMER_TOOL_H
