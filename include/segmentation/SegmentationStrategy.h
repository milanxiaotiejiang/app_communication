//
// Created by Looper on 2022/9/16.
//

#ifndef APP_COMMUNICATION_SEGMENTATIONSTRATEGY_H
#define APP_COMMUNICATION_SEGMENTATIONSTRATEGY_H

#include "sub/json/MessageStrategy.h"
#include "model/RoomVo.h"

class RoomMapDataStrategy : public MessageStrategy<std::string, MapRoomVo> {
public:
    MapRoomVo handler(std::string params) override;
};

class RoomMergeStrategy : public MessageStrategy<RoomMergeTarget, MapRoomVo> {
public:
    MapRoomVo handler(RoomMergeTarget params) override;
};

class RoomSegmentationStrategy : public MessageStrategy<RoomSegmentationTarget, MapRoomVo> {
public:
    MapRoomVo handler(RoomSegmentationTarget params) override;
};

class RoomResetStrategy : public MessageStringStrategy {
public:
    void handler() override;
};

class RoomRenameStrategy : public MessageStrategy<RoomNameTarget, std::string> {
public:
    std::string handler(RoomNameTarget params) override;
};

class AutoSegmentationStrategy : public MessageStrategy<std::string, MapRoomVo> {
public:
    MapRoomVo handler(std::string params) override;
};

#endif //APP_COMMUNICATION_SEGMENTATIONSTRATEGY_H
