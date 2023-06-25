/**
 * @file ViewPartStrategy.h
 * @author Quan Li (you@domain.com)
 * @brief 路径组合中的单个零件操作
 * @version 0.1
 * @date 2022-03-04
 *
 * @copyright Copyright (c) 2022
 *
 */
#ifndef APP_COMMUNICATION_VIEWPARTSTRATEGY_H
#define APP_COMMUNICATION_VIEWPARTSTRATEGY_H

#include "MessageStrategy.h"
#include "model/ViewPart.h"
#include "string"
#include "tool/write_file.hpp"
#include "model/UpgradeModel.h"

class CombinationPartAddStrategy : public MessageStrategy<ViewPart, std::string> {
public:
    std::string handler(ViewPart method) override;
};

class CombinationPartListStrategy : public MessageStrategy<std::string, std::vector<ViewPart>> {
public:
    std::vector<ViewPart> handler(std::string params) override;
};

class CombinationPartListStrategyV2 : public MessageStrategy<std::string, std::vector<ViewPartUpgrade>> {
public:
std::vector<ViewPartUpgrade> handler(std::string params) override;
};

class CombinationPartDeleteStrategy : public MessageStrategy<std::string, std::string> {
public:
    std::string handler(std::string params) override;
};

class CombinationPartDeleteForceStrategy : public MessageStrategy<std::string, std::string> {
public:
    std::string handler(std::string params) override;
};

class CombinationPartUpdateStrategy : public MessageStrategy<ViewPart, std::string> {
public:
    std::string handler(ViewPart params) override;
};


#endif//APP_COMMUNICATION_VIEWPARTSTRATEGY_H