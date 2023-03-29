//
// Created by lijiang on 2022/5/26.
//

#ifndef APP_COMMUNICATION_PROJECTSTRATEGY_H
#define APP_COMMUNICATION_PROJECTSTRATEGY_H

#include "MessageStrategy.h"
#include "net/base/BaseResult.h"
#include "model/Project.h"

#include "tool/write_file.hpp"
#include "model/ParamVo.h"

class ProjectStrategy : public MessageStrategy<Project, string> {
public:
    string handler(Project params) override;
};

class getProjectStrategy : public MessageStrategy<string, string> {
public:
    string handler(string params) override;
};

class PadVersionStrategy : public MessageStrategy<string, string> {
public:
    string handler(string params) override;
};

class MachineModelStrategy : public MessageStrategy<string, int> {
public:
    int handler(string params) override;
};

class GetRobotParamsStrategy : public MessageStrategy<string, ParamVo> {
public:
    ParamVo handler(string params) override;
};

class SetRobotParamsStrategy : public MessageStrategy<ParamVo, ParamVo> {
public:
    ParamVo handler(ParamVo params) override;
};

#endif //APP_COMMUNICATION_PROJECTSTRATEGY_H
