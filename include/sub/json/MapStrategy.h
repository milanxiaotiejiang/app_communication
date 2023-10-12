//
// Created by lijiang on 2021/12/18.
//

#ifndef APP_COMMUNICATION_MAPSTRATEGY_H
#define APP_COMMUNICATION_MAPSTRATEGY_H

#include "MessageStrategy.h"
#include "string"
#include "net/base/BaseMethod.h"

#include "model/MapInfo.h"
#include "net/base/BaseResult.h"
#include "tool/write_file.hpp"

#include "prohibition.h"
#include "model/RoomVo.h"

#include <nlohmann/json.hpp>

using json = nlohmann::json;

class FactoryResetStrategy: public MessageStrategy<std::string, std::string> {
public:
    std::string handler(std::string params) override;
};

class StartMapStrategy : public MessageStrategy<std::string, std::string> {
public:
    std::string handler(std::string params) override;
};

class EndMapStrategy : public MessageStrategy<MapParam, MapScore> {
public:
    MapScore handler(MapParam params) override;

    void removeAncientNeeds() const;
};

class GetMultiMapsStrategy : public MessageStrategy<std::string, std::vector<MapInfo>> {
public:
    std::vector<MapInfo> handler(std::string params) override;
};

class ChangeMapStrategy : public MessageStrategy<std::string, std::string> {
public:
    std::string handler(std::string params) override;
};

class ModifyMapNameStrategy : public MessageStrategy<MapInfo, std::string> {
public:
    std::string handler(MapInfo params) override;
};

class DeleteMapStrategy : public MessageStrategy<std::string, std::string> {
public:
    std::string handler(std::string params) override;
};

class EditMapStrategy : public MessageStrategy<std::vector<std::vector<float>>, std::string> {
public:
    std::string handler(std::vector<std::vector<float>> params) override;
};

class GetEditMapStrategy : public MessageStrategy<std::string, std::vector<std::vector<float>>> {
public:
    std::vector<std::vector<float>> handler(std::string params) override;
};

class ManualPushStartStrategy : public MessageStrategy<std::string, int> {
public:
    int handler(std::string params) override;
};

class ManualPushResetStrategy : public MessageStrategy<std::string, int> {
public:
    int handler(std::string params) override;
};

class MapObstaclesStrategy : public MessageStrategy<std::vector<std::vector<PointVo>>, std::string> {
public:
    std::string handler(std::vector<std::vector<PointVo>> params) override;
};

class MapFeasibleZoneStrategy : public MessageStrategy<std::vector<std::vector<PointVo>>, std::string> {
public:
    std::string handler(std::vector<std::vector<PointVo>> params) override;
};

class MapApplyIncreaseArea : public MessageStrategy<std::vector<int>, std::string> {
public:
    std::string handler(std::vector<int> params) override;
};

#endif //APP_COMMUNICATION_MAPSTRATEGY_H
