//
// Created by mi on 2022/8/23.
//

#ifndef APP_COMMUNICATION_KNOBCONTROLSTRATEGY_H
#define APP_COMMUNICATION_KNOBCONTROLSTRATEGY_H

#include "MessageStrategy.h"
#include <nlohmann/json.hpp>
#include "std_msgs/Bool.h"

using json = nlohmann::json;

class CollectDustStrategy : public MessageStrategy<std::string, std::string> {
public:
    std::string handler(std::string method) override;
};


#endif //APP_COMMUNICATION_KNOBCONTROLSTRATEGY_H
