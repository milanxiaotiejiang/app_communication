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
    AsyncTaskCall *asyncTaskCall;
    std::thread plan_transfer_thread;
    code_machina::BlockingCollection<RealTask> transferCollection;

    static std::shared_ptr<PointGenerator> pointGeneratorFactory(const RealTask &realTask) {
        if (realTask.isRenew()) {
            return std::make_shared<ExplorationGenerator>(ExplorationGenerator());
        } else {
//            if (realTask.getMode() == 10) {
//                return std::make_shared<CoveragePointGenerator>(CoveragePointGenerator());
//            } else if (realTask.getMode() == 2) {
//                return std::make_shared<RectanglePointGenerator>(RectanglePointGenerator());
//            } else if (realTask.getMode() == 7) {
//                return std::make_shared<CombinationPointGenerator>(CombinationPointGenerator());
//            } else if (realTask.getMode() == 6) {
//                return std::make_shared<FullPointGenerator>(FullPointGenerator());
//            }
        }
        throw app::exception(make_error_code(error::task_mode_no_find));
    }

    void plan_transfer_thread_func();

public:
    TaskDispatcher();

    static auto &instance() {
        static TaskDispatcher obj;
        return obj;
    }

    void setAsyncTaskCall(AsyncTaskCall *asyncTaskCall) {
        TaskDispatcher::asyncTaskCall = asyncTaskCall;
        plan_transfer_thread.detach();
    }

    void dispatcherTask(RealTask &realTask);

};


#endif //APP_COMMUNICATION_TASK_DISPATCHER_H
