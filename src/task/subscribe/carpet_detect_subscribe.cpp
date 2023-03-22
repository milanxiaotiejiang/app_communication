//
// Created by Looper on 2023/3/14.
//

#include "task/subscribe/carpet_detect_subscribe.h"

CarpetDetectSubscribe::CarpetDetectSubscribe(ros::NodeHandle handle) {
    sub_carpet_ = handle.subscribe("/mrrobot/carpet_detect", 1, &CarpetDetectSubscribe::subscribeCallback, this);
    count = 0;
}

void CarpetDetectSubscribe::subscribeCallback(const std_msgs::Int32 &flag_result) {
    int data = flag_result.data;
    if (data == 1) {
        count = 0;
        asyncTaskCall->executeCarpet(true);
    } else {
        count++;
        if (count > 30) {
            count = 0;
            asyncTaskCall->executeCarpet(false);
        }
    }

}