//
// Created by lijiang on 2021/12/24.
//

#include "sub/OdomSubscribe.h"

OdomSubscribe::OdomSubscribe(ros::NodeHandle handle, PubInner pubInner, PubOut pubOut)
        : handle(handle),
          pubInner(std::move(pubInner)),
          pubOut(std::move(pubOut)) {
    sub_odom_ = handle.subscribe("/odom_app", 10, &OdomSubscribe::subscribeCallback, this);
}

OdomSubscribe::~OdomSubscribe() {

}

void OdomSubscribe::subscribeCallback(const nav_msgs::Odometry &poses) {
    nav_msgs::Path passed_path;
    passed_path.header = poses.header;
    geometry_msgs::PoseStamped p;
    p.header = poses.header;
    p.pose = poses.pose.pose;
    passed_path.poses.emplace_back(p);
    pubInner.publishPassedPath(passed_path);
}
