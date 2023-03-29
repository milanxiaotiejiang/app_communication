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
using namespace std;

class SaveMapStrategy : public MessageStrategy<MapInfo, MapInfo> {
public:
    MapInfo handler(MapInfo params) override;
};

class GetMultiMapsStrategy : public MessageStrategy<string, std::vector<MapInfo>> {
public:
    vector<MapInfo> handler(string params) override;
};

class ChangeMapStrategy : public MessageStrategy<string, int> {
public:
    int handler(string params) override;
};

class EditMapStrategy : public MessageStrategy<std::vector<std::vector<float>>, string> {
public:
    string handler(vector<std::vector<float>> params) override;
};

class GetEditMapStrategy : public MessageStrategy<string, std::vector<std::vector<float>>> {
public:
    vector<std::vector<float>> handler(string params) override;
};

class ManualPushStartStrategy : public MessageStrategy<string, int> {
public:
    int handler(string params) override;
};

class ManualPushResetStrategy : public MessageStrategy<string, int> {
public:
    int handler(string params) override;
};

class ManualPushSaveStrategy : public MessageStrategy<MapInfo, MapInfo> {
public:
    MapInfo handler(MapInfo params) override;
};

class MapObstaclesStrategy : public MessageStrategy<vector<vector<PointVo>>, string> {
public:
    string handler(vector<vector<PointVo>> params) override;
};

class MapFeasibleZoneStrategy : public MessageStrategy<vector<vector<PointVo>>, string> {
public:
    string handler(vector<vector<PointVo>> params) override;
};

#endif //APP_COMMUNICATION_MAPSTRATEGY_H
