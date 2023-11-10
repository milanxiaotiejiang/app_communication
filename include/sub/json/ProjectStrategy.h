//
// Created by lijiang on 2022/5/26.
//

#ifndef APP_COMMUNICATION_PROJECTSTRATEGY_H
#define APP_COMMUNICATION_PROJECTSTRATEGY_H

#include "MessageStrategy.h"
#include "net/base/BaseResult.h"

#include "tool/write_file.hpp"
#include "model/ParamVo.h"

class PadVersionStrategy : public MessageStrategy<std::string, std::string> {
public:
    std::string handler(std::string params) override;
};

class MachineModelStrategy : public MessageStrategy<std::string, int> {
public:
    int handler(std::string params) override;
};

class GetRobotParamsStrategy : public MessageStrategy<std::string, ParamVo> {
public:
    ParamVo handler(std::string params) override;
};

class SetRobotParamsStrategy : public MessageStrategy<ParamVo, ParamVo> {
public:
    ParamVo handler(ParamVo params) override;
};

#endif //APP_COMMUNICATION_PROJECTSTRATEGY_H
