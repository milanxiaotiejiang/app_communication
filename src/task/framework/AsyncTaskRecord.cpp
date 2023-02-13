//
// Created by Looper on 2023/2/6.
//

#include "task/framework/AsyncTaskRecord.h"

bool AsyncTaskRecord::isWaitTask(event::flow flow) {
    return flow == event::flow::waiting_for_task;
}

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
           flow == event::flow::try_move_base_point_again ||
           (flow == event::flow::flowing_water_production && plannerQueue.empty());
}

bool AsyncTaskRecord::isContinueWork(event::flow flow, bool suspend) {
    LOG(INFO) << "AsyncTaskRecord : lastEmergencyStop : " << lastEmergencyStop();
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

bool AsyncTaskRecord::isRegularTask(event::flow flow) {
    if (isUnrecoverableError()) {
        return false;
    }
    if (flow == event::flow::waiting_for_task) {
        return false;
    }
    if (flow == event::flow::hardware_interrupt_task) {
        return false;
    }
    if (flow == event::flow::software_interrupt_task) {
        return false;
    }
    return true;
}

bool AsyncTaskRecord::isManualTask(const std::string &launchPeople) {
    LOG(INFO) << "AsyncTaskRecord : launchPeople : " << launchPeople;
    if (launchPeople == "App" || launchPeople == "Pad") {
        return true;
    }
    return false;
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
