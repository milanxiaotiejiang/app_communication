//
// Created by Looper on 2022/11/30.
//

#include "exploration/pose/amcl_pose_subscribe.h"
#include "segmentation/map_attribute.h"
#include "tf/transform_listener.h"
#include "task/simulation.h"
#include <geometry_msgs/Pose2D.h>

AmclPoseSubscribe::AmclPoseSubscribe(ros::NodeHandle handle) : handle(handle) {
    sub_amcl_pose_ = handle.subscribe("/amcl_pose", 10, &AmclPoseSubscribe::subscribeCallback, this);
}

void AmclPoseSubscribe::subscribeCallback(const geometry_msgs::PoseWithCovarianceStamped &poseWithCovarianceStamped) {
    geometry_msgs::PoseWithCovarianceStamped_<std::allocator<void>>::_pose_type covariance = poseWithCovarianceStamped.pose;

    geometry_msgs::PoseWithCovariance_<std::allocator<void>>::_pose_type &pose = covariance.pose;

    geometry_msgs::Pose_<std::allocator<void>>::_position_type &point = pose.position;
    geometry_msgs::Pose_<std::allocator<void>>::_orientation_type &orientation = pose.orientation;

    geometry_msgs::Pose2D pose2D;
    pose2D.x = point.x;
    pose2D.y = point.y;
    pose2D.theta = tf::getYaw(orientation);
    if (isSimulation)
        MapAttribute::instance().setRobotPositionPose(pose2D);
}

AmclPoseSubscribe::~AmclPoseSubscribe() {

}
