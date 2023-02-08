//
// Created by Looper on 2023/2/6.
//

#ifndef APP_COMMUNICATION_ASYNCTASKFRAMEWORK_H
#define APP_COMMUNICATION_ASYNCTASKFRAMEWORK_H

#include "future/async_call.h"

#include "future/async_call.h"
#include "task/status/state_machine.h"

#include "condition_variable"
#include "mutex"
#include "atomic"

#include "map"
#include "queue"

#include "task/RealPoint.h"
#include "task/RealTask.h"

const int OPENING_TIME_OF_CLEANING_MECHANISM = 10;
const int CLOSING_TIME_OF_CLEANING_MECHANISM = 5;
const int WAITING_TIME_OF_NODE_WORK_MODE = 30;

class AsyncTaskFramework : public AsyncCall {
private:
    std::condition_variable cv;
    std::mutex cv_mut;

    std::deque<loop::manual_epoll> manualEpollDeque;
    std::deque<loop::special_epoll> specialEpollDeque;
    std::deque<loop::error_epoll> errorEpollDeque;
    std::deque<loop::urgency_stop> urgencyStopDeque;
    std::deque<RealTask> taskEpollDeque;
    std::deque<RealPoint> pointEpollDeque;

    atomic<bool> sleepTimeout;

protected:

    loop::manual_epoll epoll_manual = loop::manual_epoll::manual_normal;
    loop::special_epoll epoll_special = loop::special_epoll::special_normal;
    loop::error_epoll epoll_error = loop::error_epoll::error_normal;
    loop::urgency_stop urgency_stop = loop::urgency_stop::trigger_urgency_stop;

public:
    AsyncTaskFramework();

protected:
    void execute() override;

    virtual void handleManualOperation() = 0;

    virtual void handleSpecialOperation() = 0;

    virtual void handleErrorOperation() = 0;

    virtual void handleStop() = 0;

    virtual void handleTask(const RealTask &task) = 0;

    virtual void handlePoint(const RealPoint &point) = 0;

    void notify_one(const std::function<void()> &triggerProcess);

    loop::execute_handle function_manual_epoll();

    loop::execute_handle function_special_epoll();

    loop::execute_handle function_error_epoll();

    loop::execute_handle function_urgency_stop();

    void callOutBaseStation();

    void callBackStation();

    void callSwitchWorkMode(function<void(bool work)> workingMode);

    void callOpenMechanism(const WorkStatus &status, function<void()> openMechanism);

    void callCloseMechanism(function<void()> resetMechanism);

    void callBackBasePoint();

    bool isWorkMode();

    bool isUrgencyStop();

    bool isManualMode();

    bool isUnrecoverableError();

    bool isManualControl();

    bool isSpecialDevice();

    virtual void release() = 0;

    void pushManual(loop::manual_epoll data);

    void pushSpecial(loop::special_epoll data);

    void pushError(loop::error_epoll data);

    void pushUrgencyStop(loop::urgency_stop data);

    void pushTask(const RealTask &data);

    void pushPoint(const RealPoint &data);

};


#endif //APP_COMMUNICATION_ASYNCTASKFRAMEWORK_H
