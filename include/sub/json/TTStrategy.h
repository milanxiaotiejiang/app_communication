//
// Created by lijiang on 2021/12/18.
//

#ifndef APP_COMMUNICATION_TTSTRATEGY_H
#define APP_COMMUNICATION_TTSTRATEGY_H

#include "MessageStrategy.h"
#include <geometry_msgs/Twist.h>
#include "net/base/BaseMethod.h"
#include "net/base/BaseResult.h"
#include <manager/NoticeManager.h>
#include "Python.h"
#include <fstream>

using json = nlohmann::json;
using namespace std;

class AppAlongCleanStrategy : public MessageStrategy<std::vector<float>, string> {
public:
    string handler(vector<float> params) override;
};

class PowerReductionStrategy : public MessageStrategy<string, string> {
public:
    string handler(string params) override;
};

class TTErrorCheck : public MessageStrategy<string, string> {
public:
    string handler(string params) override;
};


#endif //APP_COMMUNICATION_TTSTRATEGY_H
