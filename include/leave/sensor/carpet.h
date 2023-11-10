//
// Created by Looper on 2023/7/17.
//

#ifndef APP_COMMUNICATION_CARPET_H
#define APP_COMMUNICATION_CARPET_H

#include "sensor.h"
#include "std_msgs/Int32.h"
#include "leave/sensor/sensor_center.h"

/**
 * 地毯
 * /mrrobot/carpet_detect
 */
class Carpet : public Sensor<std_msgs::Int32> {
public:
    explicit Carpet(const ros::NodeHandle &handle);

    ~Carpet() override;

    void dateProgressing(std_msgs::Int32 data) override;
};


#endif //APP_COMMUNICATION_CARPET_H
