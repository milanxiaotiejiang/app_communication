//
// Created by Looper on 2023/7/17.
//

#ifndef APP_COMMUNICATION_TOF_H
#define APP_COMMUNICATION_TOF_H

#include "sensor.h"
#include "sensor_msgs/Range.h"
#include "leave/sensor/sensor_center.h"

/**
 * tof 2
 * /mrrobot/ls_front_left
 * /mrrobot/ls_front_right
 */
class TofLeft : public Sensor<sensor_msgs::Range> {
public:
    explicit TofLeft(const ros::NodeHandle &handle);

    ~TofLeft() override;

    void dateProgressing(sensor_msgs::Range data) override;
};

class TofRight : public Sensor<sensor_msgs::Range> {
public:
    explicit TofRight(const ros::NodeHandle &handle);

    ~TofRight() override;

    void dateProgressing(sensor_msgs::Range data) override;
};


#endif //APP_COMMUNICATION_TOF_H
