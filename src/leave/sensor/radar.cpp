//
// Created by Looper on 2023/7/17.
//

#include "leave/sensor/radar.h"
#include "net/ros/RosLaserScan.h"
#include "net/ros/RosBasic.h"

void Radar::subscribeCallback(const sensor_msgs::LaserScan &data) {
    Sensor::subscribeCallback(data);
}

Radar::Radar(const ros::NodeHandle &handle) : Sensor(handle, "/scan_raw", true, false, false) { outLog = false; }

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
                      data.intensities,
                      data.ranges.size(),
                      data.intensities.size());

    if (deliveryCenter) {
        SensorCenter::instance().setScanData(scan);
    } else {
        RequestModel<RosLaserScan> requestModel(
                "publish", APP_SCAN_RAW, scan
        );
        json jsonResult = requestModel;
//        LOG_IF(INFO, DEBUG_DUMP) << "自检 雷达 " << jsonResult.dump() << "...";
        WsServerManager::instance().sendRequestData(APP_SCAN_RAW, jsonResult.dump());
    }
}
