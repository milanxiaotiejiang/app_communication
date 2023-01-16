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


#endif //APP_COMMUNICATION_EXPLORATIONSTRATEGY_H
