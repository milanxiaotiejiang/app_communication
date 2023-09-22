//
// Created by io on 23-9-21.
//

#include "task/framework/AsyncGateFramework.h"
#include "future/async_call.h"
#include "task/point_planner.h"

AsyncGateImplement::AsyncGateImplement() {
    int err = AsyncGateImplement::make_thread(run, this);
    LOG_IF(INFO, DEBUG_GATE) << "AsyncGateImplement : " << err;
}

[[noreturn]] void AsyncGateImplement::execute() {
    for (;;) {
        std::unique_lock<std::mutex> lock(cv_mut);

        cv.wait(lock, [this]() {
            return next;
        });
        next = false;

        LOG_IF(INFO, DEBUG_GATE)
                        << "AsyncGateImplement  totalPointCount : " << totalPointCount << " , progressPointCount : "
                        << progressPointCount;
        if (pointEpollDeque.empty()) {
            if (progressPointCount == totalPointCount) {
                implementing = false;
                //执行完成
                LOG_IF(INFO, DEBUG_GATE) << "执行完成";
                callbackImplementEnd(true);
            } else {
                LOG_IF(INFO, DEBUG_GATE) << "执行错误";
                callbackImplementEnd(false);
            }
        } else {
            if (progressPointCount == totalPointCount) {
                callbackImplementStart();
            }
            RealPoint &front = pointEpollDeque.front();
            if (front.open_gate) {
                LOG_IF(INFO, DEBUG_GATE)
                                << "AsyncGateImplement  发送打开闸机的命令 ... ";
            }
            PointPlanner::instance().goToPoint(front);
        }

    }
}

void AsyncGateImplement::onImplementStart(const std::vector<RealPoint> points) {
    {
        std::unique_lock<std::mutex> lock(cv_mut);
        implementing = true;

        pointEpollDeque.clear();

        progressPointCount = 0;
        for (const auto &point: points) {
            pointEpollDeque.push_back(point);
        }
        totalPointCount = pointEpollDeque.size();

        next = true;
        cv.notify_one();
    }
}

bool AsyncGateImplement::isImplement() {
    return implementing;
}

void AsyncGateImplement::executeOnPointDone(event::error error) {
    if (!implementing) {
        return;
    }
    {
        std::unique_lock<std::mutex> lock(cv_mut);

        if (error == event::SUCCEEDED) {
            if (!pointEpollDeque.empty()) {
                pointEpollDeque.pop_front();
                progressPointCount++;
            }
        }

        next = true;
        cv.notify_one();
    }
}

void AsyncGateImplement::executeOnPathDone(event::error error) {
    if (!implementing) {
        return;
    }
    {
        std::unique_lock<std::mutex> lock(cv_mut);

        if (error == event::SUCCEEDED) {
            if (!pointEpollDeque.empty()) {
                pointEpollDeque.pop_front();
                progressPointCount++;
            }
        }

        next = true;
        cv.notify_one();
    }
}

void AsyncGateImplement::setCallbackImplementStart(const std::function<void()> &callback) {
    AsyncGateImplement::callbackImplementStart = callback;
}

void AsyncGateImplement::setCallbackImplementEnd(const std::function<void(bool)> &callback) {
    AsyncGateImplement::callbackImplementEnd = callback;
}

//*******************************************************************************************************

AsyncGateDistribution::AsyncGateDistribution() : mImplement(std::make_shared<AsyncGateImplement>()) {
    int err = AsyncGateDistribution::make_thread(run, this);
    LOG_IF(INFO, DEBUG_GATE) << "AsyncGateDistribution : " << err;

    mImplement->setCallbackImplementStart([]() {
        LOG_IF(INFO, DEBUG_GATE) << "闸机规划点位开始执行 ... ";
    });
    mImplement->setCallbackImplementEnd([this](bool b) {
        LOG_IF(INFO, DEBUG_GATE) << "通过闸机结果 " << b;
        callbackDistribution(b);
    });
}

[[noreturn]] void AsyncGateDistribution::execute() {
    for (;;) {
        std::unique_lock<std::mutex> lock(cv_mut);
        cv.wait(lock, [this] {
            return !pointList.empty();
        });

        mImplement->onImplementStart(pointList);
        pointList.clear();
    }
}

void AsyncGateDistribution::onDistributionStart(const std::vector<RealPoint> points) {
    {
        std::unique_lock<std::mutex> lock(cv_mut);

        pointList.clear();
        for (const auto &point: points) {
            pointList.push_back(point);
        }

        cv.notify_one();
    }
}

bool AsyncGateDistribution::isImplement() {
    return mImplement->isImplement();
}

void AsyncGateDistribution::executeOnPointDone(event::error error) {
    mImplement->executeOnPointDone(error);
}

void AsyncGateDistribution::executeOnPathDone(event::error error) {
    mImplement->executeOnPathDone(error);
}

void AsyncGateDistribution::setCallbackDistribution(const std::function<void(bool)> &callback) {
    AsyncGateDistribution::callbackDistribution = callback;
}
