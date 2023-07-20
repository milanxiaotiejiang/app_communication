//
// Created by Looper on 2023/7/17.
//

#ifndef APP_COMMUNICATION_IMU_H
#define APP_COMMUNICATION_IMU_H

#include "sensor.h"
#include "sensor_msgs/Imu.h"
#include "leave/sensor/sensor_center.h"

/**
 * imu
 * /handsfree/imu
 */
class Imu : public Sensor<sensor_msgs::Imu> {
public:
    explicit Imu(const ros::NodeHandle &handle);

    ~Imu() override;

    void dateProgressing(sensor_msgs::Imu data) override;
};


#endif //APP_COMMUNICATION_IMU_H
