//
// Created by Looper on 2023/4/17.
//

#include "segmentation/segmentation_subscribe.h"
#include "model/task.h"
#include "db/task_data_base.h"
#include "task/RealTask.h"
#include "exploration/path_exploration_preview_task.h"
#include "schedule/schedule_manager_singleton.h"
#include "BaseThrowable.h"
#include "segmentation/SegmentationCenter.h"
#include "db/segmentation_data_base.h"
#include "task/TaskCenter.h"

SegmentationSubscribe::SegmentationSubscribe(ros::NodeHandle handle) {
    sub_node_control_ = handle.subscribe("/segmentation_task", 1, &SegmentationSubscribe::segmentationSubscribeCallback,
                                         this);
}

void SegmentationSubscribe::segmentationSubscribeCallback(const std_msgs::Int32 &flag_result) {
    auto flag = flag_result.data;
//    const TaskVo &task = TaskDataBase::instance().loadTaskFoId(flag);
//    RealTask realTask;
//    TaskExploration::task2RealTask(task, realTask);
//    auto coverage = TaskExploration::explorationPlanningPath(realTask);
    MapPo map = SegmentationDataBase::instance().getDbMap();


    try {
        TaskCenter::instance().performTask(flag, TaskSource::App);
    } catch (app::exception const &e) {
        LOG(ERROR) << e.what();
    } catch (const std::exception &e) {
        LOG(ERROR) << e.what();
    } catch (...) {
        LOG(ERROR) << "MessageStrategy other start exception";
    }

}