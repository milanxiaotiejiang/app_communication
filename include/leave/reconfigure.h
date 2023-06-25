//
// Created by Looper on 2023/2/22.
//

#ifndef APP_COMMUNICATION_RECONFIGURE_H
#define APP_COMMUNICATION_RECONFIGURE_H

#include <dynamic_reconfigure/Reconfigure.h>
#include <ros/ros.h>

#include <utility>
#include "simulation.h"

class DR {
public:
    DR(const std::string &serviceName, const std::string &paramName);

private:
    void call(std::string service_name, dynamic_reconfigure::Config config);

    std::string service_name;
    std::string param_name;

public:

    void d(double value)  ;

    void i(int value) ;

    void b(bool value) ;

};


#endif //APP_COMMUNICATION_RECONFIGURE_H
