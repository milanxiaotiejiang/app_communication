//
// Created by Looper on 2023/7/17.
//

#include "leave/sensor/carpet.h"

Carpet::Carpet(const ros::NodeHandle &handle) : Sensor(handle, "/mrrobot/carpet_detect",
                                                       false, false, true) {
    outLog = false;
}

Carpet::~Carpet() = default;

void Carpet::dateProgressing(std_msgs::Int32 data) {
    if (outLog)
        LOG_IF(INFO, DEBUG_DUMP) << "自检 地毯 " << data << "...";

    if (deliveryCenter) {
        SensorCenter::instance().setCarpetData(data.data);
    } else {
        RequestModel<int> requestModel(
                "publish", APP_MRROBOT_CARPET_DETECT, data.data
        );
        json jsonResult = requestModel;
        WsServerManager::instance().sendRequestData(APP_MRROBOT_CARPET_DETECT, jsonResult.dump());
    }
}
