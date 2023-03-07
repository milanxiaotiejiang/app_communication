//
// Created by Looper on 2023/3/7.
//

#include "future/node/child_activate_node.h"
#include "glog/logging.h"

RvizActivateNode::RvizActivateNode(int seconds) : ActivateNode(seconds) {}

RvizActivateNode::~RvizActivateNode() = default;

bool RvizActivateNode::execute(NodeChain chain) {
    if (chain.isPrevious()) {
        return asyncExecute(chain.getPool(), []() {
            std::system(N_RVIZ.data());
        });
    }
    return false;
}

DumpActivateNode::DumpActivateNode(int seconds) : ActivateNode(seconds) {}

DumpActivateNode::~DumpActivateNode() = default;

bool DumpActivateNode::execute(NodeChain chain) {
    if (chain.isPrevious()) {
        return asyncExecute(chain.getPool(), []() {
            std::system(N_DUMP.data());
        });
    }
    return false;
}
