//
// Created by mi on 2022/8/23.
//

#ifndef APP_COMMUNICATION_KNOBCONTROLSTRATEGY_H
#define APP_COMMUNICATION_KNOBCONTROLSTRATEGY_H

#include "MessageStrategy.h"
#include <nlohmann/json.hpp>
#include "std_msgs/Bool.h"

using json = nlohmann::json;
using namespace std;


class KnobControlStrategy : public MessageStrategy<bool, string> {
public:
    string handler(bool method) override;
};

class CollectDustStrategy : public MessageStrategy<string, string> {
public:
    string handler(string method) override;
};


#endif //APP_COMMUNICATION_KNOBCONTROLSTRATEGY_H
