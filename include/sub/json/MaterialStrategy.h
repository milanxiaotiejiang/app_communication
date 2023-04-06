//
// Created by mi on 2022/8/1.
//

#ifndef APP_COMMUNICATION_MATERIALSTRATEGY_H
#define APP_COMMUNICATION_MATERIALSTRATEGY_H

#include "MessageStrategy.h"
#include "model/MaterialStatus.h"
#include "model/consumable.h"
#include "db/consumable_model.h"
#include "db/property_data_base.h"

class GetMaterialStrategy : public MessageStrategy<string, VersionSubscribe<MaterialStatus>> {
public:
    VersionSubscribe<MaterialStatus> handler(string params) override;
};

class GetConsumableStrategy : public MessageStrategy<string, ConsumableVo> {
public:
    ConsumableVo handler(string params) override;
};

class ResetConsumableStrategy : public MessageStrategy<ResetConsumableVo, ConsumableVo> {
public:
    ConsumableVo handler(ResetConsumableVo params) override;
};


#endif //APP_COMMUNICATION_MATERIALSTRATEGY_H
