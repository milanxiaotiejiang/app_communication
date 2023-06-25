//
// Created by lijiang on 2021/12/18.
//

#ifndef APP_COMMUNICATION_DEVICESTRATEGY_H
#define APP_COMMUNICATION_DEVICESTRATEGY_H

#include "MessageStrategy.h"
#include "net/WsServerManager.h"
#include <tool/Variable.h>
#include "manager/VersionManager.h"
#include "model/DeviceStatus.h"
#include "model/DeviceStatusV2.h"
#include "model/MapInfo.h"
#include <model/Task.h>
#include <model/WorkStatus.h>
#include "model/Location.h"


#include <nlohmann/json.hpp>

using json = nlohmann::json;

class GetDeviceStatusStrategy : public MessageStrategy<std::string, DeviceStatus> {
public:
    DeviceStatus handler(std::string method) override;
};

class GetDeviceStatusStrategyV2 : public MessageStrategy<std::string, DeviceStatusV2> {
public:
    DeviceStatusV2 handler(std::string params) override;
};

class ChangeWorkModeStrategy : public MessageStrategy<WorkStatus, std::string> {
public:
    std::string handler(WorkStatus params) override;
};

class ChangeAromStatusStrategy : public MessageStrategy<bool, std::string> {
public:
    std::string handler(bool params) override;
};

class GetHotWindModeStrategy : public MessageStrategy<std::string, int> {
public:
    int handler(std::string params) override;
};

class SetHotWindModeStrategy : public MessageStrategy<int, int> {
public:
    int handler(int params) override;
};

class AutomaticOilingStrategy : public MessageStringStrategy {
public:
    void handler() override;
};

class SetBaseStationStrategy : public MessageStrategy<bool, std::string> {
public:
    std::string handler(bool params) override;
};

class GetBaseStationStrategy : public MessageStrategy<std::string, bool> {
public:
    bool handler(std::string params) override;
};

class SetRainSnowStrategy : public MessageStrategy<bool, std::string> {
public:
    std::string handler(bool params) override;
};

class GetRainSnowStrategy : public MessageStrategy<std::string, bool> {
public:
    bool handler(std::string params) override;
};

class SetCollectDustStrategy : public MessageStrategy<bool, std::string> {
public:
    std::string handler(bool params) override;
};

class GetCollectDustStrategy : public MessageStrategy<std::string, bool> {
public:
    bool handler(std::string params) override;
};

class SetAutoOilStrategy : public MessageStrategy<bool, std::string> {
public:
    std::string handler(bool params) override;
};

class GetAutoOilStrategy : public MessageStrategy<std::string, bool> {
public:
    bool handler(std::string params) override;
};

class SetMaintenanceStartTimeStrategy : public MessageStrategy<long, std::string> {
public:
    std::string handler(long params) override;
};

class GetMaintenanceStartTimeStrategy : public MessageStrategy<std::string, long> {
public:
    long handler(std::string params) override;
};

#endif//APP_COMMUNICATION_DEVICESTRATEGY_H
