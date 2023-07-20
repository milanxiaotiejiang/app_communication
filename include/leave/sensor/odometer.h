//
// Created by Looper on 2023/7/17.
//

#ifndef APP_COMMUNICATION_ODOMETER_H
#define APP_COMMUNICATION_ODOMETER_H

#include "sensor.h"
#include "nav_msgs/Odometry.h"
#include "leave/sensor/sensor_center.h"

/**
 * 里程计
 * /wheel_odom
 */
class Odometer : public Sensor<nav_msgs::Odometry> {
public:
    explicit Odometer(const ros::NodeHandle &handle);

    ~Odometer() override;

    void dateProgressing(nav_msgs::Odometry data) override;
};


#endif //APP_COMMUNICATION_ODOMETER_H
