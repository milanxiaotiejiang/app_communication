//
// Created by Looper on 2022/11/14.
//

#ifndef APP_COMMUNICATION_FUTURE_H
#define APP_COMMUNICATION_FUTURE_H

#include "future/try.h"
#include "future/scheduler.h"

#include <memory>
#include "functional"
#include "mutex"
#include "atomic"

namespace async {
    namespace internal {

        enum class Progress {
            None,
            Done,
        };

        using TimeoutCallback = std::function<void()>;

        template<typename T>
        struct State {
            static_assert(std::is_same<T, void>::value ||
                          std::is_copy_constructible<T>() ||
                          std::is_move_constructible<T>(),
                          "must be copyable or movable or void"
            );

            State() : progress_(Progress::None), retrieved_{false} {
            }

            std::mutex thenLock_;

            using ValueType = typename TryWrapper<T>::Type;
            ValueType value_;

            std::function<void(ValueType &&)> then_;

            Progress progress_;

            std::function<void(TimeoutCallback &&)> onTimeout_;

            std::atomic<bool> retrieved_;

            bool isRoot() const {
                return !onTimeout_;
            }
        };
    }

    template<typename T>
    class Future;

    template<typename T>
    class Promise {
    private:
        std::shared_ptr<internal::State<T>> state_;

    public:
        Promise() : state_(std::make_shared<internal::State<T>>()) {
        }

        Promise(const Promise &) = default;

        Promise &operator=(const Promise &) = default;

        Promise(Promise &&pm) = default;

        Promise &operator=(Promise &&pm) = default;


        void setException(std::exception_ptr exp) {
            std::unique_lock<std::mutex> guard(state_->thenLock_);

            bool isRoot = state_->isRoot();
            if (isRoot && state_->progress_ != internal::Progress::None)
                return;

            state_->progress_ = internal::Progress::Done;
            state_->value_ = typename internal::State<T>::ValueType(std::move(exp));
            guard.unlock();

            if (state_->then_)
                state_->then_(std::move(state_->value_));
        }

        template<typename SHIT = T>
        typename std::enable_if<!std::is_void<SHIT>::value, void>::type setValue(SHIT &&t) {
            std::unique_lock<std::mutex> guard(state_->thenLock_);

            bool isRoot = state_->isRoot();
            if (isRoot && state_->progress_ != internal::Progress::None)
                return;

            state_->progress_ = internal::Progress::Done;
            state_->value_ = std::forward<SHIT>(t);

            guard.unlock();
        }

        template<typename SHIT = T>
        typename std::enable_if<std::is_void<SHIT>::value, void>::type setValue() {
            std::unique_lock<std::mutex> guard(state_->thenLock_);

            if (state_->progress_ != internal::Progress::None)
                return;

            state_->progress_ = internal::Progress::Done;
            state_->value_ = Try<void>();

            guard.unlock();
        }

        Future<T> getFuture() {
            bool expect = false;
            if (!state_->retrieved_.compare_exchange_strong(expect, true)) {
                throw std::runtime_error("Future already retrieved");
            }
            return Future<T>(state_);
        }

        bool isReady() const {
            return state_->progress_ != internal::Progress::None;
        }
    };


    template<typename T>
    class Future {
    private:
        std::shared_ptr<internal::State<T>> state_;

    public:
        template<typename U>
        friend
        class Future;

        Future() = default;

        Future(const Future &) = delete;

        void operator=(const Future &) = delete;

        Future(Future &&fut) = default;

        Future &operator=(Future &&fut) = default;

        explicit Future(std::shared_ptr<internal::State<T>> state) : state_(std::move(state)) {
        }

        bool valid() const {
            return state_ != nullptr;
        }

    };

    inline Future<void> makeReadyFuture() {
        Promise<void> pm;
        auto f(pm.getFuture());
        pm.setValue();
        return f;
    }
}

#endif //APP_COMMUNICATION_FUTURE_H
