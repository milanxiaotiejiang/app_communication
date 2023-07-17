//
// Created by Looper on 2023/7/17.
//

#ifndef APP_COMMUNICATION_SENSOR_CENTER_H
#define APP_COMMUNICATION_SENSOR_CENTER_H

#include "ros/ros.h"
#include "leave/sensor/carpet.h"

class SensorCenter {
private:
    SensorCenter() = default;

    SensorCenter(SensorCenter &) = delete;

    SensorCenter &operator=(const SensorCenter &) = delete;

    bool sensorSelfMode = false;

    std::shared_ptr<Carpet> carpet;

public:
    ~SensorCenter() = default;

    static auto &instance() {
        static SensorCenter obj;
        return obj;
    }

    void initialize(const ros::NodeHandle &handle);

    bool isSensorSelfMode();

    void startInspect();

    void stopInspect();
};


#endif //APP_COMMUNICATION_SENSOR_CENTER_H
