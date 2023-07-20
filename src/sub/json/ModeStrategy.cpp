//
// Created by lijiang on 2021/12/18.
//

#include "sub/json/ModeStrategy.h"
#include "task/subscribe/async_machine.h"
#include "task/manager/manual.h"
#include "task/manager/NodeWorkModeManager.h"
#include "future/node/node_control.h"
#include "leave/sensor/sensor_center.h"

std::string RobotTryEnterModeStrategy::handler(int params) {
    if (ManualManager::instance().taskRunning()) {
        throw app::exception(make_error_code(error::current_in_task));
    } else {
        if (!NodeWorkModeManager::instance().enterWorkMode(params)) {
            throw app::exception(make_error_code(error::mode_switching_is_not_supported));
        }
        return "";
    }
}

std::string RobotForceEnterModeStrategy::handler(int params) {
    ManualManager::instance().backToBase(true);
    NodeWorkModeManager::instance().forceEnterWorkMode(params);
    return "";
}

std::string RobotPreparetoWorkStrategy::handler(std::string params) {
//    if (NodeControl::instance().isWork()) {
//        return "";
//    }
//    if (!NodeWorkModeManager::instance().tryToWork()) {
//        throw app::exception(make_error_code(error::mode_switching_is_not_supported));
//    }

    NodeWorkModeManager::instance().toSleep();

    return "";
}

std::string MapPreparetoWorkStrategy::handler(std::string params) {
    PublishOutManager::instance().publishMap(Variable::get_instance()->getMapApp());
    return "";
}

std::string OpenSelfCheckStrategy::handler(std::string params) {
    SensorCenter::instance().startInspect();
    return "";
}

std::string CloseSelfCheckStrategy::handler(std::string params) {
    SensorCenter::instance().stopInspect();
    return "";
}
