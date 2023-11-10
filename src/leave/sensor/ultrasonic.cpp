//
// Created by Looper on 2023/7/17.
//

#include "leave/sensor/ultrasonic.h"
#include "net/ros/RosRange.h"

Ultrasonic1::Ultrasonic1(const ros::NodeHandle &handle) : Sensor(handle, "/mrrobot/ul_sensor1",
                                                                 false, false, true) { outLog = false; }

void Ultrasonic1::dateProgressing(sensor_msgs::Range data) {
    if (outLog)
        LOG_IF(INFO, DEBUG_DUMP) << "自检 超声 1 " << data << " ...";

    RosStamp stamp(data.header.stamp.nsec, data.header.stamp.sec);
    RosHeader header(data.header.frame_id, data.header.seq, stamp);

    RosRange rosRange(header, data.radiation_type, data.field_of_view, data.min_range, data.max_range, data.range);

    if (deliveryCenter) {
        SensorCenter::instance().setUltrasonic1Data(rosRange);
    } else {
        RequestModel<RosRange> requestModel(
                "publish", APP_MRROBOT_UL_SENSOR1, rosRange
        );
        json jsonResult = requestModel;
        WsServerManager::instance().sendRequestData(APP_MRROBOT_UL_SENSOR1, jsonResult.dump());
    }
}

Ultrasonic2::Ultrasonic2(const ros::NodeHandle &handle) : Sensor(handle, "/mrrobot/ul_sensor2",
                                                                 false, false, true) { outLog = false; }

void Ultrasonic2::dateProgressing(sensor_msgs::Range data) {
    if (outLog)
        LOG_IF(INFO, DEBUG_DUMP) << "自检 超声 2 " << data << " ...";

    RosStamp stamp(data.header.stamp.nsec, data.header.stamp.sec);
    RosHeader header(data.header.frame_id, data.header.seq, stamp);

    RosRange rosRange(header, data.radiation_type, data.field_of_view, data.min_range, data.max_range, data.range);

    if (deliveryCenter) {
        SensorCenter::instance().setUltrasonic2Data(rosRange);
    } else {
        RequestModel<RosRange> requestModel(
                "publish", APP_MRROBOT_UL_SENSOR2, rosRange
        );
        json jsonResult = requestModel;
        WsServerManager::instance().sendRequestData(APP_MRROBOT_UL_SENSOR2, jsonResult.dump());
    }
}

Ultrasonic3::Ultrasonic3(const ros::NodeHandle &handle) : Sensor(handle, "/mrrobot/ul_sensor3",
                                                                 false, false, true) { outLog = false; }

void Ultrasonic3::dateProgressing(sensor_msgs::Range data) {
    if (outLog)
        LOG_IF(INFO, DEBUG_DUMP) << "自检 超声 3 " << data << " ...";

    RosStamp stamp(data.header.stamp.nsec, data.header.stamp.sec);
    RosHeader header(data.header.frame_id, data.header.seq, stamp);

    RosRange rosRange(header, data.radiation_type, data.field_of_view, data.min_range, data.max_range, data.range);

    if (deliveryCenter) {
        SensorCenter::instance().setUltrasonic3Data(rosRange);
    } else {
        RequestModel<RosRange> requestModel(
                "publish", APP_MRROBOT_UL_SENSOR3, rosRange
        );
        json jsonResult = requestModel;
        WsServerManager::instance().sendRequestData(APP_MRROBOT_UL_SENSOR3, jsonResult.dump());
    }
}

Ultrasonic4::Ultrasonic4(const ros::NodeHandle &handle) : Sensor(handle, "/mrrobot/ul_sensor4",
                                                                 false, false, true) { outLog = false; }

void Ultrasonic4::dateProgressing(sensor_msgs::Range data) {
    if (outLog)
        LOG_IF(INFO, DEBUG_DUMP) << "自检 超声 4 " << data << " ...";

    RosStamp stamp(data.header.stamp.nsec, data.header.stamp.sec);
    RosHeader header(data.header.frame_id, data.header.seq, stamp);

    RosRange rosRange(header, data.radiation_type, data.field_of_view, data.min_range, data.max_range, data.range);

    if (deliveryCenter) {
        SensorCenter::instance().setUltrasonic4Data(rosRange);
    } else {
        RequestModel<RosRange> requestModel(
                "publish", APP_MRROBOT_UL_SENSOR4, rosRange
        );
        json jsonResult = requestModel;
        WsServerManager::instance().sendRequestData(APP_MRROBOT_UL_SENSOR4, jsonResult.dump());
    }
}
