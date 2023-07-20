//
// Created by Looper on 2023/7/17.
//

#ifndef APP_COMMUNICATION_COLLISION_H
#define APP_COMMUNICATION_COLLISION_H

#include "sensor.h"
#include "std_msgs/UInt8MultiArray.h"
#include "vector"
#include "leave/sensor/sensor_center.h"

/**
 * 碰撞
 * /mrrobot/bump_sensor
 */
class Collision : public Sensor<std_msgs::UInt8MultiArray> {
public:
    explicit Collision(const ros::NodeHandle &handle);

    ~Collision() override;

    void dateProgressing(std_msgs::UInt8MultiArray data) override;
};


#endif //APP_COMMUNICATION_COLLISION_H
