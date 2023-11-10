//
// Created by Looper on 2022/11/5.
//

#ifndef APP_COMMUNICATION_TASK_DISPATCHER_H
#define APP_COMMUNICATION_TASK_DISPATCHER_H

#include "task/RealTask.h"
#include "task/point_generator.h"
#include "BaseThrowable.h"
#include "task/async_task_call.h"
#include "db/SqliteDataBase.h"
#include "future/BlockingCollection.h"

/**
 * 任务分发
 */
class TaskDispatcher {
private:
    TaskDispatcher();

    TaskDispatcher(TaskDispatcher &) = delete;

    TaskDispatcher &operator=(const TaskDispatcher &) = delete;

public:
    ~TaskDispatcher() = default;

private:
    std::shared_ptr<AsyncTaskCall> asyncTaskCall;
    std::thread plan_transfer_thread;
    code_machina::BlockingCollection<RealTask> transferCollection;

    static std::shared_ptr<PointGenerator> pointGeneratorFactory(const RealTask &realTask) {
        return std::make_shared<ExplorationGenerator>(ExplorationGenerator());
    }

    void plan_transfer_thread_func();

public:
    static auto &instance() {
        static TaskDispatcher obj;
        return obj;
    }

    void setAsyncTaskCall(std::shared_ptr<AsyncTaskCall> asyncTaskCallPtr) {
        TaskDispatcher::asyncTaskCall = asyncTaskCallPtr;
        plan_transfer_thread.detach();
    }

    void dispatcherTask(RealTask &realTask);

};


#endif //APP_COMMUNICATION_TASK_DISPATCHER_H
