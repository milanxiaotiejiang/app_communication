//
// Created by Looper on 2022/10/11.
//

#ifndef APP_COMMUNICATION_TRACKED_POSE_SUBSCRIBE_H
#define APP_COMMUNICATION_TRACKED_POSE_SUBSCRIBE_H

#include <ros/ros.h>
#include <geometry_msgs/PoseStamped.h>

class TrackedSubscribe {

private:
    ros::NodeHandle handle;

    ros::Subscriber sub_tracked_pose_;

    void subscribeCallback(const geometry_msgs::PoseStamped &poseStamped);

public:
    TrackedSubscribe(ros::NodeHandle handle);

    virtual ~TrackedSubscribe();
};


#endif //APP_COMMUNICATION_AMCL_POSE_SUBSCRIBE_H
