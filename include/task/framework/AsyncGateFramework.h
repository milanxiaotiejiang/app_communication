//
// Created by io on 23-9-21.
//

#ifndef APP_COMMUNICATION_ASYNCGATEFRAMEWORK_H
#define APP_COMMUNICATION_ASYNCGATEFRAMEWORK_H

#include "future/async_call.h"

#include "condition_variable"
#include "mutex"
#include "atomic"
#include "queue"
#include "functional"
#include "task/RealPoint.h"
#include "task/status/state_machine.h"

class AsyncGateImplement : public AsyncCall {
private:
    std::condition_variable cv;
    std::mutex cv_mut;

    bool next{false};

    std::atomic<bool> implementing;

    std::deque<RealPoint> pointEpollDeque;

    int totalPointCount{0};
    std::atomic<int> progressPointCount{0};

    std::function<void()> callbackImplementStart;

    std::function<void(bool)> callbackImplementEnd;

protected:
    [[noreturn]] void execute() override;

public:
    AsyncGateImplement();

    void onImplementStart(const std::vector<RealPoint> points);

    bool isImplement();

    void executeOnPointDone(event::error error);

    void executeOnPathDone(event::error error);

    void setCallbackImplementStart(const std::function<void()> &callback);

    void setCallbackImplementEnd(const std::function<void(bool)> &callback);

};

class AsyncGateDistribution : public AsyncCall {
private:
    std::shared_ptr<AsyncGateImplement> mImplement;

    std::condition_variable cv;
    std::mutex cv_mut;

    std::atomic<bool> open;

    std::vector<RealPoint> pointList;

    std::function<void(bool)> callbackDistribution;

protected:
    [[noreturn]]  void execute() override;

public:
    AsyncGateDistribution();

    void onDistributionStart(const std::vector<RealPoint> points);

    bool isImplement();

    void executeOnPointDone(event::error error);

    void executeOnPathDone(event::error error);

    void setCallbackDistribution(const std::function<void(bool)> &callback);

};

#endif //APP_COMMUNICATION_ASYNCGATEFRAMEWORK_H
