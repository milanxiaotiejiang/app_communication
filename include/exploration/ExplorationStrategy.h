//
// Created by Looper on 2022/10/13.
//

#ifndef APP_COMMUNICATION_EXPLORATIONSTRATEGY_H
#define APP_COMMUNICATION_EXPLORATIONSTRATEGY_H

#include "sub/json/MessageStrategy.h"
#include "model/RoomVo.h"

class ExplorationRoomStrategy : public MessageStrategy<RoomExplorationTarget, RoomCoverage> {
public:
    RoomCoverage handler(RoomExplorationTarget params) override;
};

class PlanParamGetStrategy : public MessageStrategy<string, PlanParam> {
public:
    PlanParam handler(string params) override;
};

class PlanParamSetStrategy : public MessageStrategy<PlanParam, bool> {
public:
    bool handler(PlanParam params) override;
};

class PlanParamResetStrategy : public MessageStrategy<string, PlanParam> {
public:
    PlanParam handler(string params) override;
};

class SetExplorerEnergyStrategy : public MessageStrategy<bool, bool> {
public:
    bool handler(bool params) override;
};

class GetExplorerEnergyStrategy : public MessageStrategy<string, bool> {
public:
    bool handler(string params) override;
};

class ExplorationTaskStrategy : public MessageStrategy<long, RoomCoverage> {
public:
    RoomCoverage handler(long params) override;
};

#endif //APP_COMMUNICATION_EXPLORATIONSTRATEGY_H
