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

class CombinationPartAddStrategy : public MessageStrategy<ViewPart, string> {
public:
    string handler(ViewPart method) override;
};

class CombinationPartListStrategy : public MessageStrategy<string, vector<ViewPart>> {
public:
    vector<ViewPart> handler(string params) override;
};

class CombinationPartListStrategyV2 : public MessageStrategy<string, vector<ViewPartUpgrade>> {
public:
    vector<ViewPartUpgrade> handler(string params) override;
};

class CombinationPartDeleteStrategy : public MessageStrategy<string, string> {
public:
    string handler(string params) override;
};

class CombinationPartDeleteForceStrategy : public MessageStrategy<string, string> {
public:
    string handler(string params) override;
};

class CombinationPartUpdateStrategy : public MessageStrategy<ViewPart, string> {
public:
    string handler(ViewPart params) override;
};


#endif//APP_COMMUNICATION_VIEWPARTSTRATEGY_H