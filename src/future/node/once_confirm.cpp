//
// Created by Looper on 2023/3/7.
//

#include "future/node/once_confirm.h"
#include "glog/logging.h"

bool OnceConfirm::confirm(NodeChain chain) {
    LOG(INFO) << "最终启动的结果 : " << chain.isPrevious();
    return true;
}
