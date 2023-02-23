//
// Created by Looper on 2022/11/5.
//

#ifndef APP_COMMUNICATION_ASYNC_TASK_CALL_H
#define APP_COMMUNICATION_ASYNC_TASK_CALL_H

#include <utility>

#include "task/framework/AsyncTaskRecord.h"

#include "geometry_msgs/PoseStamped.h"
#include "task/status/state_machine.h"
#include "task/subscribe/async_machine.h"

const int MAX_FIRST_RETRY_COUNT = 2;
const int MAX_BASE_POINT_RETRY_COUNT = 3;
const int MAX_RECHARGE_RETRY_COUNT = 5;

/**
 * 任务执行线程
 * 状态机相关信息从此处提取
 */
class AsyncTaskCall : public AsyncTaskRecord {
private:

    std::atomic<event::flow> event_flow;;

protected:

    atomic<int> firstRetryCount;
    atomic<int> backBaseRetryCount;
    atomic<int> rechargeRetryCount;

    void setFlow(event::flow flow) {
        event_flow = flow;
        AsyncMachine::instance().setFlow(flow);
    }

    event::flow currentFlow() {
        return event_flow;
    }

protected:

    void handleManualOperation() override;

    void handleSpecialOperation() override;

    void handleErrorOperation() override;

    void handleStop() override;

    void handleTask(const RealTask &task) override;

    void handlePoint(const RealPoint &point) override;


    virtual void handleExecuteTask(const RealTask &task);

    void handleAutoPoint(const RealPoint &point);

    void handlePointManualControl(const RealPoint &point);

    void handlePointSpecialDevice(const RealPoint &point);

    virtual void goodGame();

    virtual void garbage();

    void reset();

    virtual void handleFlowPoint(const RealPoint &point) = 0;

    virtual void processControl(const RealPoint &point) = 0;

    virtual void handlePlannerPoint(const RealPoint &point);

    RealPoint findFrontPoint();

    RealPoint findFrontNextPoint();

    bool isBasePointReached(float disAccuracy, float angleAccuracy);


    void callGoNextPoint(const RealPoint &nextPoint);

    void callPointComplete(const std::function<void()> &f);

    void callManualCleanStart();

    void callManualCleanEnd();

    void callSubsequentSelfClean(WorkStatus status);

    void callSelfCleanClose();

    void callUrgencyStop();

    void callReleaseStop();

    void callRecoveryStop();

    void callResume();

    void callPause();

    void cancelTaskAndBack();

    void cancelTask();

    void triggerSuspend();

public:
    AsyncTaskCall();

    void executeUnrecoverableError();

    void executeOneTask(const RealTask &task);

    void executeOnNext(event::error error);

    void executeOutStation(bool result);

    void executeInStation(bool result);


    void manualBackToBase(bool force);

    void manualResume();

    void manualPause();


    void enterManual();

    void quitManual();


    void executeUrgencyStop(bool isUrgencyStop);

    void urgencyStopAndCharge();


    void forceBackToBase(loop::special_epoll operation);


    std::string runTaskId();

    std::vector<RealTask> runTaskList();

    std::vector<RealPoint> runTaskPoint();

};

#endif //APP_COMMUNICATION_ASYNC_TASK_CALL_H
