//
// Created by Looper on 2023/7/17.
//

#ifndef APP_COMMUNICATION_ULTRASONIC_H
#define APP_COMMUNICATION_ULTRASONIC_H

#include "sensor.h"
#include "sensor_msgs/Range.h"
#include "leave/sensor/sensor_center.h"

/**
 * 超声 8
 * /mrrobot/ul_sensor1
 * /mrrobot/ul_sensor2
 * /mrrobot/ul_sensor3
 * /mrrobot/ul_sensor4
 * /mrrobot/ul_sensor5
 * /mrrobot/ul_sensor6
 * /mrrobot/ul_sensor7
 * /mrrobot/ul_sensor8
 */
class Ultrasonic1 : public Sensor<sensor_msgs::Range> {
public:
    explicit Ultrasonic1(const ros::NodeHandle &handle);

    ~Ultrasonic1() override = default;

    void dateProgressing(sensor_msgs::Range data) override;
};

class Ultrasonic2 : public Sensor<sensor_msgs::Range> {
public:
    explicit Ultrasonic2(const ros::NodeHandle &handle);

    ~Ultrasonic2() override = default;

    void dateProgressing(sensor_msgs::Range data) override;
};

class Ultrasonic3 : public Sensor<sensor_msgs::Range> {
public:
    explicit Ultrasonic3(const ros::NodeHandle &handle);

    ~Ultrasonic3() override = default;

    void dateProgressing(sensor_msgs::Range data) override;
};

class Ultrasonic4 : public Sensor<sensor_msgs::Range> {
public:
    explicit Ultrasonic4(const ros::NodeHandle &handle);

    ~Ultrasonic4() override = default;

    void dateProgressing(sensor_msgs::Range data) override;
};


#endif //APP_COMMUNICATION_ULTRASONIC_H
