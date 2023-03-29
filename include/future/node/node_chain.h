//
// Created by Looper on 2023/3/7.
//

#ifndef APP_COMMUNICATION_NODE_CHAIN_H
#define APP_COMMUNICATION_NODE_CHAIN_H

#include "future/thread_pool.h"
#include "node_observer_mode.h"

class NodeChain {
private:
    async::ThreadPool *pool;
    bool previous{true};

public:

    NodeChain(async::ThreadPool *pool);

    async::ThreadPool *getPool() const;

    bool isPrevious() const;

    void setPrevious(bool previous);

};


#endif //APP_COMMUNICATION_NODE_CHAIN_H
