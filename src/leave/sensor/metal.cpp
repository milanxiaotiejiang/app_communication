//
// Created by Looper on 2023/7/17.
//

#include "leave/sensor/metal.h"

Metal::Metal(const ros::NodeHandle &handle) : Sensor(handle, "/mrrobot/on_ladder",
                                                     false, false, true) { outLog = false; }

Metal::~Metal() = default;

void Metal::dateProgressing(std_msgs::Int32 data) {
    if (outLog)
        LOG_IF(INFO, DEBUG_DUMP) << "自检 金属 " << data << " ...";

    if (deliveryCenter) {
        SensorCenter::instance().setMetalData(data.data);
    } else {
        RequestModel<int> requestModel(
                "publish", APP_MRROBOT_ON_LADDER, data.data
        );
        json jsonResult = requestModel;
        WsServerManager::instance().sendRequestData(APP_MRROBOT_ON_LADDER, jsonResult.dump());
    }
}
