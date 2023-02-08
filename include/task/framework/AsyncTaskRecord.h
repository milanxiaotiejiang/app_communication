//
// Created by Looper on 2023/2/6.
//

#ifndef APP_COMMUNICATION_ASYNCTASKRECORD_H
#define APP_COMMUNICATION_ASYNCTASKRECORD_H

#include "AsyncTaskFramework.h"
#include "task/model/TaskStack.h"

const int MAX_RECORD_TASK_STACK_SIZE = 3;

class AsyncTaskRecord : public AsyncTaskFramework {
protected:

    std::deque<TaskStack> stopStack;

    std::deque<RealTask> waitTaskQueue;
    std::deque<RealPoint> plannerQueue;

    RealTask runTask;

    bool isPause();

    bool isPreparation(event::flow flow);

    bool isFlowingWater(event::flow flow);

    bool isReturningBase(event::flow flow);

    bool isContinueWork(event::flow flow, bool suspend);

    void recordEmergencyStop(event::flow event_flow, const RealPoint &realPoint);

    bool recoverableEmergencyStop();

    bool recoverableSuspend();

    TaskStack lastEmergencyStop();

    TaskStack lastSuspend();

    void release() override;
};


#endif //APP_COMMUNICATION_ASYNCTASKRECORD_H
