//
// Created by Looper on 2023/2/6.
//

#ifndef APP_COMMUNICATION_TASKSTACK_H
#define APP_COMMUNICATION_TASKSTACK_H


#include <ostream>
#include "task/status/state_machine.h"
#include "task/RealPoint.h"

class TaskStack {
    event::flow flow;
    RealPoint suspendPoint;
public:
    TaskStack(event::flow flow, const RealPoint &suspendPoint);

    friend std::ostream &operator<<(std::ostream &os, const TaskStack &stack);
};


#endif //APP_COMMUNICATION_TASKSTACK_H
