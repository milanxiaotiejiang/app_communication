//
// Created by lijiang on 2021/12/23.
//

#include "sub/json/ModeStrategy.h"
#include "task/subscribe/async_machine.h"
#include "task/manager/manual.h"

string RobotTryEnterModeStrategy::handler(int params) {
    if (AsyncMachine::instance().getFlow() != event::flow::waiting_for_task) {
        throw app::exception(make_error_code(error::current_in_task));
    } else {
        std_msgs::Int32 mode;
        mode.data = ((int32_t) params);
        PublishInnerManager::instance().publishMode(mode);
        return "";
    }
}

string RobotForceEnterModeStrategy::handler(int params) {
    ManualManager::instance().backToBase(true);
    std_msgs::Int32 mode;
    mode.data = ((int32_t) params);
    PublishInnerManager::instance().publishMode(mode);
    return "";
}

string RobotPreparetoWorkStrategy::handler(string params) {
    std_msgs::Int32 mode;
    mode.data = (int32_t) 2;
    PublishInnerManager::instance().publishMode(mode);
    return "";
}

string MapPreparetoWorkStrategy::handler(string params) {
    PublishOutManager::instance().publishMap(Variable::get_instance()->getMapApp());
    return "";
}
