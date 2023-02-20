//
// Created by Looper on 2023/2/6.
//

#include "task/model/TaskStack.h"

TaskStack::TaskStack(event::flow flow, const RealPoint &suspendPoint) : flow(flow), suspendPoint(suspendPoint) {}

std::ostream &operator<<(std::ostream &os, const TaskStack &stack) {
    os << "flow: " << stack.flow << " suspendPoint: " << stack.suspendPoint;
    return os;
}
