//
// Created by admin1 on 22-11-25.
//

#include "task/manager/StationManager.h"
#include "manager/PublishInnerManager.h"
#include "future/timer_call.h"
#include "simulation.h"

const int STOP_IN_STATION = 0;
const int FLAG_IN_STATION = 1;
const int FLAG_OUT_STATION = 2;

const int FLAG_RESULT_FAIL = 0;
const int FLAG_RESULT_SUCCESS = 1;

void StationManager::initialize(ros::NodeHandle handle) {
    pub_flag_in = handle.advertise<std_msgs::Int32>("/flag_in", 10);
    pub_flag_out = handle.advertise<std_msgs::Int32>("/flag_out", 10);
}

void StationManager::outStation() {
    std_msgs::Int32 flag;
    flag.data = FLAG_OUT_STATION;
    pub_flag_out.publish(flag);
    if (!Environment::instance().isRealEnvironment) {
        async::TimerCall::instance().baseLoop()->scheduleLater(std::chrono::seconds(10), [this]() {
            StationManager::instance().stationOutResult(FLAG_RESULT_SUCCESS);
        });
    }
}

void StationManager::stationOutResult(int flag) {
    asyncTaskCall->executeOutStation(flag == FLAG_RESULT_SUCCESS);
}

void StationManager::backStation() {
    std_msgs::Int32 flag;
    flag.data = FLAG_IN_STATION;
    pub_flag_in.publish(flag);
    if (!Environment::instance().isRealEnvironment) {
        async::TimerCall::instance().baseLoop()->scheduleLater(std::chrono::seconds(10), [this]() {
            if (!Environment::instance().will()) {
                StationManager::instance().stationInResult(FLAG_RESULT_SUCCESS);
            } else {
                StationManager::instance().stationInResult(FLAG_RESULT_FAIL);
            }
        });
    }
}

void StationManager::stationInResult(int flag) {
    asyncTaskCall->executeInStation(flag == FLAG_RESULT_SUCCESS);
}
