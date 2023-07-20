//
// Created by Looper on 2023/7/17.
//

#include "leave/sensor/radar.h"
#include "net/ros/RosLaserScan.h"
#include "net/ros/RosBasic.h"

Radar::Radar(const ros::NodeHandle &handle) : Sensor(handle, "/scan_raw") { outLog = false; }

Radar::~Radar() = default;

void Radar::dateProgressing(sensor_msgs::LaserScan data) {
    if (outLog)
        LOG_IF(INFO, DEBUG_DUMP) << "自检 雷达 " << data << "...";

    RosStamp stamp(data.header.stamp.nsec, data.header.stamp.sec);
    RosHeader header(data.header.frame_id, data.header.seq, stamp);

    RosLaserScan scan(header,
                      data.angle_min,
                      data.angle_max,
                      data.angle_increment,
                      data.time_increment,
                      data.scan_time,
                      data.range_min,
                      data.range_max,
                      data.ranges,
                      data.intensities);

    SensorCenter::instance().setScanData(scan);
}
