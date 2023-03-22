//
// Created by Looper on 2023/3/14.
//

#ifndef APP_COMMUNICATION_CARPET_DETECT_SUBSCRIBE_H
#define APP_COMMUNICATION_CARPET_DETECT_SUBSCRIBE_H

#include <ros/ros.h>
#include "std_msgs/Int32.h"
#include "task/async_task_call.h"

class CarpetDetectSubscribe {
    ros::Subscriber sub_carpet_;
    AsyncTaskCall *asyncTaskCall;

    std::atomic<int> count;

    void subscribeCallback(const std_msgs::Int32 &carto_result);

public:
    CarpetDetectSubscribe(ros::NodeHandle handle);

    void setAsyncTaskCall(AsyncTaskCall *asyncTaskCall) {
        CarpetDetectSubscribe::asyncTaskCall = asyncTaskCall;
    }
};


#endif //APP_COMMUNICATION_CARPET_DETECT_SUBSCRIBE_H
