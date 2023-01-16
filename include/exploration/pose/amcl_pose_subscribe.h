//
// Created by Looper on 2022/10/11.
//

#ifndef APP_COMMUNICATION_AMCL_POSE_SUBSCRIBE_H
#define APP_COMMUNICATION_AMCL_POSE_SUBSCRIBE_H

#include <ros/ros.h>
#include <geometry_msgs/PoseWithCovarianceStamped.h>

class AmclPoseSubscribe {

private:
    ros::NodeHandle handle;

    ros::Subscriber sub_amcl_pose_;

    void subscribeCallback(const geometry_msgs::PoseWithCovarianceStamped &poseWithCovarianceStamped);

public:
    AmclPoseSubscribe(ros::NodeHandle handle);

    virtual ~AmclPoseSubscribe();
};


#endif //APP_COMMUNICATION_AMCL_POSE_SUBSCRIBE_H
