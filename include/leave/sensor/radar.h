//
// Created by Looper on 2023/7/17.
//

#ifndef APP_COMMUNICATION_RADAR_H
#define APP_COMMUNICATION_RADAR_H

#include "sensor.h"
#include "sensor_msgs/LaserScan.h"
#include "leave/sensor/sensor_center.h"
#include "leave/sensor/sensor_center.h"

/**
 * 雷达
 * /scan_raw
 */
class Radar : public Sensor<sensor_msgs::LaserScan> {
public:
    explicit Radar(const ros::NodeHandle &handle);

    ~Radar() override;

    void dateProgressing(sensor_msgs::LaserScan data) override;

    void subscribeCallback(const sensor_msgs::LaserScan &data) override;
};


#endif //APP_COMMUNICATION_RADAR_H
