//
// Created by Looper on 2023/3/7.
//

#include "future/node/node_control.h"

void node::NodeControl::initialize() {
    assert(state_ == State::normal);

    pool_.setNumOfThreads(THREAD_POOL_MAX_NUM);

    nodeSubject = new AbnormalSubject();
    nodeObserver = new AbnormalObserver(nodeSubject);
    nodeSubject->attach(nodeObserver);
}

void node::NodeControl::release() {
    nodeSubject->detach(nodeObserver);
    nodeObserver = nullptr;
    nodeSubject = nullptr;
}
