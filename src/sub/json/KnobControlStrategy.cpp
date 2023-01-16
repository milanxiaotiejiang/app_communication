//
// Created by mi on 2022/8/23.
//

#include "sub/json/KnobControlStrategy.h"

string KnobControlStrategy::handler(bool method) {
    std_msgs::Bool aBool;
    aBool.data = method;
    PublishInnerManager::instance().getPubInner()->publishKnobAvailable(aBool);
    return "";
}
