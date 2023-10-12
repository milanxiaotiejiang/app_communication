//
// Created by Looper on 2022/11/5.
//

#include "task/task_dispatcher.h"
#include "clean_history/CleanHistoryCenter.h"
#include "exploration/path_exploration_preview_task.h"

TaskDispatcher::TaskDispatcher() {
    plan_transfer_thread = std::thread(&TaskDispatcher::plan_transfer_thread_func, this);
}

void TaskDispatcher::dispatcherTask(RealTask &realTask) {

    TaskMode mode = SqliteDataBase::TaskModeFromInt(realTask.getMode());
    if (mode == TaskMode::Zoned) {
        realTask.setVerifyMode(true);
    }

    auto coverage = TaskExploration::explorationPlanningPath(realTask);

    if (coverage.getPoseList().empty()) {
        throw app::exception(make_error_code(error::exploration_path_planning_failed));
    }

    LOG_IF(INFO, DEBUG_TASK)
                    << " 验证覆盖点位个数为 : " << coverage.getPoseList().size() << " , 此任务验证可以通过 ... ";

//    asyncTaskCall->executeOneTask(realTask);

    realTask.setVerifyMode(false);
    transferCollection.add(realTask);
}

void TaskDispatcher::plan_transfer_thread_func() {
    while (!transferCollection.is_completed()) {
        RealTask realTask;
        auto status = transferCollection.take(realTask);
        if (status == code_machina::BlockingCollectionStatus::Ok) {
            try {
                //生成对应该任务的点列
                const auto pointGeneratorPtr = pointGeneratorFactory(realTask);
                const auto pointList = pointGeneratorPtr->taskGeneratePointList(realTask);
                if (pointList.empty()) {
                    LOG(ERROR) << " 牛耕田法规划的点位个数为空 ... ";
                    return;
                }
                //点列赋值给realTask
                realTask.setPlanPoints(pointList);
                //清洁记录更新
                clean_history_db::CleanHistoryCenter::instance().upDateByRealTask(realTask);
                //开始执行realTask
                asyncTaskCall->executeOneTask(realTask);
            } catch (app::exception const &e) {
                LOG(ERROR) << e.what();
            } catch (const std::exception &e) {
                LOG(ERROR) << e.what();
            } catch (...) {
                LOG(ERROR) << "MessageStrategy other start exception";
            }
        }
    }
}
