/**
 * @file CombinationStartegy.h
 * @author Quan Li (you@domain.com)
 * @brief 路径组合中对组合进行的操作
 * @version 0.1
 * @date 2022-03-04
 *
 * @copyright Copyright (c) 2022
 *
 */
#ifndef APP_COMMUNICATION_COMBINATIONSTRATEGY_H
#define APP_COMMUNICATION_COMBINATIONSTRATEGY_H

#include "MessageStrategy.h"
#include "manager/CombinationManager.h"
#include "manager/ViewPartManager.h"
#include "tool/write_file.hpp"
#include "model/UpgradeModel.h"

class CombinationCombinationAddStartegy : public MessageStrategy<CombinationDetail, std::string> {
public:
    std::string handler(CombinationDetail params) override;
};

class CombinationCombinationListStrategy : public MessageStrategy<std::string, std::vector<CombinationBrief>> {
public:
    std::vector<CombinationBrief> handler(std::string params) override;
};

class CombinationCombinationListStrategyV2 : public MessageStrategy<std::string, std::vector<CombinationBriefUpgrade>> {
public:
std::vector<CombinationBriefUpgrade> handler(std::string params) override;
};

class CombinationCombinationDetailsStrategy : public MessageStrategy<std::string, CombinationDetail> {
public:
    CombinationDetail handler(std::string params) override;
};

class CombinationCombinationDetailsStrategyV2 : public MessageStrategy<std::string, CombinationDetailUpgrade> {
public:
    CombinationDetailUpgrade handler(std::string params) override;
};

class CombinationCombinationDeleteStrategy : public MessageStrategy<std::string, std::string> {
public:
    std::string handler(std::string params) override;
};

class CombinationCombinationUpdateStrategy : public MessageStrategy<CombinationDetail, std::string> {
public:
    std::string handler(CombinationDetail params) override;
};

class CombinationMainStrategy : public MessageStrategy<std::string, std::string> {
public:
    std::string handler(std::string params) override;
};

class CancelCombinationMainStrategy : public MessageStrategy<std::string, std::string> {
public:
    std::string handler(std::string params) override;
};


#endif//APP_COMMUNICATION_TASKSTRATEGY_H