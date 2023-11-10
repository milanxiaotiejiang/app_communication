//
// Created by Looper on 2023/7/17.
//

#ifndef APP_COMMUNICATION_METAL_H
#define APP_COMMUNICATION_METAL_H

#include "sensor.h"
#include "std_msgs/Int32.h"
#include "leave/sensor/sensor_center.h"

/**
 * 金属
 * /mrrobot/on_ladder
 */
class Metal : public Sensor<std_msgs::Int32 > {
public:
    explicit Metal(const ros::NodeHandle &handle);

    ~Metal() override;

    void dateProgressing(std_msgs::Int32 data) override;
};


#endif //APP_COMMUNICATION_METAL_H
