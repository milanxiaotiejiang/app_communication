//
// Created by Looper on 2022/11/5.
//

#ifndef APP_COMMUNICATION_ASYNC_TASK_CALL_H
#define APP_COMMUNICATION_ASYNC_TASK_CALL_H

#include <utility>

#include "task/framework/AsyncTaskRecord.h"

#include "clean_history/CleanHistoryCenter.h"
#include "geometry_msgs/PoseStamped.h"
#include "task/status/state_machine.h"
#include "task/subscribe/async_machine.h"

const int MAX_FIRST_RETRY_COUNT = 2;
const int MAX_BASE_POINT_RETRY_COUNT = 3;
const int MAX_RECHARGE_RETRY_COUNT = 5;

const int FLOW_SEIZE_SEAT = -10;
const int FLOW_OPEN_MECHANISM = -11;
const int FLOW_CLOSE_MECHANISM = -12;
const int FLOW_OUT_STATION = -13;
const int FLOW_END_SLEEP = -14;
const int FLOW_IN_BASE_POINT = -15;
const int FLOW_IN_STATION = -16;


/**
 * 任务执行线程
 * 状态机相关信息从此处提取
 */
class AsyncTaskCall : public AsyncTaskRecord {
private:

    event::flow event_flow = event::flow::waiting_for_task;

protected:

    atomic<int> firstRetryCount;
    atomic<int> backBaseRetryCount;
    atomic<int> rechargeRetryCount;

    RealPoint flowSeizeSeatPoint;
    RealPoint flowOpenMechanismPoint;
    RealPoint flowCloseMechanismPoint;
    RealPoint flowOutStationPoint;

    RealPoint flowEndSleepPoint;
    RealPoint flowInBasePoint;
    RealPoint flowInStationPoint;
    RealPoint flowInterruptPoint;

    void setFlow(event::flow flow) {
        event_flow = flow;
        AsyncMachine::instance().setFlow(flow);
        clean_history_db::CleanHistoryCenter::instance().setCurrentFlow(flow);
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


    void handleAutoPoint(const RealPoint &point);

    void handlePointManualControl(const RealPoint &point);

    void handlePointSpecialDevice(const RealPoint &point);

    void goodGame();

    void garbage();

    void reset();

    virtual void handleFlowPoint(const RealPoint &point) = 0;

    virtual void processControl(const RealPoint &point) = 0;

    void handlePlannerPoint(const RealPoint &point);

    RealPoint findFrontPoint();

    RealPoint findFrontNextPoint();

    bool isBasePointReached(float disAccuracy, float angleAccuracy);


    void callGoNextPoint(const RealPoint &nextPoint);

    void callPointComplete(const std::function<void()> &f);

    void callManualCleanStart();

    void callManualCleanEnd();

    void callResume();

    void callPause();

    void cancelTaskAndBack();

    void cancelTask(const std::function<void()> &f);

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


    std::tuple<int, std::string, std::string> generateErrorByRealPoint(const RealPoint &real_point);

    void recordMotorError();

    void recordLaserError(std::string error_event);

};

#endif //APP_COMMUNICATION_ASYNC_TASK_CALL_H
