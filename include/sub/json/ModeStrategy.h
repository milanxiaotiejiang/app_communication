//
// Created by lijiang on 2021/12/23.
//

#ifndef APP_COMMUNICATION_MODESTRATEGY_H
#define APP_COMMUNICATION_MODESTRATEGY_H

#include "MessageStrategy.h"
#include <tool/Variable.h>

class RobotTryEnterModeStrategy : public MessageStrategy<int, std::string> {
public:
    std::string handler(int params) override;
};

class RobotForceEnterModeStrategy : public MessageStrategy<int, std::string> {
public:
    std::string handler(int params) override;
};

class RobotPreparetoWorkStrategy : public MessageStrategy<std::string, std::string> {
public:
    std::string handler(std::string params) override;
};

class MapPreparetoWorkStrategy : public MessageStrategy<std::string, std::string> {
public:
    std::string handler(std::string params) override;
};

class OpenSelfCheckStrategy : public MessageStrategy<std::string, std::string> {
public:
    std::string handler(std::string params) override;
};

class CloseSelfCheckStrategy : public MessageStrategy<std::string, std::string> {
public:
    std::string handler(std::string params) override;
};

#endif //APP_COMMUNICATION_MODESTRATEGY_H
