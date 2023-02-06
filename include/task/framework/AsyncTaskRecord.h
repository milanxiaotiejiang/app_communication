//
// Created by Looper on 2023/2/6.
//

#ifndef APP_COMMUNICATION_ASYNCTASKRECORD_H
#define APP_COMMUNICATION_ASYNCTASKRECORD_H

#include "AsyncTaskFramework.h"
#include "task/model/TaskStack.h"

const int MAX_RECORD_TASK_STACK_SIZE = 3;

class AsyncTaskRecord : public AsyncTaskFramework {
private:

    atomic<bool> pauseInterception;

protected:
    std::deque<TaskStack> stopStack;
    std::deque<TaskStack> suspendStack;

    std::deque<RealTask> waitTaskQueue;
    std::deque<RealPoint> plannerQueue;

    atomic<int> firstRetryCount;
    atomic<int> backBaseRetryCount;
    atomic<int> rechargeRetryCount;

    RealTask runTask;

    bool isPause(const TaskStack &stack);

    bool isFlowingWater(const TaskStack &stack);

    bool isReturningBase(const TaskStack &stack);

    bool isContinueWork(const TaskStack &stack);

    void recordEmergencyStop(event::flow event_flow, const RealPoint &realPoint);

    void recordSuspend(event::flow event_flow, const RealPoint &realPoint);

    bool recoverableEmergencyStop();

    bool recoverableSuspend();

    TaskStack lastEmergencyStop();

    TaskStack lastSuspend();

    void release() override;
};


#endif //APP_COMMUNICATION_ASYNCTASKRECORD_H
