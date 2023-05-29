//
// Created by lijiang on 2022/6/14.
//

#ifndef APP_COMMUNICATION_DSVERSIONSUBSCRIBE_H
#define APP_COMMUNICATION_DSVERSIONSUBSCRIBE_H

#include<ros/ros.h>
#include <ros/node_handle.h>
#include <std_msgs/String.h>
#include "manager/VersionManager.h"

class DSVersionSubscribe {
private:
    ros::NodeHandle handle;

    ros::Subscriber sub_ds_hw;
    ros::Subscriber sub_ds_sw;

    void subscribeHWCallback(const std_msgs::String &msg);

    void subscribeSWCallback(const std_msgs::String &msg);

public:
    DSVersionSubscribe(ros::NodeHandle handle);

};


#endif //APP_COMMUNICATION_DSVERSIONSUBSCRIBE_H
