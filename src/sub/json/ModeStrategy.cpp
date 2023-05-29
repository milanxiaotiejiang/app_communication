#include "sub/json/ModeStrategy.h"
#include "task/subscribe/async_machine.h"
#include "task/manager/manual.h"
#include "task/manager/NodeWorkModeManager.h"
#include "future/node/node_control.h"

string RobotTryEnterModeStrategy::handler(int params) {
    if (ManualManager::instance().taskRunning()) {
        throw app::exception(make_error_code(error::current_in_task));
    } else {
        if (!NodeWorkModeManager::instance().enterWorkMode(params)) {
            throw app::exception(make_error_code(error::mode_switching_is_not_supported));
        }
        return "";
    }
}

string RobotForceEnterModeStrategy::handler(int params) {
    ManualManager::instance().backToBase(true);
    NodeWorkModeManager::instance().forceEnterWorkMode(params);
    return "";
}

string RobotPreparetoWorkStrategy::handler(string params) {
    if (NodeControl::instance().isWork()) {
        return "";
    }
    if (!NodeWorkModeManager::instance().tryToWork()) {
        throw app::exception(make_error_code(error::mode_switching_is_not_supported));
    }
    return "";
}

string MapPreparetoWorkStrategy::handler(string params) {
    PublishOutManager::instance().publishMap(Variable::get_instance()->getMapApp());
    return "";
}
