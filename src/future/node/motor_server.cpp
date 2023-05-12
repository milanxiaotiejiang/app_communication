//
// Created by Looper on 2023/5/12.
//

#include "future/node/motor_server.h"
#include "simulation.h"
#include "glog/logging.h"

void MotorServerSingleton::init(ros::NodeHandle handle) {
    std::thread startServiceClientThread([this, &handle]() {
        start_motor = handle.serviceClient<std_srvs::Empty>("start_motor");
        start_motor.waitForExistence();
        initialize_start = true;
    });
    startServiceClientThread.detach();
}

bool MotorServerSingleton::start() {
    if (!Environment::instance().isRealEnvironment) {
        return true;
    }
    LOG(INFO) << "ModeValidate  start_motor 开始启动雷达 ------------------------------ ";
    if (initialize_start) {
        return false;
    }
    std_srvs::Empty start_cmd;
    return start_motor.call(start_cmd);
}
