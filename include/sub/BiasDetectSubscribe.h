//
// Created by Ewen Dong
//

#ifndef BIAS_DETECT_SUBSCRIBE_H
#define BIAS_DETECT_SUBSCRIBE_H

#include <ros/ros.h>
#include <std_msgs/Int16.h>

class BiasDetectSubscribe {
public:
    BiasDetectSubscribe(ros::NodeHandle handle);

    virtual ~BiasDetectSubscribe();

private:
    ros::NodeHandle handle;
    ros::Subscriber sub_bias_detect_;

    void subscribeCallback(std_msgs::Int16);
};

#endif
