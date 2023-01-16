//
// Created by lijiang on 2022/1/10.
//

#ifndef APP_COMMUNICATION_TEACHMODESTRATEGY_H
#define APP_COMMUNICATION_TEACHMODESTRATEGY_H

#include "MessageStrategy.h"
#include "manager/CombinationManager.h"
#include "manager/ViewPartManager.h"
#include "model/Point.h"
#include "model/TeachModePoint.h"
#include "tool/Variable.h"
#include "tool/write_file.hpp"
#include <thread>
#include "model/TeachModePoint.h"

geometry_msgs::Pose get_current_pose();

class StartTeachModeStrategy : public MessageStrategy<string, string> {
public:
    string handler(string params) override;
};

class StopTeachModeStrategy : public MessageStrategy<ClossTeachMode, string> {
public:
    string handler(ClossTeachMode params) override;
};

class HeartBeatofTeachModeStrategy : public MessageStrategy<string, string> {
public:
    string handler(string params) override;
};

class GetTeachModeListStrategy : public MessageStrategy<string, std::vector<ResponseGetTeachTask>> {
public:
    vector<ResponseGetTeachTask> handler(string params) override;
};

class GetTeachModeDetialStrategy : public MessageStrategy<string, std::vector<Point>> {
public:
    vector<Point> handler(string params) override;
};

class DeleteTeachModeStrategy : public MessageStrategy<vector<string>, string> {
public:
    string handler(vector<string> params) override;

    bool is_exist(const string &str, vector<string> str_list);
};

#endif //APP_COMMUNICATION_TEACHMODESTRATEGY_H
