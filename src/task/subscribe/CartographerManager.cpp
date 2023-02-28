//
// Created by Looper on 2023/2/28.
//

#include <std_msgs/Int32.h>
#include "task/subscribe/CartographerManager.h"
#include "simulation.h"
#include "future/timer_call.h"

void CartographerManager::initialize(ros::NodeHandle handle) {
    pub_cartographer = handle.advertise<std_msgs::Int32>("/update_map", 10);
}

void CartographerManager::pubCover() {
    std_msgs::Int32 flag;
    flag.data = 1;
    pub_cartographer.publish(flag);
    if (!Environment::instance().isRealEnvironment) {
        async::TimerCall::instance().baseLoop()->scheduleLater(std::chrono::seconds(10), []() {
            CartographerManager::instance().coverResult();
        });
    }
}

void CartographerManager::coverResult() {
    asyncTaskCall->executeCover();
}
