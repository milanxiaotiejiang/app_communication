//
// Created by Looper on 2023/2/6.
//

#ifndef APP_COMMUNICATION_ASYNCTASKRECORD_H
#define APP_COMMUNICATION_ASYNCTASKRECORD_H

#include "AsyncTaskFramework.h"
#include "task/model/TaskStack.h"

const int FLOW_SEIZE_SEAT = -10;
const int FLOW_OPEN_MECHANISM = -11;
const int FLOW_CLOSE_MECHANISM = -12;
const int FLOW_OUT_STATION = -13;
const int FLOW_END_SLEEP = -14;
const int FLOW_IN_BASE_POINT = -15;
const int FLOW_IN_STATION = -16;

const int MAX_RECORD_TASK_STACK_SIZE = 3;

class AsyncTaskRecord : public AsyncTaskFramework {
protected:

    std::deque<TaskStack> stopStack;

    std::deque<RealTask> waitTaskQueue;
    std::deque<RealPoint> plannerQueue;

    RealTask runTask;

    RealPoint flowSeizeSeatPoint;
    RealPoint flowOpenMechanismPoint;
    RealPoint flowCloseMechanismPoint;
    RealPoint flowOutStationPoint;

    RealPoint flowEndSleepPoint;
    RealPoint flowInBasePoint;
    RealPoint flowInStationPoint;

    bool isWaitTask(event::flow flow);

    bool isPause();

    bool isPreparation(event::flow flow);

    bool isFlowingWater(event::flow flow);

    bool isReturningBase(event::flow flow);

    bool isContinueWork(event::flow flow, bool suspend);

    bool isRegularTask(event::flow flow);

    bool isManualTask(const std::string &launchPeople);

    bool isRechargeFLow(event::flow flow);

    bool isPlannerEmpty(event::flow flow);

    void recordEmergencyStop(event::flow event_flow, const RealPoint &realPoint);

    bool recoverableEmergencyStop();

    bool recoverableSuspend();

    TaskStack lastEmergencyStop();

    void makeSurePause(event::flow flow);

    void release() override;

};


#endif //APP_COMMUNICATION_ASYNCTASKRECORD_H
