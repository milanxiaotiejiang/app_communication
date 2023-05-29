//
// Created by Looper on 2023/3/7.
//

#include "future/node/once_confirm.h"
#include "glog/logging.h"
#include "future/node/node_control.h"
#include "simulation.h"

bool OnceConfirm::confirm(NodeChain chain) {

    if (!Environment::instance().isRealEnvironment) {
        return true;
    }

    NodeControl::instance().heart_beat = 0;

    std::mutex wait_mutex;
    std::condition_variable cond;

    std::unique_lock<std::mutex> lck(wait_mutex);
    cond.wait_for(lck, std::chrono::seconds(2));

    LOG(INFO) << "最终启动的结果 : " << NodeControl::instance().heart_beat;
    return NodeControl::instance().heart_beat > 10;
}
