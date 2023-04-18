//
// Created by admin1 on 22-11-22.
//

#ifndef APP_COMMUNICATION_FULLCLEANINGMODESTRATEGY_H
#define APP_COMMUNICATION_FULLCLEANINGMODESTRATEGY_H

#include "sub/json/MessageStrategy.h"
#include "model/WorkStatus.h"
#include "tool/write_file.hpp"
#include "manager/FullCleanManager.h"

class FullCLeanListStrategy : public MessageStrategy<string, vector<FullCleanBrief>> {
public:
    vector<FullCleanBrief> handler(string params) override;
};

class FullCLeaningAddStrategy : public MessageStrategy<FullCleanBrief, string> {
public:
    string handler(FullCleanBrief params) override;
};

class FullCLeaningDeleteStrategy : public MessageStrategy<string, string> {
public:
    string handler(string params) override;
};

class FullCleaningUpdateStrategy : public MessageStrategy<FullCleanBrief, string> {
public:
    string handler(FullCleanBrief params) override;
};

class FullCleaningDetailsStrategy : public MessageStrategy<string, FullCleanBrief> {
    FullCleanBrief handler(string params) override;
};

class FullCleaningMainStrategy : public MessageStrategy<string, string> {
public:
    string handler(string params) override;
};

class CancelFullCleaningMainStrategy : public MessageStrategy<string, string> {
public:
    string handler(string params) override;
};

#endif //APP_COMMUNICATION_FULLCLEANINGMODESTRATEGY_H
