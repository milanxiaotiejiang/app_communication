//
// Created by Looper on 2022/11/14.
//

#ifndef APP_COMMUNICATION_TIMER_CALL_H
#define APP_COMMUNICATION_TIMER_CALL_H

#include "pthread.h"
#include "future/async_call.h"
#include "future/event_loop.h"
#include "future/thread_pool.h"

namespace async {
    class TimerCall {
    private:
        TimerCall();

        TimerCall(TimerCall &) = delete;

        TimerCall &operator=(const TimerCall &) = delete;

    public:
        ~TimerCall() = default;

    private:
        void _startWorkers();

        EventLoop base_;

        ThreadPool pool_;
        std::vector<std::unique_ptr<EventLoop>> loops_;
        size_t numLoop_{0};

        enum class State {
            eS_None,
            eS_Started,
            eS_Stopped,
        };
        std::atomic<State> state_;

    public:
        static auto &instance() {
            static TimerCall obj;
            return obj;
        }

        void run();

        void exit();

        bool isExit() const;

        EventLoop *baseLoop();

        void setNumOfWorker(size_t num);

        size_t numOfWorker() const;
    };

    class TimerInitCall : public AsyncCall {
    private:
        TimerInitCall() = default;

        TimerInitCall(TimerInitCall &) = delete;

        TimerInitCall &operator=(const TimerInitCall &) = delete;

    public:
        ~TimerInitCall() = default;

    private:
        pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
        pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

    public:
        static auto &instance() {
            static TimerInitCall obj;
            return obj;
        }

        void initialize();

    protected:
        void execute() override;

    };

}


#endif //APP_COMMUNICATION_TIMER_CALL_H
