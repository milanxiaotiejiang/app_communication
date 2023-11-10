//
// Created by mi on 2022/8/23.
//

#include "sub/json/KnobControlStrategy.h"

std::string KnobControlStrategy::handler(bool method) {
    std_msgs::Bool aBool;
    aBool.data = method;
    PublishInnerManager::instance().publishKnobAvailable(aBool);
    return "";
}

std::string CollectDustStrategy::handler(std::string method) {
    PublishInnerManager::instance().publishCollectDust();
    return "";
}
