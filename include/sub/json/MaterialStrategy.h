//
// Created by mi on 2022/8/1.
//

#ifndef APP_COMMUNICATION_MATERIALSTRATEGY_H
#define APP_COMMUNICATION_MATERIALSTRATEGY_H

#include "MessageStrategy.h"
#include "model/consumable.h"
#include "db/consumable_model.h"
#include "db/property_data_base.h"

class GetConsumableStrategy : public MessageStrategy<string, ConsumableVo> {
public:
    ConsumableVo handler(string params) override;
};

class ResetConsumableStrategy : public MessageStrategy<ResetConsumableVo, ConsumableVo> {
public:
    ConsumableVo handler(ResetConsumableVo params) override;
};

class MaintenanceModeStrategy : public MessageStrategy<int, string> {
public:
    string handler(int params) override;
};

class MaintenanceModeStatusStrategy : public MessageStrategy<string, bool> {
public:
    bool handler(string params) override;
};

#endif //APP_COMMUNICATION_MATERIALSTRATEGY_H
