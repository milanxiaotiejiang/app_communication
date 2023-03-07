//
// Created by Looper on 2023/3/7.
//

#ifndef APP_COMMUNICATION_NODE_CONTROL_SUBSCRIBE_H
#define APP_COMMUNICATION_NODE_CONTROL_SUBSCRIBE_H

#include "ros/ros.h"
#include <std_msgs/Int32.h>

class NodeControlSubscribe {
private:
    ros::Subscriber sub_flag_;

    void subscribeCallback(const std_msgs::Int32 &flag);

public:
    NodeControlSubscribe(ros::NodeHandle handle);
};


#endif //APP_COMMUNICATION_NODE_CONTROL_SUBSCRIBE_H
