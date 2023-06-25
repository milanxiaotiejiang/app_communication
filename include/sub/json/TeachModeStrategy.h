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

class StartTeachModeStrategy : public MessageStrategy<std::string, std::string> {
public:
    std::string handler(std::string params) override;
};

class StopTeachModeStrategy : public MessageStrategy<ClossTeachMode, std::string> {
public:
    std::string handler(ClossTeachMode params) override;
};

class HeartBeatofTeachModeStrategy : public MessageStrategy<std::string, std::string> {
public:
    std::string handler(std::string params) override;
};

class GetTeachModeListStrategy : public MessageStrategy<std::string, std::vector<ResponseGetTeachTask>> {
public:
    std::vector<ResponseGetTeachTask> handler(std::string params) override;
};

class GetTeachModeDetialStrategy : public MessageStrategy<std::string, std::vector<Point>> {
public:
    std::vector<Point> handler(std::string params) override;
};

class DeleteTeachModeStrategy : public MessageStrategy<std::vector<std::string>, std::string> {
public:
    std::string handler(std::vector<std::string> params) override;

    bool is_exist(const std::string &str, std::vector<std::string> str_list);
};

#endif //APP_COMMUNICATION_TEACHMODESTRATEGY_H
