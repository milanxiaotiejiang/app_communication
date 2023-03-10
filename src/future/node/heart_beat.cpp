//
// Created by admin1 on 2023/3/10.
//

#include "future/node/heart_beat.h"
#include "future/node/node_control.h"

CartoHeartBeat::CartoHeartBeat(ros::NodeHandle handle) {
    sub_flag_ = handle.subscribe("/carto_heart_beat", 1, &CartoHeartBeat::subscribeCallback, this);
}

void CartoHeartBeat::subscribeCallback(const std_msgs::Int32 &flag_result) {
    auto flag = flag_result.data;
    if (NodeControl::instance().heart_beat > 10000)
        NodeControl::instance().heart_beat = 0;
    NodeControl::instance().heart_beat++;
}