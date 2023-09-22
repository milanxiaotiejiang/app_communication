//
// Created by io on 23-9-20.
//

#include "segmentation/GateComprehensive.h"
#include "task/subscribe/zoo_inner_status.h"
#include "future/node/motor_server.h"
#include "manager/PublishInnerManager.h"
#include "BaseThrowable.h"

void GateSettingCenter::initialize(const ros::NodeHandle &handle) {
    gateSettingMode = false;
}

bool GateSettingCenter::isGateSettingMode() {
    return gateSettingMode;
}

void GateSettingCenter::startInspect() {
//    if (!ZooInnerStatus::instance().getIsCharging()) {
//        throw app::exception(make_error_code(error::please_ensure_to_start_end_the_self_at_the_base_station));
//    }
    if (ZooInnerStatus::instance().getUrgencyStopStatus()) {
        throw app::exception(make_error_code(error::please_ensure_to_start_end_the_self_non_emergency_stop_status));
    }

    std_msgs::Int32 map_start;
    map_start.data = 2;
    PublishInnerManager::instance().publishManualPush(map_start);

    MotorServerSingleton::instance().start();

    gateSettingMode = true;
}

void GateSettingCenter::stopInspect() {
//    if (!ZooInnerStatus::instance().getIsCharging()) {
//        throw app::exception(make_error_code(error::the_map_needs_to_be_saved_at_the_base_station_location));
//    }

    std_msgs::Int32 map_start;
    map_start.data = 0;
    PublishInnerManager::instance().publishManualPush(map_start);

    MotorServerSingleton::instance().stop();

    gateSettingMode = false;
}
