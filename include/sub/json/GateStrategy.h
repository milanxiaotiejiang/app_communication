//
// Created by io on 23-9-19.
//

#ifndef APP_COMMUNICATION_GATESTRATEGY_H
#define APP_COMMUNICATION_GATESTRATEGY_H

#include "sub/json/MessageStrategy.h"
#include "model/gate_info.h"


class AddGateStrategy : public MessageStrategy<GateInfo, int> {
public:
    int handler(GateInfo params) override;
};

class DeleteGateStrategy : public MessageStrategy<long, std::string> {
public:
    std::string handler(long params) override;
};

class PurgeGateStrategy : public MessageStringStrategy {
public:
    void handler() override;
};

class ModifyGateStrategy : public MessageStrategy<GateInfo, long> {
public:
    long handler(GateInfo params) override;
};

class ListGateStrategy : public MessageStrategy<std::string, std::vector<GateInfo>> {
public:
    std::vector<GateInfo> handler(std::string params) override;
};

class QueryIdGateStrategy : public MessageStrategy<long, GateInfo> {
public:
    GateInfo handler(long params) override;
};

class OpenGateSettingStrategy : public MessageStrategy<std::string, std::string> {
public:
    std::string handler(std::string params) override;
};

class CloseGateSettingStrategy : public MessageStrategy<std::string, std::string> {
public:
    std::string handler(std::string params) override;
};

class MultipleModifyGateStrategy: public MessageStrategy<GateSimpleInfo, long> {
public:
    long handler(GateSimpleInfo params) override;
};

class MultipleListGateStrategy: public MessageStrategy<std::string, std::vector<GateInfo>>{
public:
    std::vector<GateInfo> handler(std::string params) override;
};

class MultiplePurgeGateStrategy: public MessageStrategy<std::string, std::string>{
public:
    std::string handler(std::string params) override;
};

#endif //APP_COMMUNICATION_GATESTRATEGY_H
