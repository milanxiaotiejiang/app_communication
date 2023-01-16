//
// Created by Looper on 2022/11/14.
//

#ifndef APP_COMMUNICATION_TRY_H
#define APP_COMMUNICATION_TRY_H

#include <exception>
#include <stdexcept>

namespace async {
    template<typename T>
    class Try {

        enum class State {
            None,
            Exception,
            Value,
        };

    private:
        State state_;
        union {
            T value_;
            std::exception_ptr exception_;
        };

    public:
        Try() : state_(State::None) {
        }

        Try(const T &t) :
                state_(State::Value),
                value_(t) {
        }

        Try(T &&t) :
                state_(State::Value),
                value_(std::move(t)) {
        }

        Try(std::exception_ptr e) :
                state_(State::Exception),
                exception_(std::move(e)) {
        }

        // move
        Try(Try<T> &&t) :
                state_(t.state_) {
            if (state_ == State::Value)
                new(&value_)T(std::move(t.value_));
            else if (state_ == State::Exception)
                new(&exception_)std::exception_ptr(std::move(t.exception_));
        }

        Try<T> &operator=(Try<T> &&t) {
            if (this == &t)
                return *this;

            this->~Try();

            state_ = t.state_;
            if (state_ == State::Value)
                new(&value_)T(std::move(t.value_));
            else if (state_ == State::Exception)
                new(&exception_)std::exception_ptr(std::move(t.exception_));
            return *this;
        }

        // copy
        Try(const Try<T> &t) :
                state_(t.state_) {
            if (state_ == State::Value)
                new(&value_)T(t.value_);
            else if (state_ == State::Exception)
                new(&exception_)std::exception_ptr(t.exception_);
        }

        Try<T> &operator=(const Try<T> &t) {
            if (this == &t)
                return *this;

            this->~Try();

            state_ = t.state_;
            if (state_ == State::Value)
                new(&value_)T(t.value_);
            else if (state_ == State::Exception)
                new(&exception_)std::exception_ptr(t.exception_);
            return *this;
        }

        ~Try() {
            if (state_ == State::Exception)
                exception_.~exception_ptr();
            else if (state_ == State::Value)
                value_.~T();
        }


        operator const T &() const & {
            return value();
        }

        operator T &() & { return value(); }

        operator T &&() && { return std::move(value()); }

        const T &operator*() const {
            return value();
        }

        T &operator*() {
            return value();
        }


        // get value
        const T &value() const &{
            check();
            return value_;
        }

        T &value() &{
            check();
            return value_;
        }

        T &&value() &&{
            check();
            return std::move(value_);
        }

        // get exception
        const std::exception_ptr &exception() const &{
            if (!hasException())
                throw std::runtime_error("Not exception state");
            return exception_;
        }

        std::exception_ptr &exception() &{
            if (!hasException())
                throw std::runtime_error("Not exception state");
            return exception_;
        }

        std::exception_ptr &&exception() &&{
            if (!hasException())
                throw std::runtime_error("Not exception state");
            return std::move(exception_);
        }

        bool hasValue() const {
            return state_ == State::Value;
        }

        bool hasException() const {
            return state_ == State::Exception;
        }

        struct UninitializedTry {
        };

        void check() const {
            if (state_ == State::Exception)
                std::rethrow_exception(exception_);
            else if (state_ == State::None)
                throw UninitializedTry();
        }

        // Amazing! Thanks to folly
        template<typename R>
        R get() {
            return std::forward<R>(value());
        }
    };


    template<>
    class Try<void> {
        enum class State {
            Exception,
            Value,
        };

    private:
        State state_;
        std::exception_ptr exception_;

    public:

        Try() : state_(State::Value) {
        }

        explicit Try(std::exception_ptr e) : state_(State::Exception), exception_(std::move(e)) {
        }

        // move
        Try(Try<void> &&t) :
                state_(t.state_) {
            if (state_ == State::Exception)
                new(&exception_)std::exception_ptr(std::move(t.exception_));
        }

        Try<void> &operator=(Try<void> &&t) {
            if (this == &t)
                return *this;

            this->~Try();

            state_ = t.state_;
            if (state_ == State::Exception)
                new(&exception_)std::exception_ptr(std::move(t.exception_));
            return *this;
        }

        // copy
        Try(const Try<void> &t) :
                state_(t.state_) {
            if (state_ == State::Exception)
                new(&exception_)std::exception_ptr(t.exception_);
        }

        Try<void> &operator=(const Try<void> &t) {
            if (this == &t)
                return *this;

            this->~Try();

            state_ = t.state_;
            if (state_ == State::Exception)
                new(&exception_)std::exception_ptr(t.exception_);
            return *this;
        }

        ~Try() {
            if (state_ == State::Exception)
                exception_.~exception_ptr();
        }

        // get exception
        const std::exception_ptr &exception() const &{
            if (!hasException())
                throw std::runtime_error("Not exception state");

            return exception_;
        }

        std::exception_ptr &exception() &{
            if (!hasException())
                throw std::runtime_error("Not exception state");

            return exception_;
        }

        std::exception_ptr &&exception() &&{
            if (!hasException())
                throw std::runtime_error("Not exception state");

            return std::move(exception_);
        }

        bool hasValue() const {
            return state_ == State::Value;
        }

        bool hasException() const {
            return state_ == State::Exception;
        }

        void check() const {
            if (state_ == State::Exception)
                std::rethrow_exception(exception_);
        }

        // Amazing! Thanks to folly
        template<typename R>
        R get() {
            return std::forward<R>(*this);
        }
    };


    // TryWrapper<T> : if T is Try type, then Type is T otherwise is Try<T>
    template<typename T>
    struct TryWrapper {
        using Type = Try<T>;
    };

    template<typename T>
    struct TryWrapper<Try<T>> {
        using Type = Try<T>;
    };
}

#endif //APP_COMMUNICATION_TRY_H
