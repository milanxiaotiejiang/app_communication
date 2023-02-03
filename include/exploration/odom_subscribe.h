//
// Created by Looper on 2022/10/11.
//

#ifndef APP_COMMUNICATION_ODOM_SUBSCRIBE_H
#define APP_COMMUNICATION_ODOM_SUBSCRIBE_H

#include <ros/ros.h>
#include <geometry_msgs/PoseWithCovarianceStamped.h>
#include <nav_msgs/Odometry.h>
#include "nav_msgs/Path.h"

class OdomSubscribe {

private:
    ros::NodeHandle handle;

    ros::Subscriber sub_odom_;

    void subscribeCallback(const nav_msgs::OdometryConstPtr &msg);

public:
    OdomSubscribe(ros::NodeHandle handle);

    virtual ~OdomSubscribe();
};


#endif //APP_COMMUNICATION_ODOM_SUBSCRIBE_H
