//
// Created by Looper on 2023/7/17.
//

#include "leave/sensor/collision.h"

Collision::Collision(const ros::NodeHandle &handle) : Sensor(handle, "/mrrobot/bump_sensor",
                                                             false, false, true) { outLog = false; }

Collision::~Collision() = default;

void Collision::dateProgressing(std_msgs::UInt8MultiArray data) {
    unsigned long size = data.data.size();
    if (outLog) {
        std::string outStr;
        for (int i = 0; i < size; i++) {
            outStr.append("   " + std::to_string(i) + " " + std::to_string(data.data[i]));
        }
        LOG_IF(INFO, DEBUG_DUMP) << "自检 碰撞 " << outStr << "...";
    }

    std::vector<int> collisionData;
    for (const auto &item: data.data) {
        collisionData.push_back(item);
    }
    if (deliveryCenter) {
        SensorCenter::instance().setCollisionData(collisionData);
    } else {
        RequestModel<std::vector<int>> requestModel(
                "publish", APP_MRROBOT_BUMP_SENSOR, collisionData
        );
        json jsonResult = requestModel;
        WsServerManager::instance().sendRequestData(APP_MRROBOT_BUMP_SENSOR, jsonResult.dump());
    }
}
