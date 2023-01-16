//
// Created by lijiang on 2021/12/20.
//

#ifndef APP_COMMUNICATION_STATUSSTRATEGY_H
#define APP_COMMUNICATION_STATUSSTRATEGY_H

#include "MessageStrategy.h"
#include "manager/CleanHistoryManager.h"
#include "ros/ros.h"
#include <nlohmann/json.hpp>


using json = nlohmann::json;
using namespace std;

class StatusPauseStrategy : public MessageStringStrategy  {
public:
    void handler() override;
};

class StatusResumeStrategy : public MessageStringStrategy  {
public:
    void handler() override;
};

class StatusChargeStrategy : public MessageStringStrategy  {
public:
    void handler() override;
};

class EnterManualStrategy : public MessageStringStrategy {
public:
    void handler() override;
};

class QuitManualStrategy : public MessageStringStrategy {
public:
    void handler() override;
};

class ShutDownStrategy : public MessageStringStrategy {
public:
    void handler() override;
};

class RebootStrategy : public MessageStringStrategy{
public:
    void handler() override;
};

#endif//APP_COMMUNICATION_STATUSSTRATEGY_H
