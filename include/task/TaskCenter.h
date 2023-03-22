//
// Created by Looper on 2022/11/5.
//

#ifndef APP_COMMUNICATION_TASKCENTER_H
#define APP_COMMUNICATION_TASKCENTER_H

#include "glog/logging.h"
#include "model/Task.h"
#include "task/RealTask.h"
#include "task/subscribe/ZooRobotStatusSubscribe.h"
#include "task/subscribe/FlagResultSubscribe.h"
#include "task/call/reserved_call.h"
#include "task/subscribe/CartographerSubscribe.h"
#include "task/subscribe/carpet_detect_subscribe.h"
#include "task/subscribe/lift_detect_subscribe.h"

const int LOW_RSOC = 10;

/**
 * 任务控制中心节点
 */
class TaskCenter {
private:
    ros::NodeHandle nodeHandle;

    ReservedCall *asyncTaskCall = nullptr;

    ZooRobotStatusSubscribe *zooRobotStatusSubscribe;
    FlagOutSubscribe *flagOutSubscribe;
    FlagInSubscribe *flagInSubscribe;
    CartographerSubscribe *cartographerSubscribe;
    CarpetDetectSubscribe *carpetDetectSubscribe;
    LiftDetectSubscribe *liftDetectSubscribe;

    void task2RealTask(const Task &task, RealTask &realTask);

    void realExecuteTask(const Task &task);

public:
    static auto &instance() {
        static TaskCenter obj;
        return obj;
    }

    void initialize(ros::NodeHandle handle);

    void uninstall();

    void executeTask(const Task &task);

};

#endif //APP_COMMUNICATION_TASKCENTER_H
