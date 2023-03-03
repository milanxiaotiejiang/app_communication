//
// Created by Looper on 2023/2/28.
//

#include "task/subscribe/CartographerSubscribe.h"
#include "task/subscribe/CartographerManager.h"
#include "exploration/ExplorationCenter.h"

CartographerSubscribe::CartographerSubscribe(ros::NodeHandle handle) {
    sub_cartographer_ = handle.subscribe("/update_finish", 1, &CartographerSubscribe::subscribeCallback, this);
}

void CartographerSubscribe::subscribeCallback(const std_msgs::Int32 &flag_result) {
    ExplorationCenter::instance().repaintCoveragePath(true, true);
    CartographerManager::instance().coverResult();
}