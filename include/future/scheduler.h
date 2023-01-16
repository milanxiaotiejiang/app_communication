//
// Created by Looper on 2022/11/14.
//

#ifndef APP_COMMUNICATION_SCHEDULER_H
#define APP_COMMUNICATION_SCHEDULER_H

#include "functional"
#include "chrono"

namespace async {
    class Scheduler {
    public:
        virtual ~Scheduler() = default;

        virtual void scheduleLater(std::chrono::milliseconds duration, std::function<void()> f) = 0;

        virtual void schedule(std::function<void()> f) = 0;
    };
}

#endif //APP_COMMUNICATION_SCHEDULER_H
