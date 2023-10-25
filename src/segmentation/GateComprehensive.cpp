//
// Created by io on 23-9-20.
//

#include "segmentation/GateComprehensive.h"
#include "task/subscribe/zoo_inner_status.h"
#include "future/node/motor_server.h"
#include "manager/PublishInnerManager.h"
#include "BaseThrowable.h"
#include "task/manager/NodeWorkModeManager.h"

void GateSettingCenter::initialize(const ros::NodeHandle &handle) {
    gateSettingMode = false;
}

bool GateSettingCenter::isGateSettingMode() {
    return gateSettingMode;
}

void GateSettingCenter::startInspect() {
    if (Environment::instance().isRealEnvironment) {
        if (!ZooInnerStatus::instance().getIsCharging()) {
            throw app::exception(make_error_code(error::please_ensure_that_the_gate_setting_is_turned_on_off_at_the_base_station));
        }

        if (ZooInnerStatus::instance().getUrgencyStopStatus()) {
            throw app::exception(make_error_code(error::please_ensure_to_start_end_the_self_non_emergency_stop_status));
        }

        std_msgs::Int32 map_start;
        map_start.data = 2;
        PublishInnerManager::instance().publishManualPush(map_start);

        if (!NodeWorkModeManager::instance().enterWorkMode(2)) {
            throw app::exception(make_error_code(error::mode_switching_is_not_supported));
        }
    }

    gateSettingMode = true;
}

void GateSettingCenter::stopInspect() {
    if (Environment::instance().isRealEnvironment) {
        if (!ZooInnerStatus::instance().getIsCharging()) {
            throw app::exception(make_error_code(error::the_map_needs_to_be_saved_at_the_base_station_location));
        }

        std_msgs::Int32 map_start;
        map_start.data = 0;
        PublishInnerManager::instance().publishManualPush(map_start);

        NodeWorkModeManager::instance().toSleep();
    }

    gateSettingMode = false;
}
