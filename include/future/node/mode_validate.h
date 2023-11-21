//
// Created by Looper on 2023/4/11.
//

#ifndef APP_COMMUNICATION_MODE_VALIDATE_H
#define APP_COMMUNICATION_MODE_VALIDATE_H

#include <ros/ros.h>
#include "machine.h"
#include "future/thread_pool.h"

class ModeValidate {
private:

    async::ThreadPool pool_;

    static int getMoveBaseMode();

public:
    static bool validateCartographer(node::State state);

    static bool validateMoveBase(int open);

    static bool validateMoveBaseAvailable();

    static bool validateCoreMoveAvailable();

    static bool validateHardwareServer();

    static void validateMotorServer(int stepId, std::atomic<bool> &stopFlag, std::promise<bool> &promise);

    static void validateInuServer1(int stepId, std::atomic<bool> &stopFlag, std::promise<bool> &promise);

    static void validateInuServer2(int stepId, std::atomic<bool> &stopFlag, std::promise<bool> &promise);

};


#endif //APP_COMMUNICATION_MODE_VALIDATE_H
