//
// Created by lijiang on 2021/12/18.
//

#include "task/TaskCenter.h"
#include "task/manager/manual.h"
#include "task/model/PointProgressVo.h"
#include "sub/json/TaskStrategy.h"

#include "exploration/ExplorationStrategy.h"
#include "segmentation/SegmentationCenter.h"
#include "exploration/ExplorationCenter.h"
#include "db/segmentation_data_base.h"
#include "simulation.h"
#include "db/SqliteDataBase.h"
#include "tool/param_check.h"

std::string PerformTaskStrategy::handler(OnTask params) {
    LOG(ERROR) << params;
    checkRate(params.on_rate);
    return TaskCenter::instance().performTask(params.task_id,
                                              SqliteDataBase::TaskSourceFromString(params.on_source),
                                              params.on_rate
    );
}

RunTask RunningTaskStrategy::handler(std::string params) {
    RunTask runTask("");
    const RealTask &runningTask = ManualManager::instance().runningTask();
    if (!runningTask.getId().empty()) {
        runTask.taskId = runningTask.getId();
        runTask.renew = true;
        runTask.newTaskId = runningTask.getTaskId();
    } else {
        throw app::exception(make_error_code(error::no_run_task));
    }
    return runTask;
}

bool IsInBasementStrategy::handler(std::string params) {
    return true;
}

VersionInfo GetRosVersionStrategy::handler(std::string params) {
    VersionManager::instance().getAirCodeVersion();
    VersionInfo into(
            VersionManager::instance().getAirCodeVersion(),
            VersionManager::instance().getDsHardVersion(),
            VersionManager::instance().getDsSoftVersion(),
            VersionManager::instance().getAppPadVersion()
    );
    return into;
}

std::vector<PointProgressVo> GetFinishedPointStrategy::handler(std::string params) {
    return ManualManager::instance().runTaskPointList();
}

std::string RobotMoveStrategy::handler(PoseVo params) {
    geometry_msgs::PoseStamped poseStamped;
    poseStamped.header.frame_id = "map";
    poseStamped.pose.position.x = params.getX();
    poseStamped.pose.position.y = params.getY();
    poseStamped.pose.position.z = 0;
    poseStamped.pose.orientation = tf::createQuaternionMsgFromYaw(params.getTheta());
    PublishInnerManager::instance().pubNavGoal(poseStamped);
    return "";
}

