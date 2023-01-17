//
// Created by lijiang on 2021/12/22.
//

#include "Timer_tool.h"

int Timer_tool::getSumTime() {
    if (!is_started) {
        return 0;
    } else if (is_paused) {
        return durTimeBeforePause;
    } else {
        sumTime = (ros::Time::now() - restart).sec + durTimeBeforePause;
        return sumTime;
    }
}

void Timer_tool::startTimer() {
    restart = ros::Time::now();
    sumTime = 0;
    durTimeBeforePause = 0;
    is_started = true;
}

void Timer_tool::startRunTimer() {
    start0 = ros::Time::now();
    RunSumTime = 0;
}

void Timer_tool::stopTimer() {
    is_started = false;
}

void Timer_tool::pauseTimer() {
    if (!is_paused) {
        is_paused = true;
        durTimeBeforePause = (ros::Time::now() - restart).sec;
    }

}

void Timer_tool::resumeTimer() {
    if (is_paused) {
        is_paused = false;
        restart = ros::Time::now();
    }

}

bool Timer_tool::isStarted() const {
    return is_started;
}

void Timer_tool::setIsStarted(bool isStarted) {
    is_started = isStarted;
}

bool Timer_tool::isPaused() const {
    return is_paused;
}

void Timer_tool::setIsPaused(bool isPaused) {
    is_paused = isPaused;
}

int Timer_tool::getWaitSec() const {
    return WAIT_SEC;
}

void Timer_tool::setWaitSec(int waitSec) {
    WAIT_SEC = waitSec;
}

int Timer_tool::getRunSumTime() const {
    return (ros::Time::now() - start0).sec;
}
