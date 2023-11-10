//
// Created by Looper on 2023/7/17.
//

#include "leave/sensor/odometer.h"
#include "net/ros/RosBasic.h"

Odometer::Odometer(const ros::NodeHandle &handle) : Sensor(handle, "/wheel_odom",
                                                           false, true, true) { outLog = false; }

Odometer::~Odometer() = default;

void Odometer::dateProgressing(nav_msgs::Odometry data) {
    if (outLog)
        LOG_IF(INFO, DEBUG_DUMP) << "自检 里程计 " << data << " ...";

    RosStamp stamp(data.header.stamp.nsec, data.header.stamp.sec);
    RosHeader header(data.header.frame_id, data.header.seq, stamp);

    RosOrientation orientation(data.pose.pose.orientation.w,
                               data.pose.pose.orientation.x,
                               data.pose.pose.orientation.y,
                               data.pose.pose.orientation.z);
    RosPosition position(data.pose.pose.position.x,
                         data.pose.pose.position.y,
                         data.pose.pose.position.z);
    RosOrigin origin(orientation, position);
    std::vector<double> poseCovariance;
    for (const auto &item: data.pose.covariance) {
        poseCovariance.push_back(item);
    }
    RosPose pose(origin, poseCovariance);

    RosAngular angular(data.twist.twist.angular.x,
                       data.twist.twist.angular.y,
                       data.twist.twist.angular.z);
    RosLinear linear(data.twist.twist.linear.x,
                     data.twist.twist.linear.y,
                     data.twist.twist.linear.z);
    RosTwistX twistX(angular, linear);
    std::vector<double> twistCovariance;
    for (const auto &item: data.twist.covariance) {
        twistCovariance.push_back(item);
    }
    RosTwist twist(twistX, twistCovariance);

    RosOdom odom(data.child_frame_id, header, pose, twist);

    if (deliveryCenter) {
        SensorCenter::instance().setOdomData(odom);
    } else {
        RequestModel<RosOdom> requestModel(
                "publish", APP_WHEEL_ODOM, odom
        );
        json jsonResult = requestModel;
        WsServerManager::instance().sendRequestData(APP_WHEEL_ODOM, jsonResult.dump());
    }
}
