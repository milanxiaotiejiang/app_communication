//
// Created by Looper on 2023/7/17.
//

#include "leave/sensor/imu.h"
#include "net/ros/RosBasic.h"
#include "net/ros/RosImu.h"

Imu::Imu(const ros::NodeHandle &handle) : Sensor(handle, "/handsfree/imu",
                                                 false, true, true) { outLog = false; }

Imu::~Imu() = default;

void Imu::dateProgressing(sensor_msgs::Imu data) {
    if (outLog)
        LOG_IF(INFO, DEBUG_DUMP) << "自检 Imu " << data << "...";

    sensor_msgs::Imu::_orientation_covariance_type &orientation_covariance = data.orientation_covariance;

    sensor_msgs::Imu::_angular_velocity_covariance_type &angular_velocity_covariance = data.angular_velocity_covariance;

    sensor_msgs::Imu::_linear_acceleration_covariance_type &linear_acceleration_covariance = data.linear_acceleration_covariance;

    RosStamp stamp(data.header.stamp.nsec, data.header.stamp.sec);
    RosHeader header(data.header.frame_id, data.header.seq, stamp);

    RosOrientation orientation(data.orientation.w, data.orientation.x, data.orientation.y, data.orientation.z);

    std::vector<double> orientationCovariance(data.orientation_covariance.begin(), data.orientation_covariance.end());

    RosVector3 angularVelocity(data.angular_velocity.x, data.angular_velocity.y, data.angular_velocity.z);

    std::vector<double> angularVelocityCovariance(data.angular_velocity_covariance.begin(),
                                                  data.angular_velocity_covariance.end());

    RosVector3 linearAcceleration(data.linear_acceleration.x, data.linear_acceleration.y, data.linear_acceleration.z);

    std::vector<double> linearAccelerationCovariance(data.linear_acceleration_covariance.begin(),
                                                     data.linear_acceleration_covariance.end());

    RosImu rosImu(header, orientation, orientationCovariance, angularVelocity,
                  angularVelocityCovariance, linearAcceleration, linearAccelerationCovariance);

    if (deliveryCenter) {
        SensorCenter::instance().setImuData(rosImu);
    } else {
        RequestModel<RosImu> requestModel(
                "publish", APP_HANDSFREE_IMU, rosImu
        );
        json jsonResult = requestModel;
        WsServerManager::instance().sendRequestData(APP_HANDSFREE_IMU, jsonResult.dump());
    }
}
