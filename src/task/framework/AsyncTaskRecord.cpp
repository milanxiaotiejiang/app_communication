//
// Created by Looper on 2023/2/6.
//

#include "task/framework/AsyncTaskRecord.h"

bool AsyncTaskRecord::isPause(const TaskStack &stack) {
    //todo
    return false;
}

bool AsyncTaskRecord::isFlowingWater(const TaskStack &stack) {
    //todo
    return false;
}

bool AsyncTaskRecord::isReturningBase(const TaskStack &stack) {
    //todo
    return false;
}

bool AsyncTaskRecord::isContinueWork(const TaskStack &stack) {
    //todo
    return false;
}

void AsyncTaskRecord::recordEmergencyStop(event::flow event_flow, const RealPoint &realPoint) {
    TaskStack stack(event_flow, realPoint);
    stopStack.push_back(stack);
    if (stopStack.size() > MAX_RECORD_TASK_STACK_SIZE) {
        stopStack.pop_front();
    }
}

void AsyncTaskRecord::recordSuspend(event::flow event_flow, const RealPoint &realPoint) {
    TaskStack stack(event_flow, realPoint);
    suspendStack.push_back(stack);
    if (suspendStack.size() > MAX_RECORD_TASK_STACK_SIZE) {
        suspendStack.pop_front();
    }
}

bool AsyncTaskRecord::recoverableEmergencyStop() {
    if (stopStack.empty())
        return false;
    const auto stack = lastEmergencyStop();
    if (!isContinueWork(stack))
        return false;
    LOG(INFO) << "handlePoint Stack : " << stack << " ...";
    return true;
}

bool AsyncTaskRecord::recoverableSuspend() {
    if (suspendStack.empty())
        return false;
    const auto stack = lastSuspend();
    if (!isContinueWork(stack))
        return false;
    LOG(INFO) << "handlePoint Stack : " << stack << " ...";
    return true;
}

TaskStack AsyncTaskRecord::lastEmergencyStop() {
    return stopStack.front();
}

TaskStack AsyncTaskRecord::lastSuspend() {
    return suspendStack.front();
}

void AsyncTaskRecord::release() {
    AsyncTaskFramework::release();
    stopStack.clear();
    suspendStack.clear();
}
