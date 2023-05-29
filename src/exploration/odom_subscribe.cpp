//
// Created by Looper on 2022/11/30.
//

#include "exploration/odom_subscribe.h"
#include "segmentation/map_attribute.h"
#include "tf/transform_listener.h"
#include "manager/PublishInnerManager.h"
#include "tool/Variable.h"
#include "net/WsServerManager.h"
#include <geometry_msgs/Pose2D.h>

OdomSubscribe::OdomSubscribe(ros::NodeHandle handle) : handle(handle) {
    sub_odom_ = handle.subscribe("/odom_app", 10, &OdomSubscribe::subscribeCallback, this);
}

void OdomSubscribe::subscribeCallback(const nav_msgs::OdometryConstPtr &msg) {
    geometry_msgs::PoseWithCovarianceStamped_<std::allocator<void>>::_pose_type covariance = msg->pose;

    geometry_msgs::PoseWithCovariance_<std::allocator<void>>::_pose_type &pose = covariance.pose;

    geometry_msgs::Pose_<std::allocator<void>>::_position_type &point = pose.position;
    geometry_msgs::Pose_<std::allocator<void>>::_orientation_type &orientation = pose.orientation;

    geometry_msgs::Pose2D pose2D;
    pose2D.x = point.x;
    pose2D.y = point.y;
    pose2D.theta = tf::getYaw(orientation);
    MapAttribute::instance().setRobotPositionPose(pose2D);

    nav_msgs::Path passed_path;
    passed_path.header = msg->header;
    geometry_msgs::PoseStamped p;
    p.header = msg->header;
    p.pose = msg->pose.pose;
    passed_path.poses.emplace_back(p);
    PublishInnerManager::instance().publishPassedPath(passed_path);

    geometry_msgs::Pose current_pose;
    current_pose.position = msg->pose.pose.position;
    current_pose.orientation = msg->pose.pose.orientation;
    Variable::get_instance()->setCurrentPose(current_pose);
    WsServerManager::instance().setOdomApp(msg);
}

OdomSubscribe::~OdomSubscribe() = default;
