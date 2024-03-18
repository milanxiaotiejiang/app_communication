//
// Created by mi on 2022/8/1.
//

#ifndef APP_COMMUNICATION_MATERIALSTRATEGY_H
#define APP_COMMUNICATION_MATERIALSTRATEGY_H

#include "MessageStrategy.h"
#include "model/consumable.h"
#include "db/consumable_model.h"
#include "db/property_data_base.h"

class GetConsumableStrategy : public MessageStrategy<std::string, ConsumableVo> {
public:
    ConsumableVo handler(std::string params) override;
};

class ResetConsumableStrategy : public MessageStrategy<ResetConsumableVo, ConsumableVo> {
public:
    ConsumableVo handler(ResetConsumableVo params) override;
};

class HotWindModeStrategy : public MessageStrategy<int, std::string> {
public:
    std::string handler(int params) override;
};

class HotWindModeStatusStrategy : public MessageStrategy<std::string, bool> {
public:
    bool handler(std::string params) override;
};

class MaintenanceModeStrategy : public MessageStrategy<int, std::string> {
public:
    std::string handler(int params) override;
};

class MaintenanceModeStatusStrategy : public MessageStrategy<std::string, bool> {
public:
    bool handler(std::string params) override;
};

class SewagePumpSwitchStrategy : public MessageStrategy<bool, std::string> {
public:
    std::string handler(bool params) override;

};

#endif //APP_COMMUNICATION_MATERIALSTRATEGY_H
