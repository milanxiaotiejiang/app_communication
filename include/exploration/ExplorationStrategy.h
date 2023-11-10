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

class PlanParamGetStrategy : public MessageStrategy<std::string, PlanParam> {
public:
    PlanParam handler(std::string params) override;
};

class PlanParamSetStrategy : public MessageStrategy<PlanParam, bool> {
public:
    bool handler(PlanParam params) override;
};

class PlanParamResetStrategy : public MessageStrategy<std::string, PlanParam> {
public:
    PlanParam handler(std::string params) override;
};

class SetExplorerEnergyStrategy : public MessageStrategy<bool, bool> {
public:
    bool handler(bool params) override;
};

class GetExplorerEnergyStrategy : public MessageStrategy<std::string, bool> {
public:
    bool handler(std::string params) override;
};

class ExplorationTaskStrategy : public MessageStrategy<long, RoomCoverage> {
public:
    RoomCoverage handler(long params) override;
};

#endif //APP_COMMUNICATION_EXPLORATIONSTRATEGY_H
