//
// Created by Looper on 2022/9/27.
//

#ifndef APP_COMMUNICATION_SEGMENTATIONSUBSCRIBE_H
#define APP_COMMUNICATION_SEGMENTATIONSUBSCRIBE_H

#include <ros/ros.h>
#include <std_msgs/Int32.h>

class SegmentationSubscribe {
    ros::NodeHandle handle;
    ros::Subscriber subscriber_segmentation_;

    void subscribeCallback(const std_msgs::Int32 &result);

public:
    SegmentationSubscribe(ros::NodeHandle handle);

    virtual ~SegmentationSubscribe();

};


#endif //APP_COMMUNICATION_SEGMENTATIONSUBSCRIBE_H
