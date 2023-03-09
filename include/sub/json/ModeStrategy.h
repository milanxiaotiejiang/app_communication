//
// Created by lijiang on 2021/12/23.
//

#ifndef APP_COMMUNICATION_MODESTRATEGY_H
#define APP_COMMUNICATION_MODESTRATEGY_H

#include "MessageStrategy.h"
#include <tool/Variable.h>

class RobotTryEnterModeStrategy : public MessageStrategy<int, string> {
public:
    string handler(int params) override;
};

class RobotForceEnterModeStrategy : public MessageStrategy<int, string> {
public:
    string handler(int params) override;
};

class RobotPreparetoWorkStrategy : public MessageStrategy<string, string> {
public:
    string handler(string params) override;
};

class MapPreparetoWorkStrategy : public MessageStrategy<string, string> {
public:
    string handler(string params) override;
};

#endif //APP_COMMUNICATION_MODESTRATEGY_H
