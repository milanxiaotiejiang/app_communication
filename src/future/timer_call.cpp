//
// Created by Looper on 2022/11/14.
//

#include "future/timer_call.h"
#include "future/async_defer.h"

namespace async {

    TimerCall::TimerCall() : state_{State::eS_None} {
    }

    void TimerCall::_startWorkers() {
        assert(state_ == State::eS_None);

        std::mutex mutex;
        std::condition_variable cond;

        pool_.setNumOfThreads(numLoop_);
        for (size_t i = 0; i < numLoop_; ++i) {
            pool_.execute([this, &mutex, &cond]() {
                EventLoop *loop(new EventLoop);

                {
                    std::unique_lock<std::mutex> guard(mutex);
                    loops_.push_back(std::unique_ptr<EventLoop>(loop));
                    if (loops_.size() == numLoop_)
                        cond.notify_one();
                }

                loop->run();
            });
        }

        std::unique_lock<std::mutex> guard(mutex);
        cond.wait(guard, [this]() {
            return loops_.size() == numLoop_;
        });

        state_ = State::eS_Started;
    }

    void TimerCall::run() {
        if (state_ != State::eS_None)
            return;

        _startWorkers();
        baseLoop()->run();

        pool_.joinAll();
        loops_.clear();
        numLoop_ = 0;
    }

    void TimerCall::exit() {
        if (state_ == State::eS_Stopped)
            return;

        state_ = State::eS_Stopped;
    }

    bool TimerCall::isExit() const {
        return state_ == State::eS_Stopped;
    }

    EventLoop *TimerCall::baseLoop() {
        return &base_;
    }

    void TimerCall::setNumOfWorker(size_t num) {
        assert(state_ == State::eS_None);
        assert(num <= 512);

        numLoop_ = num;
    }

    size_t TimerCall::numOfWorker() const {
        return 1 + numLoop_;
    }

    void TimerInitCall::initialize() {
        TimerInitCall::make_thread(run, this);
        pthread_cond_wait(&cond, &mutex);
    }

    void TimerInitCall::execute() {
        auto &timerCall = TimerCall::instance();
        timerCall.setNumOfWorker(0);

        // Do scheduled tasks, delayed loading and other things
        auto &loop = *timerCall.baseLoop();

        loop.scheduleAfter(std::chrono::seconds(1), [this]() {
            LOG(INFO) << "init TimerInitCall. " << pthread_self();
            pthread_cond_signal(&cond);
        });

        timerCall.run();
    }
}