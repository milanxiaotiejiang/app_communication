//
// Created by Looper on 2023/3/7.
//

#include "future/node/child_activate_node.h"
#include "glog/logging.h"
#include "future/node/node_control.h"
#include "simulation.h"

RvizActivateNode::RvizActivateNode(int seconds) : ActivateNode(seconds) {}

RvizActivateNode::~RvizActivateNode() = default;

bool RvizActivateNode::execute(NodeChain chain) {
    if (chain.isPrevious()) {
        return asyncExecute(chain.getPool(), []() {
            int status = std::system(N_RVIZ.data());
            LOG(INFO) << "status : " << status;
        });
    }
    return false;
}

KillMapServerActivateNode::KillMapServerActivateNode(int seconds) : ActivateNode(seconds) {}

KillMapServerActivateNode::~KillMapServerActivateNode() = default;

bool KillMapServerActivateNode::execute(NodeChain chain) {
    NodeControl::instance().offSleep();
    sleep(seconds);
    return true;
}

BuildMappingActivateNode::BuildMappingActivateNode(int seconds) : ActivateNode(seconds) {}

BuildMappingActivateNode::~BuildMappingActivateNode() = default;

bool BuildMappingActivateNode::execute(NodeChain chain) {
    if (chain.isPrevious()) {
        return asyncExecute(chain.getPool(), []() {
            NodeControl::system_start(n_build_mapping);
        });
    }
    return false;
}

SubmapToMapActivateNode::SubmapToMapActivateNode(int seconds) : ActivateNode(seconds) {}

SubmapToMapActivateNode::~SubmapToMapActivateNode() = default;

bool SubmapToMapActivateNode::execute(NodeChain chain) {
    if (chain.isPrevious()) {
        return asyncExecute(chain.getPool(), []() {
            NodeControl::system_start(n_submap_to_map);
        });
    }
    return false;
}

LocalizationActivateNode::LocalizationActivateNode(int seconds) : ActivateNode(seconds) {}

LocalizationActivateNode::~LocalizationActivateNode() = default;

bool LocalizationActivateNode::execute(NodeChain chain) {
    if (chain.isPrevious()) {
        return asyncExecute(chain.getPool(), []() {
            NodeControl::system_start(n_localization);
        });
    }
    return false;
}

LoadMapActivateNode::LoadMapActivateNode(int seconds) : ActivateNode(seconds) {}

LoadMapActivateNode::~LoadMapActivateNode() = default;

bool LoadMapActivateNode::execute(NodeChain chain) {
    if (chain.isPrevious()) {
        return asyncExecute(chain.getPool(), []() {
            if (Environment::instance().isRealEnvironment) {
                NodeControl::system_start(n_load_map);
            } else {
                NodeControl::system_start(n_tt_load_map);
            }
        });
    }
    return false;
}

NavigationActivateNode::NavigationActivateNode(int seconds) : ActivateNode(seconds) {}

NavigationActivateNode::~NavigationActivateNode() = default;

bool NavigationActivateNode::execute(NodeChain chain) {
    if (chain.isPrevious()) {
        return asyncExecute(chain.getPool(), []() {
            if (Environment::instance().isRealEnvironment) {
                NodeControl::system_start(n_navigation);
            } else {
                NodeControl::system_start(N_tt_navigation);
            }
        });
    }
    return false;
}
