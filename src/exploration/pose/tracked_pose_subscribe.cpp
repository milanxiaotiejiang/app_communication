//
// Created by Looper on 2022/10/11.
//

#include "exploration/pose/tracked_pose_subscribe.h"
#include <geometry_msgs/Pose2D.h>
#include "tf/transform_listener.h"
#include "segmentation/map_attribute.h"

TrackedSubscribe::TrackedSubscribe(ros::NodeHandle handle) : handle(handle) {
    sub_tracked_pose_ = handle.subscribe("/tracked_pose", 10, &TrackedSubscribe::subscribeCallback, this);
}

void TrackedSubscribe::subscribeCallback(const geometry_msgs::PoseStamped &poseStamped) {
    geometry_msgs::Pose2D pose2D;
    pose2D.x = poseStamped.pose.position.x;
    pose2D.y = poseStamped.pose.position.y;
    pose2D.theta = tf::getYaw(poseStamped.pose.orientation);
    MapAttribute::instance().setRobotPositionPose(pose2D);
}

TrackedSubscribe::~TrackedSubscribe() {

}