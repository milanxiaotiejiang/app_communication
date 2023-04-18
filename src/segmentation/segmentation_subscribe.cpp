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

    TaskVo taskVo(0, map.id, "划区清洁01", 1, 0, false,
                  false, false, "App", "", 0, 0, 0);

    std::vector<PointVo> points;
    points.emplace_back(1, 1);
    points.emplace_back(1, 100);
    points.emplace_back(100, 100);
    points.emplace_back(100, 1);
    ZoneVo zoneVo(0, points);

    std::vector<ZoneVo> zones;
    zones.push_back(zoneVo);
    taskVo.setZones(zones);

    TaskDataBase::instance().addTask(map.id, taskVo);

    try {

        const vector<TaskVo> &vector = TaskDataBase::instance().loadTaskFoMap(map.id);
        json jsonResult = vector;
        LOG(ERROR) << jsonResult.dump();
    } catch (app::exception const &e) {
        LOG(ERROR) << e.what();
    } catch (const std::exception &e) {
        LOG(ERROR) << e.what();
    } catch (...) {
        LOG(ERROR) << "MessageStrategy other start exception";
    }

}