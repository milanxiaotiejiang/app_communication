//
// Created by Looper on 2023/7/17.
//

#include "leave/sensor/ultrasonic.h"
#include "net/ros/RosRange.h"

Ultrasonic1::Ultrasonic1(const ros::NodeHandle &handle) : Sensor(handle, "/mrrobot/ul_sensor1") { outLog = false; }

void Ultrasonic1::dateProgressing(sensor_msgs::Range data) {
    if (outLog)
        LOG_IF(INFO, DEBUG_DUMP) << "自检 超声 1 " << data << " ...";

    RosStamp stamp(data.header.stamp.nsec, data.header.stamp.sec);
    RosHeader header(data.header.frame_id, data.header.seq, stamp);

    RosRange rosRange(header, data.radiation_type, data.field_of_view, data.min_range, data.max_range, data.range);

    SensorCenter::instance().setUltrasonic1Data(rosRange);
}

Ultrasonic2::Ultrasonic2(const ros::NodeHandle &handle) : Sensor(handle, "/mrrobot/ul_sensor2") { outLog = false; }

void Ultrasonic2::dateProgressing(sensor_msgs::Range data) {
    if (outLog)
        LOG_IF(INFO, DEBUG_DUMP) << "自检 超声 2 " << data << " ...";

    RosStamp stamp(data.header.stamp.nsec, data.header.stamp.sec);
    RosHeader header(data.header.frame_id, data.header.seq, stamp);

    RosRange rosRange(header, data.radiation_type, data.field_of_view, data.min_range, data.max_range, data.range);

    SensorCenter::instance().setUltrasonic2Data(rosRange);
}

Ultrasonic3::Ultrasonic3(const ros::NodeHandle &handle) : Sensor(handle, "/mrrobot/ul_sensor3") { outLog = false; }

void Ultrasonic3::dateProgressing(sensor_msgs::Range data) {
    if (outLog)
        LOG_IF(INFO, DEBUG_DUMP) << "自检 超声 3 " << data << " ...";

    RosStamp stamp(data.header.stamp.nsec, data.header.stamp.sec);
    RosHeader header(data.header.frame_id, data.header.seq, stamp);

    RosRange rosRange(header, data.radiation_type, data.field_of_view, data.min_range, data.max_range, data.range);

    SensorCenter::instance().setUltrasonic3Data(rosRange);
}

Ultrasonic4::Ultrasonic4(const ros::NodeHandle &handle) : Sensor(handle, "/mrrobot/ul_sensor4") { outLog = false; }

void Ultrasonic4::dateProgressing(sensor_msgs::Range data) {
    if (outLog)
        LOG_IF(INFO, DEBUG_DUMP) << "自检 超声 4 " << data << " ...";

    RosStamp stamp(data.header.stamp.nsec, data.header.stamp.sec);
    RosHeader header(data.header.frame_id, data.header.seq, stamp);

    RosRange rosRange(header, data.radiation_type, data.field_of_view, data.min_range, data.max_range, data.range);

    SensorCenter::instance().setUltrasonic4Data(rosRange);
}
