//
// Created by lijiang on 2021/12/18.
//

#ifndef APP_COMMUNICATION_TASKSTRATEGY_H
#define APP_COMMUNICATION_TASKSTRATEGY_H

#include "MessageStrategy.h"
#include <std_msgs/Int32.h>

#include "model/BaseResult.h"
#include "model/Partion.h"
#include "model/Point.h"
#include "model/Task.h"
#include "string"
#include "tool/write_file.hpp"
#include <model/BaseMethod.h>

#include <nlohmann/json.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>

#include "manager/VersionManager.h"
#include "model/VersionInfo.h"
#include "ros/package.h"
#include "tool/Variable.h"
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <prohibition.h>
#include <set_pose.hpp>
#include <tf/transform_datatypes.h>
#include <tf/transform_listener.h>
#include "glog/logging.h"
#include "tool/ThreadPool.h"
#include <chrono>
#include "model/UpgradeModel.h"
#include "task/model/PointProgressVo.h"

class ExecuteTaskStrategy : public MessageStrategy<Task, string> {
public:
    string handler(Task params) override;
};

class GetTaskListStrategy : public MessageStrategy<string, std::vector<Task>> {
public:
    vector<Task> handler(string params) override;
};

class GetTaskListStrategyV2 : public MessageStrategy<string, std::vector<TaskUpgrade>> {
public:
    vector<TaskUpgrade> handler(string params) override;
};

class IsInBasementStrategy : public MessageStrategy<string, bool> {
public:
    bool handler(string params) override;
};

class GetRosVersionStrategy : public MessageStrategy<string, VersionInfo> {
public:
    VersionInfo handler(string params) override;
};

class GetFinishedPointStrategy : public MessageStrategy<string, deque<PointProgressVo>> {
public:
    deque<PointProgressVo> handler(string params) override;
};

class GetFullPlanStrategy : public MessageBaseStrategy {
public:
    void dateProgressing(int source, json &jdecode) override;
};

#endif// APP_COMMUNICATION_TASKSTRATEGY_H
