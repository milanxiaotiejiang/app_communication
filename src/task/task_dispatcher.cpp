//
// Created by Looper on 2022/11/5.
//

#include "task/task_dispatcher.h"
#include "manager/CombinationManager.h"
#include "manager/ViewPartManager.h"
#include "manager/FullPlanWorkStatusManager.h"
#include "clean_history/CleanHistoryCenter.h"
#include "model/FullClean.h"
#include "manager/FullCleanManager.h"
#include "exploration/path_exploration_preview_task.h"

TaskDispatcher::TaskDispatcher() {
    plan_transfer_thread = std::thread(&TaskDispatcher::plan_transfer_thread_func, this);
}

void TaskDispatcher::dispatcherTask(RealTask &realTask) {

    if (!realTask.isRenew()) {
        WorkStatus workStatus;
        //模式2和10，默认尘推模式
        if (realTask.getMode() == 10) {
            workStatus = WorkStatus(0, 0, 0, 1, 0, 0);
        } else if (realTask.getMode() == 2) {
            workStatus = WorkStatus(0, 0, 0, 1, 0, 0);
        } else if (realTask.getMode() == 7) {
            //combination组合任务，从Combination信息中获取清洁模式信息
            //首先获取combination_id
            std::string combination_id_temp = realTask.getCombination().getCombinationID();
            //从combinationManager中获取对应的combination_brief
            CombinationBrief combination_brief_temp;
            if (CombinationManager::get_instance()->GetCombination(combination_brief_temp, combination_id_temp) !=
                SUCCESS_) {
                throw app::exception(make_error_code(error::combination_brief_get_fail));
            }
            //获取此combination_brief的workStatus
            workStatus = combination_brief_temp.getWorkStatus();
        } else if (realTask.getMode() == 6) {
            //全覆盖的直接获取，2212A to do
            //首先获取combination_id
            std::string combination_id_temp = realTask.getCombination().getCombinationID();
            //从fullCleanManager中获取相对应的full_clean_brief
            CombinationBrief combination_brief_temp;
            if (CombinationManager::get_instance()->GetCombination(combination_brief_temp, combination_id_temp) !=
                SUCCESS_) {
                throw app::exception(make_error_code(error::combination_brief_get_fail));
            }
            //获取此combination_brief的workStatus
            workStatus = combination_brief_temp.getWorkStatus();

            //before 2212A
            //workStatus = FullPlanWorkStatusManager::instance().getFullCleanWorkStatus();
        }
        //realTask赋值workStatus
        realTask.setWorkStatus(workStatus);
    }

    if (!realTask.isRenew()) {
        //如果任务是湿拖任务，清水箱已空或者污水箱已满，不能分发任务
        if (realTask.getWorkStatus().getMopStatus() == 1) {
            if (ZooInnerStatus::instance().getCleanWaterLevel() == 0) {
                throw app::exception(make_error_code(error::clean_water_level_check_failed));
            }
            if (ZooInnerStatus::instance().getDirtyWaterLevel() == 100) {
                throw app::exception(make_error_code(error::dirty_water_level_check_failed));
            }
        }
        if (realTask.getWorkStatus().getVacuumStatus() == 1) {
            if (ZooInnerStatus::instance().getDirtyWaterLevel() == 100) {
                throw app::exception(make_error_code(error::dirty_water_level_check_failed));
            }
        }
    }

    if (realTask.isRenew()) {
        TaskMode mode = SqliteDataBase::TaskModeFromInt(realTask.getMode());
        if (mode == TaskMode::Zoned) {
            realTask.setVerifyMode(true);
        }
    }

    auto coverage = TaskExploration::explorationPlanningPath(realTask);

    if (coverage.getPoseList().empty()) {
        throw app::exception(make_error_code(error::exploration_path_planning_failed));
    }

    LOG(INFO) << " 验证覆盖点位个数为 : " << coverage.getPoseList().size() << " , 此任务验证可以通过 ... ";
    realTask.setVerifyMode(false);
    transferCollection.add(realTask);
}

void TaskDispatcher::plan_transfer_thread_func() {
    while (!transferCollection.is_completed()) {
        RealTask realTask;
        auto status = transferCollection.take(realTask);
        if (status == code_machina::BlockingCollectionStatus::Ok) {

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

        }
    }
}
