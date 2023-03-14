//
// Created by Looper on 2023/3/14.
//

#include "task/subscribe/carpet_detect_subscribe.h"

CarpetDetectSubscribe::CarpetDetectSubscribe(ros::NodeHandle handle) {
    sub_carpet_ = handle.subscribe("/mrrobot/carpet_detect", 1, &CarpetDetectSubscribe::subscribeCallback, this);
}

void CarpetDetectSubscribe::subscribeCallback(const std_msgs::Int32 &flag_result) {
    int data = flag_result.data;
    asyncTaskCall->executeCarpet(data == 1);
}