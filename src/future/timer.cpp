//
// Created by Looper on 2022/11/14.
//

#include "future/timer.h"
#include "glog/logging.h"

namespace async {
    namespace internal {

        unsigned int TimerManager::s_timerIdGen_ = 0;

        void TimerManager::update() {
            {
                std::unique_lock<std::mutex> lock(cv_mut);

                const auto now = std::chrono::steady_clock::now();

                for (auto it(timers_.begin()); it != timers_.end();) {
                    if (it->first > now)
                        return;

                    it->second.onTimer();

                    Timer timer(std::move(it->second));
                    it = timers_.erase(it);

                    if (timer.count_ != 0) {
                        const auto tp = timer.id_->first;
                        auto itNew = timers_.insert(std::make_pair(tp, std::move(timer)));
                        if (it == timers_.end() || itNew->first < it->first)
                            it = itNew;
                    }
                }
            }
        }

        bool TimerManager::cancel(TimerId id) {
            {
                std::unique_lock<std::mutex> lock(cv_mut);

                if (timers_.empty())
                    return true;

                auto begin = timers_.lower_bound(id->first);
                if (begin == timers_.end())
                    return false;

                auto end = timers_.upper_bound(id->first);
                for (auto it(begin); it != end; ++it) {
                    if (it->second.uniqueId() == id->second) {
                        it->second.count_ = 0;
                        return true;
                    }
                }

                return false;
            }
        }

        void TimerManager::cancelAny() {
            {
                std::unique_lock<std::mutex> lock(cv_mut);

                if (timers_.empty())
                    return;

                timers_.clear();
            }

        }

        DurationMs TimerManager::nearestTimer() const {
            {
//                std::unique_lock<std::mutex> lock(cv_mut);

                if (timers_.empty())
                    return DurationMs::max();

                const auto &timer = timers_.begin()->second;
                auto now = std::chrono::steady_clock::now();
                if (now > timer.id()->first)
                    return DurationMs::min();
                else
                    return std::chrono::duration_cast<DurationMs>(timer.id()->first - now);
            }
        }


        void TimerManager::Timer::_move(TimerManager::Timer &&timer) {
            this->id_ = std::move(timer.id_);
            this->func_ = std::move(timer.func_);
            this->interval_ = std::move(timer.interval_);
            this->count_ = timer.count_;
        }

        TimerManager::Timer::Timer(const TimePoint &tp) :
                id_(
                        std::make_shared<std::pair<TimePoint, unsigned int>>(
                                tp, ++TimerManager::s_timerIdGen_)
                ),
                count_(kForever) {
        }

        TimerManager::Timer::Timer(TimerManager::Timer &&timer) {
            _move(std::move(timer));
        }

        TimerManager::Timer &TimerManager::Timer::operator=(TimerManager::Timer &&timer) {
            if (this != &timer)
                _move(std::move(timer));
            return *this;
        }

        void TimerManager::Timer::onTimer() {
            if (!func_ || count_ == 0)
                return;

            if (count_ == kForever || count_-- > 0) {
                func_();
                id_->first += interval_;
            } else {
                count_ = 0;
            }
        }

        TimerId TimerManager::Timer::id() const {
            return id_;
        }

        unsigned int TimerManager::Timer::uniqueId() const {
            return id_->second;
        }
    }
}