//
// Created by Looper on 2022/11/14.
//

#ifndef APP_COMMUNICATION_THREAD_POOL_H
#define APP_COMMUNICATION_THREAD_POOL_H

#include "deque"
#include "thread"
#include "memory"
#include "mutex"
#include "condition_variable"
#include "future/future.h"

/**
 * 利用了变长模板参数，对投递的任务函数签名没有任何限制，不像传统的线程实现必须接受 void func(void\* ）的签名。
 * 同时结合Future，可以方便的注册回调。
 * 线程池可以设置最大线程 数量和最小线程数量。
 * 在任务比较多的时候，线程数会增加，但不会超过你设置的线程池大小；
 * 在任务比较少的时候，会自动回收多余的线程
 */
namespace async {

    class ThreadPool final {
    private:
        int numThreads_{1};
        std::deque<std::thread> workers_;

        mutable std::mutex mutex_;
        std::condition_variable cond_;
        bool shutdown_{false};
        std::deque<std::function<void()>> tasks_;

        static const int kMaxThreads = 512;
        static std::thread::id s_mainThread;

        void _workerRoutine();

        void _start();

    public:
        ThreadPool();

        ~ThreadPool();

        ThreadPool(const ThreadPool &) = delete;

        void operator=(const ThreadPool &) = delete;

        template<typename F, typename... Args,
                typename = typename std::enable_if<!std::is_void<typename std::result_of<F(
                        Args...)>::type>::value, void>::type,
                typename Dummy = void>
        auto execute(F &&f, Args &&... args) -> Future<typename std::result_of<F(Args...)>::type>;

        template<typename F, typename... Args,
                typename = typename std::enable_if<std::is_void<typename std::result_of<F(
                        Args...)>::type>::value, void>::type>
        auto execute(F &&f, Args &&... args) -> Future<void>;

        void joinAll();

        void setNumOfThreads(int n);

        size_t workerThreads() const;

        size_t tasks() const;
    };

    template<typename F, typename... Args, typename, typename>
    auto ThreadPool::execute(F &&f, Args &&... args) -> Future<typename std::result_of<F(Args...)>::type> {
        using resultType = typename std::result_of<F(Args...)>::type;

        std::unique_lock<std::mutex> guard(mutex_);
        if (shutdown_)
            throw std::runtime_error("execute on closed thread pool");

        if (workers_.empty()) {
            _start();
        }

        Promise<resultType> promise;
        auto future = promise.GetFuture();

        auto func = std::bind(std::forward<F>(f), std::forward<Args>(args)...);
        auto task = [t = std::move(func), pm = std::move(promise)]() mutable {
            try {
                pm.SetValue(Try<resultType>(t()));
            } catch (...) {
                pm.SetException(std::current_exception());
            }
        };

        tasks_.emplace_back(std::move(task));
        cond_.notify_one();

        return future;
    }

    template<typename F, typename... Args, typename>
    auto ThreadPool::execute(F &&f, Args &&... args) -> Future<void> {
        using resultType = typename std::result_of<F(Args...)>::type;
        static_assert(std::is_void<resultType>::value, "must be void");

        std::unique_lock<std::mutex> guard(mutex_);
        if (shutdown_)
            return makeReadyFuture();

        if (workers_.empty()) {
            _start();
        }

        Promise<resultType> promise;
        auto future = promise.getFuture();

        auto func = std::bind(std::forward<F>(f), std::forward<Args>(args)...);
        auto task = [t = std::move(func), pm = std::move(promise)]() mutable {
            try {
                t();
                pm.setValue();
            } catch (...) {
                pm.setException(std::current_exception());
            }
        };

        tasks_.emplace_back(std::move(task));
        cond_.notify_one();

        return future;
    }
}


#endif //APP_COMMUNICATION_THREAD_POOL_H
