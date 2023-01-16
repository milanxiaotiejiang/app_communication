//
// Created by lijiang on 2022/5/26.
//

#ifndef APP_COMMUNICATION_PROJECTSTRATEGY_H
#define APP_COMMUNICATION_PROJECTSTRATEGY_H

#include "MessageStrategy.h"
#include "model/BaseResult.h"
#include "model/Project.h"

#include "tool/write_file.hpp"

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

#endif //APP_COMMUNICATION_PROJECTSTRATEGY_H
