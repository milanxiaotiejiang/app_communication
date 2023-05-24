//
// Created by Looper on 2023/2/6.
//

#ifndef APP_COMMUNICATION_TASKSTACK_H
#define APP_COMMUNICATION_TASKSTACK_H

#include <ostream>
#include "task/status/state_machine.h"
#include "task/RealBlock.h"

class TaskStack {
public:
    event::flow flow;
    RealBlock suspendPoint;

    TaskStack(event::flow flow, const RealBlock &suspendPoint);

    friend std::ostream &operator<<(std::ostream &os, const TaskStack &stack);
};


#endif //APP_COMMUNICATION_TASKSTACK_H
