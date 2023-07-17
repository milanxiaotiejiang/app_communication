//
// Created by Looper on 2023/7/17.
//

#include "leave/sensor/sensor_center.h"

void SensorCenter::initialize(const ros::NodeHandle &handle) {
    carpet = std::make_shared<Carpet>(handle);
}

bool SensorCenter::isSensorSelfMode() {
    return sensorSelfMode;
}

void SensorCenter::startInspect() {
    if (!sensorSelfMode) {
        sensorSelfMode = true;
        carpet->startInspect();
    }
}

void SensorCenter::stopInspect() {
    if (sensorSelfMode) {
        sensorSelfMode = false;
        carpet->stopInspect();
    }
}
