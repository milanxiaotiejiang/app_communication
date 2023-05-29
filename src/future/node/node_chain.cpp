//
// Created by Looper on 2023/3/7.
//

#include "future/node/node_chain.h"

bool NodeChain::isPrevious() const {
    return previous;
}

void NodeChain::setPrevious(bool previous) {
    NodeChain::previous = previous;
}

async::ThreadPool *NodeChain::getPool() const {
    return pool;
}

NodeChain::NodeChain(async::ThreadPool *pool) : pool(pool) {}
