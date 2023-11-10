//
// Created by Looper on 2023/5/12.
//

#include "future/node/motor_server.h"
#include "simulation.h"
#include "simulation.h"

void MotorServerSingleton::init(ros::NodeHandle handle) {
    if (!Environment::instance().isRealEnvironment) {
        return;
    }
    LOG_IF(INFO, DEBUG_NODE) << "MotorServerSingleton  init ... ";
    start_motor = handle.serviceClient<std_srvs::Empty>("start_motor");
    start_motor.waitForExistence();
    stop_motor = handle.serviceClient<std_srvs::Empty>("stop_motor");
    stop_motor.waitForExistence();
    LOG_IF(INFO, DEBUG_NODE) << "MotorServerSingleton  init start_motor stop_motor complete ... ";
    initialize_start = true;
}

bool MotorServerSingleton::start() {
    if (!Environment::instance().isRealEnvironment) {
        return true;
    }
    LOG_IF(INFO, DEBUG_NODE) << "MotorServerSingleton  start_motor 开始 start 雷达 ... ";
    if (!initialize_start) {
        return false;
    }
    std_srvs::Empty start_cmd;
    bool callResult = start_motor.call(start_cmd);
    LOG_IF(INFO, DEBUG_NODE) << "MotorServerSingleton  callResult " << callResult << " ... ";
    return callResult;
}

bool MotorServerSingleton::stop() {
    if (!Environment::instance().isRealEnvironment) {
        return true;
    }
    LOG_IF(INFO, DEBUG_NODE) << "MotorServerSingleton  stop_motor 开始 stop 雷达 ... ";
    if (!initialize_start) {
        return false;
    }
    std_srvs::Empty stop_cmd;
    bool callResult = stop_motor.call(stop_cmd);
    LOG_IF(INFO, DEBUG_NODE) << "MotorServerSingleton  callResult " << callResult << " ... ";
    return callResult;
}
