//
// Created by Looper on 2023/5/12.
//

#ifndef APP_COMMUNICATION_MOTOR_SERVER_H
#define APP_COMMUNICATION_MOTOR_SERVER_H

#include "ros/ros.h"
#include "std_srvs/Empty.h"
#include "thread"

class MotorServerSingleton {

private:
    bool initialize_start = false;
    ros::ServiceClient start_motor;

public:

    static MotorServerSingleton &instance() {
        static MotorServerSingleton instance;
        return instance;
    }

    void init(ros::NodeHandle handle);

    bool start();

    MotorServerSingleton(const MotorServerSingleton &) = delete;

    MotorServerSingleton &operator=(const MotorServerSingleton &) = delete;

    MotorServerSingleton(MotorServerSingleton &&) = delete;

    MotorServerSingleton &operator=(MotorServerSingleton &&) = delete;

private:

    MotorServerSingleton() = default;

    ~MotorServerSingleton() = default;
};


#endif //APP_COMMUNICATION_MOTOR_SERVER_H
