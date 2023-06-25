//
// Created by Looper on 2022/11/8.
//

#ifndef APP_COMMUNICATION_ASYNC_CALL_H
#define APP_COMMUNICATION_ASYNC_CALL_H

#include "thread"
#include "simulation.h"
#include "BaseThrowable.h"

class AsyncCall {
private:
    bool is_initialize = false;

protected:

    static void *run(void *arg);

    static int make_thread(void *(*fn)(void *), void *arg);

    virtual void execute() = 0;

};


#endif //APP_COMMUNICATION_ASYNC_CALL_H
