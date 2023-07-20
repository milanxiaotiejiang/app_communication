//
// Created by Looper on 2023/7/17.
//

#include "leave/sensor/tof.h"
#include "net/ros/RosRange.h"

TofLeft::TofLeft(const ros::NodeHandle &handle) : Sensor(handle, "/mrrobot/ls_front_left") { outLog = false; }

TofLeft::~TofLeft() = default;

void TofLeft::dateProgressing(sensor_msgs::Range data) {
    if (outLog)
        LOG_IF(INFO, DEBUG_DUMP) << "自检 top left " << data << " ...";

    RosStamp stamp(data.header.stamp.nsec, data.header.stamp.sec);
    RosHeader header(data.header.frame_id, data.header.seq, stamp);

    RosRange rosRange(header, data.radiation_type, data.field_of_view, data.min_range, data.max_range, data.range);

    SensorCenter::instance().setTofLeftData(rosRange);
}

TofRight::TofRight(const ros::NodeHandle &handle) : Sensor(handle, "/mrrobot/ls_front_right") { outLog = false; }

TofRight::~TofRight() = default;

void TofRight::dateProgressing(sensor_msgs::Range data) {
    if (outLog)
        LOG_IF(INFO, DEBUG_DUMP) << "自检 top right " << data << " ...";

    RosStamp stamp(data.header.stamp.nsec, data.header.stamp.sec);
    RosHeader header(data.header.frame_id, data.header.seq, stamp);

    RosRange rosRange(header, data.radiation_type, data.field_of_view, data.min_range, data.max_range, data.range);

    SensorCenter::instance().setTofRightData(rosRange);
}
