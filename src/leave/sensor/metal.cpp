//
// Created by Looper on 2023/7/17.
//

#include "leave/sensor/metal.h"

Metal::Metal(const ros::NodeHandle &handle) : Sensor(handle, "/mrrobot/on_ladder") { outLog = false; }

Metal::~Metal() = default;

void Metal::dateProgressing(std_msgs::Int32 data) {
    if (outLog)
        LOG_IF(INFO, DEBUG_DUMP) << "自检 金属 " << data << " ...";

    SensorCenter::instance().setMetalData(data.data);
}
