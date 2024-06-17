//
// Created by io on 23-11-21.
//

#include "../../../include/future/node/hardware_subscriber.h"

void InuSubscriberSingleton::init(ros::NodeHandle handle) {
    subscriberInu1 = handle.subscribe("/1/depth/depth2pc", 1, &InuSubscriberSingleton::subscribeCallback1, this);
    subscriberInu2 = handle.subscribe("/2/depth/depth2pc", 1, &InuSubscriberSingleton::subscribeCallback2, this);
    subscriberScan = handle.subscribe("/scan", 1, &InuSubscriberSingleton::subscribeCallbackScan, this);
}

void InuSubscriberSingleton::subscribeCallback1(const sensor_msgs::PointCloud2 &data) {
    if (heart_beat_inu1 == INT_MAX - 1) {
        heart_beat_inu1 = 0;
    }
    heart_beat_inu1++;
}

void InuSubscriberSingleton::subscribeCallback2(const sensor_msgs::PointCloud2 &data) {
    if (heart_beat_inu2 == INT_MAX - 1) {
        heart_beat_inu2 = 0;
    }
    heart_beat_inu2++;
}

void InuSubscriberSingleton::subscribeCallbackScan(const sensor_msgs::LaserScan &data) {
    if (heart_beat_scan == INT_MAX - 1) {
        heart_beat_scan = 0;
    }
    heart_beat_scan++;
}

void InuSubscriberSingleton::recount() {
    heart_beat_inu1 = 0;
    heart_beat_inu2 = 0;
    heart_beat_scan = 0;
}

int InuSubscriberSingleton::heartBeatInu1() {
    return heart_beat_inu1;
}

int InuSubscriberSingleton::heartBeatInu2() {
    return heart_beat_inu2;
}

int InuSubscriberSingleton::heartBeatScan() {
    return heart_beat_scan;
}
