//
// Created by io on 23-11-21.
//

#ifndef APP_COMMUNICATION_HARDWARE_SUBSCRIBER_H
#define APP_COMMUNICATION_HARDWARE_SUBSCRIBER_H


#include <ros/ros.h>
#include <sensor_msgs/PointCloud2.h>
#include <sensor_msgs/LaserScan.h>

class InuSubscriberSingleton {
private:
    std::atomic<int> heart_beat_inu1;
    std::atomic<int> heart_beat_inu2;
    std::atomic<int> heart_beat_scan;

    ros::Subscriber subscriberInu1;
    ros::Subscriber subscriberInu2;

    ros::Subscriber subscriberScan;

public:
    static InuSubscriberSingleton &instance() {
        static InuSubscriberSingleton instance;
        return instance;
    }

    void init(ros::NodeHandle handle);

    void subscribeCallback1(const sensor_msgs::PointCloud2 &data);

    void subscribeCallback2(const sensor_msgs::PointCloud2 &data);

    void subscribeCallbackScan(const sensor_msgs::LaserScan &data);

    void recount();

    int heartBeatInu1();

    int heartBeatInu2();

    int heartBeatScan();
};


#endif //APP_COMMUNICATION_HARDWARE_SUBSCRIBER_H
