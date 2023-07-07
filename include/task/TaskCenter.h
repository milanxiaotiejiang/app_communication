//
// Created by Looper on 2022/11/5.
//

#ifndef APP_COMMUNICATION_TASKCENTER_H
#define APP_COMMUNICATION_TASKCENTER_H

#include "simulation.h"
#include "model/Task.h"
#include "model/task.h"
#include "task/RealTask.h"
#include "task/subscribe/ZooRobotStatusSubscribe.h"
#include "task/subscribe/FlagResultSubscribe.h"
#include "task/call/reserved_call.h"
#include "task/subscribe/carpet_detect_subscribe.h"
#include "task/subscribe/lift_detect_subscribe.h"
#include "leave/cartographer_node.h"
#include "db/task_model.h"

const int LOW_RSOC = 10;

/**
 * 任务控制中心节点
 */
class TaskCenter {
private:
    TaskCenter() = default;

    TaskCenter(TaskCenter &) = delete;

    TaskCenter &operator=(const TaskCenter &) = delete;

public:
    ~TaskCenter() = default;

private:
    ros::NodeHandle nodeHandle;

    std::shared_ptr<ReservedCall> asyncTaskCall;

    ZooRobotStatusSubscribe *zooRobotStatusSubscribe;
    FlagOutSubscribe *flagOutSubscribe;
    FlagInSubscribe *flagInSubscribe;
    CarpetDetectSubscribe *carpetDetectSubscribe;
    LiftDetectSubscribe *liftDetectSubscribe;

    std::string preTask(const RealTask &task);

    std::string proTask(const RealTask &task);

    static std::string realTask(RealTask task);

public:
    static auto &instance() {
        static TaskCenter obj;
        return obj;
    }

    void initialize(ros::NodeHandle handle);

    void uninstall();

    void executeTask(const Task &task);

    std::string performTask(const long taskId, TaskSource on_source, int on_rate);

};

#endif //APP_COMMUNICATION_TASKCENTER_H
