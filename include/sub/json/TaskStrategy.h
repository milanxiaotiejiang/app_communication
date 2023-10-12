//
// Created by lijiang on 2021/12/18.
//

#ifndef APP_COMMUNICATION_TASKSTRATEGY_H
#define APP_COMMUNICATION_TASKSTRATEGY_H

#include "MessageStrategy.h"
#include <std_msgs/Int32.h>

#include "net/base/BaseResult.h"
#include "model/Point.h"
#include "string"
#include "tool/write_file.hpp"
#include "net/base/BaseMethod.h"

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
#include <tf/transform_datatypes.h>
#include <tf/transform_listener.h>
#include "simulation.h"
#include <chrono>
#include "model/UpgradeModel.h"
#include "task/model/PointProgressVo.h"
#include "model/task.h"

class PerformTaskStrategy : public MessageStrategy<OnTask, std::string> {
public:
    std::string handler(OnTask params) override;
};

class RunningTaskStrategy : public MessageStrategy<std::string, RunTask> {
public:
    RunTask handler(std::string params) override;
};

class IsInBasementStrategy : public MessageStrategy<std::string, bool> {
public:
    bool handler(std::string params) override;
};

class GetRosVersionStrategy : public MessageStrategy<std::string, VersionInfo> {
public:
    VersionInfo handler(std::string params) override;
};

class GetFinishedPointStrategy : public MessageStrategy<std::string, std::vector<PointProgressVo>> {
public:
    std::vector<PointProgressVo> handler(std::string params) override;
};

#endif// APP_COMMUNICATION_TASKSTRATEGY_H
