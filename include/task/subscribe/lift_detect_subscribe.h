//
// Created by Looper on 2023/3/14.
//

#ifndef APP_COMMUNICATION_LIFT_DETECT_SUBSCRIBE_H
#define APP_COMMUNICATION_LIFT_DETECT_SUBSCRIBE_H

#include <ros/ros.h>
#include "std_msgs/Int32.h"
#include "task/async_task_call.h"

class LiftDetectSubscribe {
    ros::Subscriber sub_lift_;
    std::shared_ptr<AsyncTaskCall> asyncTaskCall;

    void subscribeCallback(const std_msgs::Int32 &carto_result);

public:
    LiftDetectSubscribe(ros::NodeHandle handle);

    void setAsyncTaskCall(std::shared_ptr<AsyncTaskCall> asyncTaskCall) {
        LiftDetectSubscribe::asyncTaskCall = asyncTaskCall;
    }
};


#endif //APP_COMMUNICATION_LIFT_DETECT_SUBSCRIBE_H
