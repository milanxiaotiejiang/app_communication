//
// Created by lijiang on 2021/12/18.
//

#ifndef APP_COMMUNICATION_TIMERSTRATEGY_H
#define APP_COMMUNICATION_TIMERSTRATEGY_H

#include "MessageStrategy.h"
#include "string"
#include "net/base/BaseMethod.h"
#include "net/base/BaseResult.h"

#include "model/TimerInfo.h"
#include "tool/write_file.hpp"

#include "prohibition.h"

#include <nlohmann/json.hpp>

using json = nlohmann::json;

class SetTimerStrategy : public MessageStrategy<TimerInfo, std::string> {
public:
    std::string handler(TimerInfo params) override;
};

class UpdateTimerStrategy : public MessageStrategy<TimerInfo, std::string> {
public:
    std::string handler(TimerInfo params) override;
};

class GetTimerListStrategy : public MessageStrategy<std::string, std::vector<TimerInfo>> {
public:
    std::vector<TimerInfo> handler(std::string params) override;
};

class DelTimerStrategy : public MessageStrategy<std::string, std::string> {
public:
    std::string handler(std::string method) override;
};


#endif //APP_COMMUNICATION_TIMERSTRATEGY_H
