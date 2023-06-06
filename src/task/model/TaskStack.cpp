//
// Created by Looper on 2023/2/6.
//

#include "task/model/TaskStack.h"

TaskStack::TaskStack(event::flow flow, const RealBlock &suspendBlock) : flow(flow), suspendBlock(suspendBlock) {}

std::ostream &operator<<(std::ostream &os, const TaskStack &stack) {
    os << "flow: " << stack.flow << " suspendBlock: " << stack.suspendBlock;
    return os;
}
