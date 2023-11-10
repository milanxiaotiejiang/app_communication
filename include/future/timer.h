//
// Created by Looper on 2022/11/14.
//

#ifndef APP_COMMUNICATION_TIMER_H
#define APP_COMMUNICATION_TIMER_H

#include <chrono>
#include <memory>
#include <functional>
#include <mutex>
#include "map"
#include "ostream"

namespace async {
    using DurationMs = std::chrono::milliseconds;
    using TimePoint = std::chrono::steady_clock::time_point;
    using TimerId = std::shared_ptr<std::pair<TimePoint, unsigned int> >;

    constexpr int kForever = -1;

    inline std::ostream &operator<<(std::ostream &os, const TimerId &d) {
        os << "[TimerId:" << (void *) d.get() << "]";
        return os;
    }

    namespace internal {
        class TimerManager final {
        private:
            class Timer {
                friend class TimerManager;

            private:
                void _move(Timer &&timer);

                TimerId id_;

                std::function<void()> func_;
                DurationMs interval_;
                int count_;

            public:
                explicit Timer(const TimePoint &timer);

                // only move
                Timer(Timer &&timer);

                Timer &operator=(Timer &&timer);

                Timer(const Timer &) = delete;

                void operator=(const Timer &) = delete;

                template<typename F, typename... Args>
                void setCallback(F &&f, Args &&... args);

                void onTimer();

                TimerId id() const;

                unsigned int uniqueId() const;
            };

            std::multimap<TimePoint, Timer> timers_;

            friend class Timer;

            static unsigned int s_timerIdGen_;

            std::mutex cv_mut;

        public:
            TimerManager() = default;

            ~TimerManager() = default;

            TimerManager(const TimerManager &) = delete;

            void operator=(const TimerManager &) = delete;

            void update();

            bool cancel(TimerId id);

            void cancelAny();

            DurationMs nearestTimer();

            /**
             * 在指定的时间点，执行任务一次
             */
            template<typename F, typename... Args>
            TimerId scheduleAt(const TimePoint &triggerTime, F &&f, Args &&... args);

            /**
             * 在经过指定的延迟时间后，执行任务一次
             */
            template<typename Duration, typename F, typename... Args>
            TimerId scheduleAfter(const Duration &duration, F &&f, Args &&... args);

            /**
             * 在指定的时间点，执行任务，之后以固定的周期重复执行指定的任务。
             * @tparam RepeatCount
             */
            template<int RepeatCount, typename Duration, typename F, typename... Args>
            TimerId scheduleAtWithRepeat(const TimePoint &triggerTime, const Duration &period, F &&f, Args &&... args);

            /**
             * 在经过指定的延迟时间后，执行任务，之后以固定的周期重复执行指定的任务。
             * @tparam RepeatCount 可以指定重复次数，在task执行指定次数后停止，也可以kForever不停止
             */
            template<int RepeatCount, typename Duration, typename F, typename... Args>
            TimerId scheduleAfterWithRepeat(const Duration &period, F &&f, Args &&... args);

        };

        template<typename F, typename... Args>
        TimerId TimerManager::scheduleAt(const TimePoint &triggerTime, F &&f, Args &&... args) {
            return scheduleAtWithRepeat<1>(triggerTime, DurationMs(0), std::forward<F>(f), std::forward<Args>(args)...);
        }

        template<typename Duration, typename F, typename... Args>
        TimerId TimerManager::scheduleAfter(const Duration &duration, F &&f, Args &&... args) {
            return scheduleAt(std::chrono::steady_clock::now() + duration,
                              std::forward<F>(f), std::forward<Args>(args)...);
        }

        template<int RepeatCount, typename Duration, typename F, typename... Args>
        TimerId TimerManager::scheduleAtWithRepeat(const TimePoint &triggerTime, const Duration &period, F &&f,
                                                   Args &&... args) {
            {
                std::unique_lock<std::mutex> lock(cv_mut);

                static_assert(RepeatCount != 0, "Why you add a timer with zero count?");

                using namespace std::chrono;

                Timer t(triggerTime);

                t.interval_ = std::max(DurationMs(1), duration_cast<DurationMs>(period));
                t.count_ = RepeatCount;
                TimerId id = t.id();

                t.template setCallback(std::forward<F>(f), std::forward<Args>(args)...);
                timers_.insert(std::make_pair(triggerTime, std::move(t)));
                return id;
            }

        }

        template<int RepeatCount, typename Duration, typename F, typename... Args>
        TimerId TimerManager::scheduleAfterWithRepeat(const Duration &period, F &&f, Args &&... args) {
            return scheduleAtWithRepeat<RepeatCount>(std::chrono::steady_clock::now() + period,
                                                     period, std::forward<F>(f), std::forward<Args>(args)...);
        }


        template<typename F, typename... Args>
        void TimerManager::Timer::setCallback(F &&f, Args &&... args) {
            func_ = std::bind(std::forward<F>(f), std::forward<Args>(args)...);
        }
    }
}


#endif //APP_COMMUNICATION_TIMER_H
