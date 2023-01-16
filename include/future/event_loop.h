//
// Created by Looper on 2022/11/14.
//

#ifndef APP_COMMUNICATION_EVENT_LOOP_H
#define APP_COMMUNICATION_EVENT_LOOP_H

#include "future/scheduler.h"
#include "future/timer.h"
#include "future/future.h"
#include <mutex>
#include <cassert>
#include "vector"

namespace async {

    class EventLoop : public Scheduler {
    private:
        bool _loop(DurationMs timeout);

        internal::TimerManager timers_;

        std::mutex fctrMutex_;
        std::vector<std::function<void()> > functors_;

        int id_;
        static std::atomic<int> s_evId;

        static thread_local unsigned int s_id;

    public:
        EventLoop();

        ~EventLoop();

        EventLoop(const EventLoop &) = delete;

        void operator=(const EventLoop &) = delete;

        EventLoop(EventLoop &&) = delete;

        void operator=(EventLoop &&) = delete;

        void scheduleLater(std::chrono::milliseconds duration, std::function<void()> f) override;

        void schedule(std::function<void()> f) override;

        bool cancel(TimerId id);

        void cancelAny();

        template<typename F, typename... Args>
        TimerId scheduleAt(const TimePoint &triggerTime, F &&f, Args &&...args);

        template<typename Duration, typename F, typename... Args>
        TimerId scheduleAfter(const Duration &duration, F &&f, Args &&...args);

        template<int RepeatCount, typename Duration, typename F, typename... Args>
        TimerId scheduleAtWithRepeat(const TimePoint &triggerTime, const Duration &period, F &&f, Args &&...args);

        template<int RepeatCount, typename Duration, typename F, typename... Args>
        TimerId scheduleAfterWithRepeat(const Duration &period, F &&f, Args &&...args);

        template<typename F, typename... Args,
                typename = typename std::enable_if<!std::is_void<typename std::result_of<F(
                        Args...)>::type>::value, void>::type,
                typename Dummy = void>
        auto execute(F &&f, Args &&...args) -> Future<typename std::result_of<F(Args...)>::type>;

        template<typename F, typename... Args,
                typename = typename std::enable_if<std::is_void<typename std::result_of<F(
                        Args...)>::type>::value, void>::type>
        auto execute(F &&f, Args &&...args) -> Future<void>;

        void run();

        bool inThisLoop() const;

        void reset();
    };

    template<typename F, typename... Args>
    TimerId EventLoop::scheduleAt(const TimePoint &triggerTime, F &&f, Args &&... args) {
        assert (inThisLoop());
        return timers_.template scheduleAt(triggerTime, std::forward<F>(f), std::forward<Args>(args)...);
    }

    template<typename Duration, typename F, typename... Args>
    TimerId EventLoop::scheduleAfter(const Duration &duration, F &&f, Args &&... args) {
        assert (inThisLoop());
        return timers_.template scheduleAfter(duration, std::forward<F>(f), std::forward<Args>(args)...);
    }

    template<int RepeatCount, typename Duration, typename F, typename... Args>
    TimerId
    EventLoop::scheduleAtWithRepeat(const TimePoint &triggerTime, const Duration &period, F &&f, Args &&... args) {
        assert (inThisLoop());
        return timers_.template
                scheduleAtWithRepeat<RepeatCount>(triggerTime, period, std::forward<F>(f), std::forward<Args>(args)...);
    }

    template<int RepeatCount, typename Duration, typename F, typename... Args>
    TimerId EventLoop::scheduleAfterWithRepeat(const Duration &period, F &&f, Args &&... args) {
        assert (inThisLoop());
        return timers_.template
                scheduleAfterWithRepeat<RepeatCount>(period, std::forward<F>(f), std::forward<Args>(args)...);
    }


    template<typename F, typename... Args, typename, typename Dummy>
    Future<typename std::result_of<F(Args...)>::type> EventLoop::execute(F &&f, Args &&... args) {
        using resultType = typename std::result_of<F(Args...)>::type;

        Promise<resultType> promise;
        auto future = promise.GetFuture();

        if (inThisLoop()) {
            promise.template setValue(std::forward<F>(f)(std::forward<Args>(args)...));
        } else {
            auto task = std::bind(std::forward<F>(f), std::forward<Args>(args)...);
            auto func = [t = std::move(task), pm = std::move(promise)]() mutable {

                try {
                    pm.template setValue(Try<resultType>(t()));
                } catch (...) {
                    pm.template setException(std::current_exception());
                }

            };

            {
                std::unique_lock<std::mutex> guard(fctrMutex_);
                functors_.emplace_back(std::move(func));
            }

        }

        return future;
    }

    template<typename F, typename... Args, typename>
    Future<void> EventLoop::execute(F &&f, Args &&... args) {
        using resultType = typename std::result_of<F(Args...)>::type;
        static_assert(std::is_void<resultType>::value, "must be void");

        Promise<void> promise;
        auto future = promise.getFuture();

        if (inThisLoop()) {
            std::forward<F>(f)(std::forward<Args>(args)...);
            promise.template setValue();
        } else {
            auto task = std::bind(std::forward<F>(f), std::forward<Args>(args)...);
            auto func = [t = std::move(task), pm = std::move(promise)]() mutable {
                try {
                    t();
                    pm.template setValue();
                } catch (...) {
                    pm.setException(std::current_exception());
                }
            };

            {
                std::unique_lock<std::mutex> guard(fctrMutex_);
                functors_.emplace_back(std::move(func));
            }

        }

        return future;
    }
}

#endif //APP_COMMUNICATION_EVENT_LOOP_H
