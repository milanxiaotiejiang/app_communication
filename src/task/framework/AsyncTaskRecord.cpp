//
// Created by Looper on 2023/2/6.
//

#include "task/framework/AsyncTaskRecord.h"

bool AsyncTaskRecord::isPause() {
    return epoll_manual == loop::manual_epoll::manual_pause;
}

bool AsyncTaskRecord::isPreparation(event::flow flow) {
    return flow == event::flow::out_base_station ||
           flow == event::flow::switch_node_work_mode ||
           flow == event::flow::preliminary_preparation_completed;
}

bool AsyncTaskRecord::isFlowingWater(event::flow flow) {
    return flow == event::flow::cleaning_mechanism_ready ||
           flow == event::flow::ensure_move_to_start_point ||
           flow == event::flow::flowing_water_production;
}

bool AsyncTaskRecord::isReturningBase(event::flow flow) {
    return flow == event::flow::arrive_base_point_success ||
           flow == event::flow::flowing_water_execution_completed ||
           flow == event::flow::arrive_base_station_success ||
           flow == event::flow::try_recharging_again ||
           flow == event::flow::try_move_base_point_again;
}

bool AsyncTaskRecord::isContinueWork(event::flow flow, bool suspend) {
    if (isManualMode()) {
        return false;
    }
    if (isUnrecoverableError()) {
        return false;
    }
    if (suspend) {
        return isFlowingWater(flow) || isReturningBase(flow);
    } else {
        return isPreparation(flow) || isFlowingWater(flow) || isReturningBase(flow);
    }
}

void AsyncTaskRecord::recordEmergencyStop(event::flow event_flow, const RealPoint &realPoint) {
    TaskStack stack(event_flow, realPoint);
    stopStack.push_back(stack);
    if (stopStack.size() > MAX_RECORD_TASK_STACK_SIZE) {
        stopStack.pop_front();
    }
}

bool AsyncTaskRecord::recoverableEmergencyStop() {
    if (stopStack.empty())
        return false;
    if (!isContinueWork(lastEmergencyStop().flow, false))
        return false;
    return true;
}

bool AsyncTaskRecord::recoverableSuspend() {
    if (stopStack.empty())
        return false;
    return isContinueWork(lastEmergencyStop().flow, true);
}

TaskStack AsyncTaskRecord::lastEmergencyStop() {
    return stopStack.back();
}

void AsyncTaskRecord::release() {
    AsyncTaskFramework::release();
    stopStack.clear();
}
