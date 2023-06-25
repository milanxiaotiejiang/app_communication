//
// Created by siyu on 2022/04/06.
//

#ifndef APP_COMMUNICATION_ROBOTSTATUSINNERSUBSCRIBE_H
#define APP_COMMUNICATION_ROBOTSTATUSINNERSUBSCRIBE_H

#include<ros/ros.h>
#include <pub/PubInner.h>
#include <model/WorkStatus.h>
#include <std_msgs/Int32.h>
#include <pub/PubOut.h>
#include "tool/OldFunction.hpp"
#include <tool/OldFunction.hpp>
#include "nlohmann/json.hpp"

using json = nlohmann::json;

class CurrentInnerSubscribe {
private:
    ros::NodeHandle handle;
    PubInner pubInner;
    PubOut pubOut;
    ros::Subscriber sub_ir_;
    int ir_status_message;

    void subscribeCallback(const std_msgs::Int32 &msg);

public:
    CurrentInnerSubscribe(ros::NodeHandle handle, PubInner pubInner, PubOut pubOut);

    virtual ~CurrentInnerSubscribe();

};


#endif //APP_COMMUNICATION_ROBOTSTATUSINNERSUBSCRIBE_H