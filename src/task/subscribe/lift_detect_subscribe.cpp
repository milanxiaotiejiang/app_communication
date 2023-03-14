//
// Created by Looper on 2023/3/14.
//

#include "task/subscribe/lift_detect_subscribe.h"

LiftDetectSubscribe::LiftDetectSubscribe(ros::NodeHandle handle) {
    sub_lift_ = handle.subscribe("/mrrobot/blob_detect", 1, &LiftDetectSubscribe::subscribeCallback, this);
}

void LiftDetectSubscribe::subscribeCallback(const std_msgs::Int32 &flag_result) {
    int data = flag_result.data;
    asyncTaskCall->executeLift(data == 1);
}