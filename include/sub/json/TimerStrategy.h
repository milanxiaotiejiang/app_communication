//
// Created by lijiang on 2021/12/18.
//

#ifndef APP_COMMUNICATION_TIMERSTRATEGY_H
#define APP_COMMUNICATION_TIMERSTRATEGY_H

#include "MessageStrategy.h"
#include "string"
#include <model/BaseMethod.h>

#include "model/TimerInfo.h"
#include "model/BaseResult.h"
#include "tool/write_file.hpp"

#include "prohibition.h"

#include <nlohmann/json.hpp>

using json = nlohmann::json;
using namespace std;

class AddTimerStrategy : public MessageStrategy<TimerInfo, string> {
public:
    string handler(TimerInfo params) override;
};

class UpdateTimerStrategy : public MessageStrategy<TimerInfo, string> {
public:
    string handler(TimerInfo params) override;
};

class GetTimerListStrategy : public MessageStrategy<string, std::vector<TimerInfo>> {
public:
    vector<TimerInfo> handler(string params) override;
};

class DeleteTimerStrategy : public MessageStrategy<string, string> {
public:
    string handler(string method) override;
};


#endif //APP_COMMUNICATION_TIMERSTRATEGY_H
