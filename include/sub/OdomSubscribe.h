//
// Created by lijiang on 2021/12/24.
//

#ifndef APP_COMMUNICATION_ODOMSUBSCRIBE_H
#define APP_COMMUNICATION_ODOMSUBSCRIBE_H

#include<ros/ros.h>
#include <pub/PubInner.h>
#include <pub/PubOut.h>
#include "nav_msgs/Odometry.h"
#include "nav_msgs/Path.h"

class OdomSubscribe {
private:
    ros::NodeHandle handle;

    PubInner pubInner;
    PubOut pubOut;
    ros::Subscriber sub_odom_;

    void subscribeCallback(const nav_msgs::Odometry &poses);

public:

    OdomSubscribe(ros::NodeHandle handle, PubInner pubInner, PubOut pubOut);

    virtual ~OdomSubscribe();

};


#endif //APP_COMMUNICATION_ODOMSUBSCRIBE_H
