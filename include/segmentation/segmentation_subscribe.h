//
// Created by Looper on 2023/4/17.
//

#ifndef APP_COMMUNICATION_SEGMENTATION_SUBSCRIBE_H
#define APP_COMMUNICATION_SEGMENTATION_SUBSCRIBE_H

#include "ros/ros.h"
#include <std_msgs/Int32.h>
#include "std_msgs/String.h"

class SegmentationSubscribe {
private:
    ros::Subscriber sub_node_control_;
    ros::Subscriber sub_order_control_;
    ros::Subscriber sub_test_control_;
    ros::Subscriber sub_room_control_;
    ros::Subscriber sub_open_gate_;
    ros::Subscriber sub_close_gate_;

    void segmentationSubscribeCallback(const std_msgs::Int32 &flag);

    void segmentationOrderSubscribeCallback(const std_msgs::Int32 &flag);

    void segmentationTestSubscribeCallback(const std_msgs::Int32 &flag);

    void segmentationRoomSubscribeCallback(const std_msgs::Int32 &flag);

    void gateOpenSubscribeCallback(const std_msgs::String &flag);
    void gateCloseSubscribeCallback(const std_msgs::String &flag);

public:
    SegmentationSubscribe(ros::NodeHandle handle);
};

#endif //APP_COMMUNICATION_SEGMENTATION_SUBSCRIBE_H
