//
// Created by lijiang on 2022/1/11.
//

#ifndef APP_COMMUNICATION_ODOMINNERSUBSCRIBE_H
#define APP_COMMUNICATION_ODOMINNERSUBSCRIBE_H

#include <ros/ros.h>
#include <pub/PubInner.h>
#include <pub/PubOut.h>
#include "net/WsServerManager.h"

class OdomInnerSubscribe {

private:
    ros::NodeHandle handle;

    PubInner pubInner;
    PubOut pubOut;
    ros::Subscriber sub_odom_inner_;

public:

    OdomInnerSubscribe(ros::NodeHandle handle, PubInner pubInner, PubOut pubOut);

    virtual ~OdomInnerSubscribe();

    void subscribeCallback(const nav_msgs::Odometry &odom_app);
};


#endif //APP_COMMUNICATION_ODOMINNERSUBSCRIBE_H
