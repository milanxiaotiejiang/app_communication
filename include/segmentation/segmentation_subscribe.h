//
// Created by Looper on 2023/4/17.
//

#ifndef APP_COMMUNICATION_SEGMENTATION_SUBSCRIBE_H
#define APP_COMMUNICATION_SEGMENTATION_SUBSCRIBE_H

#include "ros/ros.h"
#include <std_msgs/Int32.h>

class SegmentationSubscribe {
private:
    ros::Subscriber sub_node_control_;
    ros::Subscriber sub_order_control_;

    void segmentationSubscribeCallback(const std_msgs::Int32 &flag);

    void segmentationOrderSubscribeCallback(const std_msgs::Int32 &flag);

public:
    SegmentationSubscribe(ros::NodeHandle handle);
};

#endif //APP_COMMUNICATION_SEGMENTATION_SUBSCRIBE_H
