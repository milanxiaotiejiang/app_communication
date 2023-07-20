//
// Created by Looper on 2023/7/17.
//

#ifndef APP_COMMUNICATION_SILVER_BULL_H
#define APP_COMMUNICATION_SILVER_BULL_H

#include "sensor.h"
#include "sensor_msgs/PointCloud2.h"
#include "leave/sensor/sensor_center.h"

/**
 * 银牛 2
 * /1/depth/depth2pc 下
 * /2/depth/depth2pc 上
 */
class SilverBullUp : public Sensor<sensor_msgs::PointCloud2> {
public:
    explicit SilverBullUp(const ros::NodeHandle &handle);

    ~SilverBullUp() override;

    void dateProgressing(sensor_msgs::PointCloud2 data) override;
};

class SilverBullDown : public Sensor<sensor_msgs::PointCloud2> {
public:
    explicit SilverBullDown(const ros::NodeHandle &handle);

    ~SilverBullDown() override;

    void dateProgressing(sensor_msgs::PointCloud2 data) override;
};

#endif //APP_COMMUNICATION_SILVER_BULL_H
