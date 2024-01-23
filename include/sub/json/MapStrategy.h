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
#include "net/base/CompositeList.h"

#include <nlohmann/json.hpp>

using json = nlohmann::json;

class FactoryResetStrategy : public MessageStrategy<std::string, std::string> {
public:
    std::string handler(std::string params) override;
};

class StartMapStrategy : public MessageStrategy<std::string, std::string> {
public:
    std::string handler(std::string params) override;
};

class EndMapStrategy : public MessageStrategy<BuildMapParam, MapScore> {
public:
    MapScore handler(BuildMapParam params) override;
};

class GetMultiMapsStrategy : public MessageStrategy<std::string, std::vector<MultiMapInfo>> {
public:
    std::vector<MultiMapInfo> handler(std::string params) override;
};

class ChangeMapStrategy : public MessageStrategy<std::string, std::string> {
public:
    std::string handler(std::string params) override;
};

class ModifyMapNameStrategy : public MessageStrategy<ModifyMapName, std::string> {
public:
    std::string handler(ModifyMapName params) override;
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

class MultipleEditMapStrategy : public MessageStrategy<CompositeFloatList, std::string> {
public:
    std::string handler(CompositeFloatList params) override;
};

class MultipleGetEditMapStrategy : public MessageStrategy<std::string, CompositeFloatList> {
public:
    CompositeFloatList handler(std::string params) override;
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

class MultipleMapObstaclesStrategy : public MessageStrategy<CompositePointList, std::string> {
public:
    std::string handler(CompositePointList params) override;
};

class MultipleMapFeasibleZoneStrategy : public MessageStrategy<CompositePointList, std::string> {
public:
    std::string handler(CompositePointList params) override;
};

class AddBuildStrategy : public MessageStrategy<BuildVo, long> {
public:
    long handler(BuildVo params) override;
};

class DeleteBuildStrategy : public MessageStrategy<long, std::string> {
public:
    std::string handler(long params) override;
};

class ModifyBuildNameStrategy : public MessageStrategy<BuildVo, std::string> {
public:
    std::string handler(BuildVo params) override;
};

class ListBuildStrategy : public MessageStrategy<std::string, std::vector<BuildVo>> {
public:
    std::vector<BuildVo> handler(std::string params) override;
};

class ModifyMapBaseStationStrategy : public MessageStrategy<MapBaseStation, std::string> {
public:
    std::string handler(MapBaseStation params) override;
};

class ModifyMapFloorStrategy : public MessageStrategy<MapFloor, std::string> {
public:
    std::string handler(MapFloor params) override;
};

class ModifyMapElevatorStrategy : public MessageStrategy<MapElevator, std::string> {
public:
    std::string handler(MapElevator params) override;
};

class ListMapForBuildStrategy : public MessageStrategy<long, std::vector<MultiMapInfo>> {
public:
    std::vector<MultiMapInfo> handler(long params) override;
};

class AttachBuildMapStrategy : public MessageStrategy<AttachBuildMap, std::string> {
public:
    std::string handler(AttachBuildMap params) override;
};

class MapForIdStrategy : public MessageStrategy<std::string, MultiMapInfo> {
public:
    MultiMapInfo handler(std::string params) override;
};

class TTElevatorStrategy : public MessageStrategy<std::string, std::string> {
public:
    std::string handler(std::string params) override;
};

#endif //APP_COMMUNICATION_MAPSTRATEGY_H
