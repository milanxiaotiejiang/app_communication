//
// Created by Looper on 2022/10/8.
//

#ifndef APP_COMMUNICATION_TIMER_H
#define APP_COMMUNICATION_TIMER_H

#include <sys/time.h>
#include <cstdlib>

class Timer {
public:
    Timer() {
        startCount.tv_sec = startCount.tv_usec = 0;
        endCount.tv_sec = endCount.tv_usec = 0;

        stopped = 0;
        startTimeInMicroSec = 0;
        endTimeInMicroSec = 0;

        start();
    }

    ~Timer() {
    }

    void start() {
        stopped = 0; // reset stop flag
        gettimeofday(&startCount, NULL);
    }

    void stop() {
        stopped = 1; // set timer stopped flag
        gettimeofday(&endCount, NULL);
    }

    double getElapsedTime() {
        return this->getElapsedTimeInSec();
    }

    double getElapsedTimeInSec() {
        return this->getElapsedTimeInMicroSec() * 0.000001;
    }

    double getElapsedTimeInMilliSec() {
        return this->getElapsedTimeInMicroSec() * 0.001;
    }

    double getElapsedTimeInMicroSec() {
        if (!stopped)
            gettimeofday(&endCount, NULL);

        startTimeInMicroSec = (startCount.tv_sec * 1000000.0) + startCount.tv_usec;
        endTimeInMicroSec = (endCount.tv_sec * 1000000.0) + endCount.tv_usec;

        return endTimeInMicroSec - startTimeInMicroSec;
    }

protected:

private:
    double startTimeInMicroSec; // starting time in micro-second
    double endTimeInMicroSec; // ending time in micro-second
    int stopped; // stop flag
    timeval startCount{}; //
    timeval endCount{}; //
};


#endif //APP_COMMUNICATION_TIMER_H
