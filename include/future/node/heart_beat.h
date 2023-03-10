//
// Created by admin1 on 2023/3/10.
//

#ifndef APP_COMMUNICATION_HEART_BEAT_H
#define APP_COMMUNICATION_HEART_BEAT_H

#include "ros/ros.h"
#include <std_msgs/Int32.h>

class CartoHeartBeat {
private:
    ros::Subscriber sub_flag_;

    void subscribeCallback(const std_msgs::Int32 &flag);

public:
    CartoHeartBeat(ros::NodeHandle handle);
};


#endif //APP_COMMUNICATION_HEART_BEAT_H
