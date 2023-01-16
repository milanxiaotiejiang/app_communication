//
// Created by lijiang on 2022/1/11.
//

#include "sub/OdomInnerSubscribe.h"
#include "tool/Variable.h"

OdomInnerSubscribe::OdomInnerSubscribe(ros::NodeHandle handle, PubInner pubInner, PubOut pubOut)
        : handle(handle),
          pubInner(std::move(pubInner)),
          pubOut(std::move(pubOut)) {
    sub_odom_inner_ = handle.subscribe("/odom_app", 10, &OdomInnerSubscribe::subscribeCallback, this);
}

OdomInnerSubscribe::~OdomInnerSubscribe() {

}

void OdomInnerSubscribe::subscribeCallback(const nav_msgs::Odometry &odom_app) {
    geometry_msgs::Pose current_pose;
    current_pose.position = odom_app.pose.pose.position;
    current_pose.orientation = odom_app.pose.pose.orientation;
    Variable::get_instance()->setCurrentPose(current_pose);
    WsServerManager::instance().setOdomApp(odom_app);
}