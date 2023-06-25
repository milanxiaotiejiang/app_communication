//
// Created by admin1 on 22-11-22.
//

#ifndef APP_COMMUNICATION_FULLCLEANINGMODESTRATEGY_H
#define APP_COMMUNICATION_FULLCLEANINGMODESTRATEGY_H

#include "sub/json/MessageStrategy.h"
#include "model/WorkStatus.h"
#include "tool/write_file.hpp"
#include "manager/FullCleanManager.h"

class FullCLeanListStrategy : public MessageStrategy<std::string, std::vector<FullCleanBrief>> {
public:
    std::vector<FullCleanBrief> handler(std::string params) override;
};

class FullCLeaningAddStrategy : public MessageStrategy<FullCleanBrief, std::string> {
public:
    std::string handler(FullCleanBrief params) override;
};

class FullCLeaningDeleteStrategy : public MessageStrategy<std::string, std::string> {
public:
    std::string handler(std::string params) override;
};

class FullCleaningUpdateStrategy : public MessageStrategy<FullCleanBrief, std::string> {
public:
    std::string handler(FullCleanBrief params) override;
};

class FullCleaningDetailsStrategy : public MessageStrategy<std::string, FullCleanBrief> {
    FullCleanBrief handler(std::string params) override;
};

class FullCleaningMainStrategy : public MessageStrategy<std::string, std::string> {
public:
    std::string handler(std::string params) override;
};

class CancelFullCleaningMainStrategy : public MessageStrategy<std::string, std::string> {
public:
    std::string handler(std::string params) override;
};

#endif //APP_COMMUNICATION_FULLCLEANINGMODESTRATEGY_H
