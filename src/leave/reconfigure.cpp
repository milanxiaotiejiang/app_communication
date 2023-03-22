//
// Created by Looper on 2023/2/22.
//

#include "leave/reconfigure.h"
#include "simulation.h"

#include <utility>

DR::DR(const std::string &serviceName, const std::string &paramName) : service_name(serviceName),
                                                                       param_name(paramName) {}


void DR::call(std::string service_name, dynamic_reconfigure::Config config) {
    dynamic_reconfigure::ReconfigureResponse reconfigureResponse;

    dynamic_reconfigure::ReconfigureRequest reconfigureRequest;
    reconfigureRequest.config = std::move(config);

    if (Environment::instance().isRealEnvironment) {
        ros::service::call(service_name + "/set_parameters", reconfigureRequest, reconfigureResponse);
    }
}

void DR::d(double value) {
    dynamic_reconfigure::DoubleParameter parameter;
    parameter.name = param_name;
    parameter.value = value;

    dynamic_reconfigure::Config config;
    config.doubles.push_back(parameter);

    call(service_name, config);
}

void DR::i(int value) {
    dynamic_reconfigure::IntParameter parameter;
    parameter.name = param_name;
    parameter.value = value;

    dynamic_reconfigure::Config config;
    config.ints.push_back(parameter);

    call(service_name, config);
}

void DR::b(bool value) {
    dynamic_reconfigure::BoolParameter parameter;
    parameter.name = param_name;
    parameter.value = value;

    dynamic_reconfigure::Config config;
    config.bools.push_back(parameter);

    call(service_name, config);
}
