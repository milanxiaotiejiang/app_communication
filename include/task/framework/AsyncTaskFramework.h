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
#include "task/feedback.h"

const int OPENING_TIME_OF_CLEANING_MECHANISM = 10;
const int CLOSING_TIME_OF_CLEANING_MECHANISM = 5;
const int WAITING_TIME_OF_NODE_WORK_MODE = 80;

class AsyncTaskFramework : public AsyncCall {
private:
    std::condition_variable cv;
    std::mutex cv_mut;

    std::deque<loop::manual_epoll> manualEpollDeque;
    std::deque<loop::special_epoll> specialEpollDeque;
    std::deque<loop::error_epoll> errorEpollDeque;
    std::deque<loop::urgency_stop> urgencyStopDeque;
    std::deque<RealTask> taskEpollDeque;
    std::deque<RealBlock> blockEpollDeque;

    std::atomic<bool> sleepTimeout;

public:
    AsyncTaskFramework();

protected:

    std::atomic<loop::manual_epoll> epoll_manual;
    std::atomic<loop::special_epoll> epoll_special;
    std::atomic<loop::error_epoll> epoll_error;
    std::atomic<loop::urgency_stop> urgency_stop;

    void execute() override;

    virtual void handleManualOperation() = 0;

    virtual void handleSpecialOperation() = 0;

    virtual void handleErrorOperation() = 0;

    virtual void handleStop() = 0;

    virtual void handleTask(const RealTask &task) = 0;

    virtual void handleBlock(const RealBlock &block) = 0;

//    void notify_one(const std::function<void()> &triggerProcess);

    /**
     * https://zhuanlan.zhihu.com/p/384316039
     * 左值可以简单理解为可以放在等号左边或者可以取址的表达式的值类别，除了左值剩下的就是右值，一般多为临时变量。
     * 对于类型为T的左值引用使用T &表示，右值引用使用T &&表示。
     */
    template<typename F, typename... Args>
    void notify_one(F &&f, Args &&... args) {
        {
            std::unique_lock<std::mutex> lock(cv_mut);
            std::forward<F>(f)(std::forward<Args>(args)...);
        }
        cv.notify_one();
    }

    template<typename F, typename... Args>
    void lock(F &&f, Args &&... args) {
        {
            std::unique_lock<std::mutex> lock(cv_mut);
            std::forward<F>(f)(std::forward<Args>(args)...);
        }
    }

    void notify_one() {
        cv.notify_one();
    }

    loop::execute_handle function_manual_epoll();

    loop::execute_handle function_special_epoll();

    loop::execute_handle function_error_epoll();

    loop::execute_handle function_urgency_stop();

    void setEpollManual(loop::manual_epoll epoll_manual);

    void setEpollSpecial(loop::special_epoll epoll_special);

    void setEpollError(loop::error_epoll epoll_error);

    void setUrgencyStop(loop::urgency_stop urgency_stop);

    void callOutBaseStation();

    void callBackStation();

    void callCancelBackStation();

    void callSwitchWorkMode(std::function<void(bool work)> f);

    virtual void callOpenMechanism(const WorkStatus &status, bool knife, std::function<void()> f) = 0;

    virtual void callCloseMechanism(std::function<void()> f) = 0;

    virtual void callBackBasePoint();

    void callNeedPublishSleep();

    bool isWorkMode();

    bool isUrgencyStop();

    bool isManualMode();

    bool isUnrecoverableError();

    bool isManualControl();

    bool isExchangeTask();

    bool isSpecialDevice();

    bool isCharging();

    virtual void release() = 0;

    void pushManual(loop::manual_epoll data);

    void pushSpecial(loop::special_epoll data);

    void pushError(loop::error_epoll data);

    void pushUrgencyStop(loop::urgency_stop data);

    void pushTask(const RealTask &data);

    void pushBlock(const RealBlock &data);

};


#endif //APP_COMMUNICATION_ASYNCTASKFRAMEWORK_H
