//
// Created by lijiang on 2021/12/24.
//

#ifndef APP_COMMUNICATION_FULLPATHSUBSCRIBE_H
#define APP_COMMUNICATION_FULLPATHSUBSCRIBE_H

#include<ros/ros.h>
#include <pub/PubInner.h>
#include <pub/PubOut.h>
#include "nav_msgs/Odometry.h"
#include "nav_msgs/Path.h"

class FullPathSubscribe {
private:
    ros::NodeHandle handle;

    PubInner pubInner;
    PubOut pubOut;

    ros::Subscriber path_sub_;

    void subscribeCallback(const nav_msgs::Path &path);

public:
    FullPathSubscribe(ros::NodeHandle handle, PubInner pubInner, PubOut pubOut);

    virtual ~FullPathSubscribe();
};


#endif //APP_COMMUNICATION_FULLPATHSUBSCRIBE_H
