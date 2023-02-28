//
// Created by Looper on 2023/2/28.
//

#ifndef APP_COMMUNICATION_CARTOGRAPHERSUBSCRIBE_H
#define APP_COMMUNICATION_CARTOGRAPHERSUBSCRIBE_H

#include <ros/ros.h>
#include <std_msgs/Int32.h>

class CartographerSubscribe {
    ros::Subscriber sub_cartographer_;

    void subscribeCallback(const std_msgs::Int32 &carto_result);

public:
    CartographerSubscribe(ros::NodeHandle handle);
};


#endif //APP_COMMUNICATION_CARTOGRAPHERSUBSCRIBE_H
