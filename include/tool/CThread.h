#ifndef __thread_h__
#define __thread_h__

#include <pthread.h>

class CThread {
public:
    CThread();

    virtual ~CThread();

    pthread_t self();

    virtual void *run() = 0;

    int start();

    int join();

    int detach();

    int stop();

private:
    pthread_t m_tid;
    int m_running;
    int m_detached;
};

#endif