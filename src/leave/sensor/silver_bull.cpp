//
// Created by Looper on 2023/7/17.
//

#include "leave/sensor/silver_bull.h"
#include "net/ros/RosPointCloud2.h"

SilverBullUp::SilverBullUp(const ros::NodeHandle &handle) : Sensor(handle, "/2/depth/depth2pc",
                                                                   false, false, true) { outLog = false; }

SilverBullUp::~SilverBullUp() = default;

void SilverBullUp::dateProgressing(sensor_msgs::PointCloud2 data) {
    if (outLog)
        LOG_IF(INFO, DEBUG_DUMP) << "自检 银牛上 " << data << " ...";

    RosStamp stamp(data.header.stamp.nsec, data.header.stamp.sec);
    RosHeader header(data.header.frame_id, data.header.seq, stamp);

    std::vector<RosPointField> fields;
    for (const auto &item: data.fields) {
        fields.push_back(RosPointField(item.name, item.offset, item.datatype, item.count));
    }
    std::vector<int> datas;
    for (const auto &item: data.data) {
        datas.push_back(item);
    }
    RosPointCloud2 rosPointCloud2(header, data.height, data.width, fields,
                                  data.is_bigendian, data.point_step, data.row_step, datas, data.is_dense);

//    SensorCenter::instance().setSilverUpData(rosPointCloud2);

    RequestModel<RosPointCloud2> requestModel(
            "publish", APP_2_DEPTH_DEPTH2PC, rosPointCloud2
    );
    json jsonResult = requestModel;
    WsServerManager::instance().sendRequestData(APP_2_DEPTH_DEPTH2PC, jsonResult.dump());
}

SilverBullDown::SilverBullDown(const ros::NodeHandle &handle) : Sensor(handle, "/1/depth/depth2pc",
                                                                       false, false, true) { outLog = false; }

SilverBullDown::~SilverBullDown() = default;

void SilverBullDown::dateProgressing(sensor_msgs::PointCloud2 data) {
    if (outLog)
        LOG_IF(INFO, DEBUG_DUMP) << "自检 银牛下 " << data << " ...";

    RosStamp stamp(data.header.stamp.nsec, data.header.stamp.sec);
    RosHeader header(data.header.frame_id, data.header.seq, stamp);

    std::vector<RosPointField> fields;
    for (const auto &item: data.fields) {
        fields.push_back(RosPointField(item.name, item.offset, item.datatype, item.count));
    }
    std::vector<int> datas;
    for (const auto &item: data.data) {
        datas.push_back(item);
    }
    RosPointCloud2 rosPointCloud2(header, data.height, data.width, fields,
                                  data.is_bigendian, data.point_step, data.row_step, datas, data.is_dense);

    if (deliveryCenter) {
        SensorCenter::instance().setSilverDownData(rosPointCloud2);
    } else {
        RequestModel<RosPointCloud2> requestModel(
                "publish", APP_1_DEPTH_DEPTH2PC, rosPointCloud2
        );
        json jsonResult = requestModel;
        WsServerManager::instance().sendRequestData(APP_1_DEPTH_DEPTH2PC, jsonResult.dump());
    }
}
