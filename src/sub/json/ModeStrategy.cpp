//
// Created by lijiang on 2021/12/23.
//

#include "sub/json/ModeStrategy.h"
#include "task/subscribe/async_machine.h"
#include "task/manager/manual.h"
#include "task/manager/NodeWorkModeManager.h"

/*
 *  mode:0:建图；1修改地图；2工作
 */
string RobotTryEnterModeStrategy::handler(int params) {
    if (ManualManager::instance().taskRunning()) {
        throw app::exception(make_error_code(error::current_in_task));
    } else {
        NodeWorkModeManager::instance().enterWorkMode(params);
        return "";
    }
}

string RobotForceEnterModeStrategy::handler(int params) {
    ManualManager::instance().backToBase(true);
    NodeWorkModeManager::instance().enterWorkMode(params);
    return "";
}

string RobotPreparetoWorkStrategy::handler(string params) {
    NodeWorkModeManager::instance().setWorkMode(WorkMode::WORKING);
    return "";
}

string MapPreparetoWorkStrategy::handler(string params) {
    PublishOutManager::instance().publishMap(Variable::get_instance()->getMapApp());
    return "";
}
