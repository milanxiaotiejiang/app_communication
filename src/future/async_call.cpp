//
// Created by Looper on 2022/11/8.
//

#include "future/async_call.h"

int AsyncCall::make_thread(void *(*fn)(void *), void *arg) {
    int err;
    pthread_t tid;
    pthread_attr_t attr;
    err = pthread_attr_init(&attr);
    if (err != 0)
        return err;

    err = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    if (err != 0)
        return err;

    pthread_create(&tid, &attr, fn, arg);

    pthread_attr_destroy(&attr);
    return err;
}

void *AsyncCall::run(void *arg) {
    auto call = (AsyncCall *) arg;
    call->execute();
    return nullptr;
}

