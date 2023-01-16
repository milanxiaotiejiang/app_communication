//
// Created by lijiang on 2022/1/10.
//

#ifndef APP_COMMUNICATION_SCANINNERSUBSCRIBE_H
#define APP_COMMUNICATION_SCANINNERSUBSCRIBE_H


#include<ros/ros.h>
#include <pub/PubInner.h>
#include <pub/PubOut.h>
#include "sensor_msgs/LaserScan.h"
#include <Eigen/Geometry>

class ScanInnerSubscribe {


private:
    ros::NodeHandle handle;
    PubInner pubInner;
    PubOut pubOut;
    ros::Subscriber sub_scan_inner_;

    sensor_msgs::LaserScan scan_app;
public:
    ScanInnerSubscribe(ros::NodeHandle &handle, PubInner &pubInner, PubOut &pubOut);

    virtual ~ScanInnerSubscribe();

    void subscribeCallback(const sensor_msgs::LaserScan &scan_raw);

};

#endif //APP_COMMUNICATION_SCANINNERSUBSCRIBE_H
