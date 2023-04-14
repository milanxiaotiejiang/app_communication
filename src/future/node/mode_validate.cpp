//
// Created by Looper on 2023/4/11.
//

#include "future/node/mode_validate.h"
#include "simulation.h"
#include "glog/logging.h"
#include "future/node/node_control.h"
#include "future/thread_pool.h"

bool ModeValidate::validate(node::State state) {
    if (!Environment::instance().isRealEnvironment) {
        return true;
    }

    NodeControl::instance().heart_beat = 0;

    std::mutex wait_mutex;
    std::condition_variable cond;

    std::unique_lock<std::mutex> lck(wait_mutex);
    cond.wait_for(lck, std::chrono::seconds(2));

    LOG(INFO) << "最终启动结果 "
              << "  state： " << static_cast<int>(state)
              << "  carto_mode： " << NodeControl::instance().cartoMode()
              << "  heart_beat： " << NodeControl::instance().heart_beat;

    // 0定位，1建图，2睡眠
    switch (state) {
        case node::State::sleep:
            return NodeControl::instance().cartoMode() == 2 && NodeControl::instance().heart_beat < 20;
        case node::State::work:
            return NodeControl::instance().cartoMode() == 0 && NodeControl::instance().heart_beat > 30;
        case node::State::map:
            return NodeControl::instance().cartoMode() == 1 && NodeControl::instance().heart_beat > 30;
    }
}