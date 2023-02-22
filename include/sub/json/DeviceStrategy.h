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
using namespace std;

class GetDeviceStatusStrategy : public MessageStrategy<string, DeviceStatus> {
public:
    DeviceStatus handler(string method) override;
};

class GetDeviceStatusStrategyV2 : public MessageStrategy<string, DeviceStatusV2> {
public:
    DeviceStatusV2 handler(string params) override;
};

class ChangeWorkModeStrategy : public MessageStrategy<WorkStatus, string> {
public:
    string handler(WorkStatus params) override;
};

class ChangeAromStatusStrategy : public MessageStrategy<bool, string> {
public:
    string handler(bool params) override;
};

class SelfCleanStrategy : public MessageStrategy<string, string> {
public:
    string handler(string params) override;
};

class OpenMachineDrawerStrategy : public MessageStrategy<string, string> {
public:
    string handler(string params) override;
};

class LightBeltModeStrategy : public MessageStrategy<int, string> {
public:
    string handler(int params) override;
};

class PlayerRecruitVoiceStrategy : public MessageStrategy<int, string> {
public:
    string handler(int params) override;
};

class GetHotWindModeStrategy : public MessageStrategy<string, int> {
public:
    int handler(string params) override;
};

class SetHotWindModeStrategy : public MessageStrategy<int, int> {
public:
    int handler(int params) override;
};


#endif//APP_COMMUNICATION_DEVICESTRATEGY_H
