//
// Created by Looper on 2022/11/5.
//

#ifndef APP_COMMUNICATION_ASYNC_TASK_CALL_H
#define APP_COMMUNICATION_ASYNC_TASK_CALL_H

#include <utility>

#include "future/async_call.h"

#include "condition_variable"
#include "mutex"
#include "atomic"

#include "map"
#include "queue"

#include "task/RealPoint.h"
#include "task/RealTask.h"

#include "clean_history/CleanHistoryCenter.h"
#include "geometry_msgs/PoseStamped.h"
#include "task/status/state_machine.h"
#include "task/subscribe/async_machine.h"

const int EXECUTE_HANDLE_UNRECOVERABLE_ERROR = -1;
const int EXECUTE_HANDLE_STOP = 0;
const int EXECUTE_HANDLE_TASK = 1;
const int EXECUTE_HANDLE_POINT = 2;

const int MAX_FIRST_RETRY_COUNT = 1;
const int MAX_BASE_POINT_RETRY_COUNT = 5;
const int MAX_RECHARGE_RETRY_COUNT = 5;

const int SPECIAL_MANUAL_RESUME = -1;
const int SPECIAL_MANUAL_PAUSE = -2;
const int SPECIAL_MANUAL_BACK = -3;

const int SPECIAL_MANUAL_CLEAN_START = -4;
const int SPECIAL_MANUAL_CLEAN_END = -5;

const int SPECIAL_FORCE_LOW_BATTERY = -6;
const int SPECIAL_MANUAL_CONTROL_FORCE_BACK = -7;
const int SPECIAL_EQUIPMENT_ERROR_BACK = -8;

const int FLOW_SEIZE_SEAT = -10;
const int FLOW_OPEN_MECHANISM = -11;
const int FLOW_CLOSE_MECHANISM = -12;
const int FLOW_OUT_STATION = -13;
const int FLOW_END_SLEEP = -14;
const int FLOW_IN_BASE_POINT = -15;
const int FLOW_IN_STATION = -16;
const int FLOW_INTERRUPT = -17;
const int SPECIAL_STOP_URGENT_IN_BASE = -18;

const int OPENING_TIME_OF_CLEANING_MECHANISM = 10;
const int CLOSING_TIME_OF_CLEANING_MECHANISM = 5;
const int WAITING_TIME_OF_NODE_WORK_MODE = 30;

/**
 * 任务执行线程
 * 状态机相关信息从此处提取
 */
class AsyncTaskCall : public AsyncCall {
private:
    std::deque<RealTask> taskDeque;

    std::deque<RealTask> waitTaskQueue;

    RealTask workTask;

//    timer::internal::TimerManager timers;

    event::flow event_flow = event::flow::waiting_for_task;

    atomic<bool> pauseInterception;
    atomic<bool> manualClean_;
    event::flow recordFlow;
    RealPoint recordPoint;

    event::status event_status = event::status::AUTO_STATE;

    RealPoint specialResumePoint;
    RealPoint specialPausePoint;
    RealPoint specialBackPoint;

    RealPoint specialManualCleanStartPoint;
    RealPoint specialManualCleanEndPoint;

    RealPoint specialLowBatteryPoint;
    RealPoint specialMCFBackPoint;

    RealPoint specialEquipmentErrorBackPoint;
    RealPoint specialStopUrgentInBasePoint;

    atomic<bool> triggerUrgencyStop;
    atomic<bool> urgencyStopState;

    atomic<bool> triggerUnrecoverableError;
    atomic<bool> unrecoverableErrorState;

    event::flow urgencyStopEventFlow;
    RealPoint urgencyStopPoint;
    event::status urgencyStopStatus;

protected:
    std::condition_variable cv;
    std::mutex cv_mut;

    std::deque<RealPoint> pointDeque;

    std::deque<RealPoint> plannerQueue;

    atomic<bool> sleepTimeout;
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

    void setPauseInterception(bool interception) {
        pauseInterception = interception;
    }

    bool currentInterception() {
        return pauseInterception;
    }

    void setManualClean(bool manual_clean) {
        manualClean_ = manual_clean;
    }

    bool manualClean() {
        return manualClean_;
    }

    event::flow currentRecordFlow() {
        return recordFlow;
    }

    RealPoint currentRecordPoint() {
        return recordPoint;
    }

    void recordAutoState(RealPoint realPoint) {
        recordFlow = event_flow;
        recordPoint = std::move(realPoint);
    }


    event::flow stopEventFlow() const {
        return urgencyStopEventFlow;
    }

    RealPoint stopPoint() const {
        return urgencyStopPoint;
    }

    event::status stopStatus() const {
        return urgencyStopStatus;
    }

    void recordLockState(RealPoint realPoint) { //记录下本地的状态，用于解除急停后再次循环
        urgencyStopEventFlow = event_flow;
        urgencyStopPoint = std::move(realPoint);
        urgencyStopStatus = event_status;
    }

protected:
    void execute() override;

    void handleStop();

    void handleTask(const RealTask &task);

    void handleAutoPoint(const RealPoint &point);

    void handleManualPoint(const RealPoint &point);

    void handleForcePoint(const RealPoint &point);

    RealPoint findFrontPoint();

    RealPoint findFrontNextPoint();

    void goodGame();

    void reset();

    void handleSpecialPoint(const RealPoint &point);

    virtual void handleFlowPoint(const RealPoint &point) = 0;

    void handlePlannerPoint(const RealPoint &point);

    virtual void processControl(const RealPoint &point) = 0;

    bool isBasePointReached(float disAccuracy, float angleAccuracy);

    void callOutBaseStation();

    void callSwitchWorkMode();

    void callOpenMechanism(const RealPoint &point);

    void callCloseMechanism();

    void callGoFirstPoint();

    void callRetryFirstPoint(const std::function<void()> &f);

    void callGoNextPoint(const RealPoint &nextPoint);

    void callPointComplete(const std::function<void()> &f);

    void callBackBasePoint();

    void callBackStation();

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

    void forceBackToBase(int force_type);

    std::tuple<int, std::string, std::string> generateErrorByRealPoint(const RealPoint &real_point);

    void recordMotorError();

    void recordLaserError(std::string error_event);
};

#endif //APP_COMMUNICATION_ASYNC_TASK_CALL_H
