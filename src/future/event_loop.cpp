//
// Created by Looper on 2022/11/14.
//

#include <thread>
#include <utility>
#include "future/event_loop.h"
#include "future/async_defer.h"
#include "future/timer_call.h"

namespace async {

    static thread_local EventLoop *g_thisLoop = nullptr;

    thread_local unsigned int EventLoop::s_id = 0;

    std::atomic<int> EventLoop::s_evId{0};

    bool EventLoop::_loop(DurationMs timeout) {
//        ASYNC_DEFER {
        timers_.update();

        if (fctrMutex_.try_lock()) {
            decltype(functors_) funcs;
            funcs.swap(functors_);
            fctrMutex_.unlock();

            for (const auto &f: funcs)
                f();
        }
//        };

        std::this_thread::sleep_for(timeout);
        return true;
    }

    EventLoop::EventLoop() {
        assert (!g_thisLoop && "There must be only one EventLoop per thread");
        g_thisLoop = this;

        id_ = s_evId++;
    }

    EventLoop::~EventLoop() = default;

    void EventLoop::scheduleLater(std::chrono::milliseconds duration, std::function<void()> f) {
        if (inThisLoop()) {
            scheduleAfterWithRepeat<1>(duration, std::move(f));
        } else {
            execute([=]() {
                scheduleAfterWithRepeat<1>(duration, f);
            });
        }
    }

    void EventLoop::schedule(std::function<void()> f) {
        execute(std::move(f));
    }

    bool EventLoop::cancel(TimerId id) {
        return timers_.cancel(std::move(id));
    }

    void EventLoop::cancelAny() {
        timers_.cancelAny();
    }

    void EventLoop::run() {
        assert (this->inThisLoop());

        const DurationMs kDefaultPollTime(100);
        const DurationMs kMinPollTime(1);

        while (!TimerCall::instance().isExit()) {
            auto timeout = std::min(kDefaultPollTime, timers_.nearestTimer());
            timeout = std::max(kMinPollTime, timeout);

            _loop(timeout);
        }
    }

    bool EventLoop::inThisLoop() const {
        return this == g_thisLoop;
    }

    void EventLoop::reset() {
        {
            std::unique_lock<std::mutex> guard(fctrMutex_);
            functors_.clear();
        }
    }
}
