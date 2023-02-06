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

const int MAX_FIRST_RETRY_COUNT = 1;
const int MAX_BASE_POINT_RETRY_COUNT = 5;
const int MAX_RECHARGE_RETRY_COUNT = 5;

const int FLOW_SEIZE_SEAT = -10;
const int FLOW_OPEN_MECHANISM = -11;
const int FLOW_CLOSE_MECHANISM = -12;
const int FLOW_OUT_STATION = -13;
const int FLOW_END_SLEEP = -14;
const int FLOW_IN_BASE_POINT = -15;
const int FLOW_IN_STATION = -16;
const int FLOW_INTERRUPT = -17;


/**
 * 任务执行线程
 * 状态机相关信息从此处提取
 */
class AsyncTaskCall : public AsyncTaskRecord {
private:

    event::flow event_flow = event::flow::waiting_for_task;

    event::status event_status = event::status::AUTO_STATE;


protected:



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
    void handleAutoPoint(const RealPoint &point);

    void handleManualPoint(const RealPoint &point);

    void handleForcePoint(const RealPoint &point);

    RealPoint findFrontPoint();

    RealPoint findFrontNextPoint();

    void goodGame();

    void reset();

    virtual void handleFlowPoint(const RealPoint &point) = 0;

    void handlePlannerPoint(const RealPoint &point);

    virtual void processControl(const RealPoint &point) = 0;

    bool isBasePointReached(float disAccuracy, float angleAccuracy);


    void callGoFirstPoint();

    void callRetryFirstPoint(const std::function<void()> &f);

    void callGoNextPoint(const RealPoint &nextPoint);

    void callPointComplete(const std::function<void()> &f);


    void callTaskInterrupt(const RealPoint &point);

    void callManualCleanStart();

    void callManualCleanEnd();

    void callPause();

    void callResume();

    void callStopUrgentInBase();

    void peculiarTriggerBack(const std::function<void()> &f);

    void peculiarDisposeBasePoint(const RealPoint &point, event::flow needFlow, bool whetherDisposeMechanism);

    void peculiarDisposeMechanism(const RealPoint &point, event::flow needFlow);

    void peculiarDisposeStation(const RealPoint &point, event::flow arriveFlow,
                                event::flow retryFlow);

    void handleManualOperation() override;

    void handleSpecialOperation() override;

    void handleErrorOperation() override;

    void handleStop() override;

    void handleTask(const RealTask &task) override;

    void handlePoint(const RealPoint &point) override;

public:
    AsyncTaskCall();

    void executeUnrecoverableError();

    void executeUrgencyStop(bool isUrgencyStop);

    void executeOneTask(const RealTask &task);

    void executeOnNext(event::error error);

    void executeOutStation(bool result);

    void executeInStation(bool result);

    void exchangeFrontPoint(const RealPoint &point);

    void manualBackToBase(bool force);

    void manualResume();

    void manualPause();

    void enterManual();

    void quitManual();

    void urgencyStopAndCharge();

    std::string runTaskId();

    std::vector<RealTask> runTaskList();

    std::vector<RealPoint> runTaskPoint();

    void forceBackToBase(loop::special_epoll operation);

    std::tuple<int, std::string, std::string> generateErrorByRealPoint(const RealPoint &real_point);

    void recordMotorError();

    void recordLaserError(std::string error_event);

};

#endif //APP_COMMUNICATION_ASYNC_TASK_CALL_H
