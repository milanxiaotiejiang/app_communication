//
// Created by io on 23-10-31.
//

#ifndef APP_COMMUNICATION_MULTIPLE_MAP_SUBSCRIBE_H
#define APP_COMMUNICATION_MULTIPLE_MAP_SUBSCRIBE_H

#include "ros/ros.h"
#include <std_msgs/Int32.h>
#include "std_msgs/String.h"

class MultipleMapSubscribe {
private:
    ros::Subscriber sub_map_create_;
    ros::Subscriber sub_map_switch_;

    void multipleMapCreateSubscribeCallback(const std_msgs::Int32 &flag);

    void multipleMapSwitchSubscribeCallback(const std_msgs::String &flag);

public:
    MultipleMapSubscribe(ros::NodeHandle handle);
};


#endif //APP_COMMUNICATION_MULTIPLE_MAP_SUBSCRIBE_H
