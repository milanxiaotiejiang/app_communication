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
#include "segmentation/map_modification.h"
#include "leave/map_control.h"

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
    auto generateMat = SegmentationCenter::instance().generateMat();


//    vector<vector<PointVo>> params;
//    vector<PointVo> ps;
//    ps.emplace_back(0, 0);
//    ps.emplace_back(0, generateMat.rows / 2);
//    ps.emplace_back(generateMat.rows / 2, generateMat.rows / 2);
//    ps.emplace_back(generateMat.rows / 2, 0);
//    params.push_back(ps);
//
//    std::vector<std::vector<cv::Point>> points;
//
//    for (const auto &vector: params) {
//        std::vector<cv::Point> cvs;
//        for (const auto &pointVo: vector) {
//            cv::Point point(pointVo.getX(), pointVo.getY());
//            cvs.push_back(point);
//        }
//        points.push_back(cvs);
//    }
//
//    MapModification mapModification;
//    mapModification.addFeasibleZone(points);
//    MapControl::instance().backupMap(SegmentationDataBase::instance().getDbMap().id, false);
//    MapControl::instance().changeMapServer();

    const std::vector<MapPo> &allMap1 = SegmentationDataBase::instance().loadAllMap();

    std::string params = allMap1[flag].id;

    MapPo oldMap = SegmentationDataBase::instance().getDbMap();
    if (oldMap.id == params) {
        return;
    }

    const std::vector<MapPo> &allMap = SegmentationDataBase::instance().loadAllMap();
    bool isFind = false;
    for (const auto &item: allMap) {
        if (item.id == params) {
            isFind = true;
            break;
        }
    }
    if (isFind) {
        MapControl::instance().backupAndRetrieve(oldMap.id);

        MapControl::instance().loadInformation(params);
        MapControl::instance().changeMapServer();
        // todo 关注睡眠模式
        CartographerPublisher::instance().publishStartCartoLocalization();
    }



//    try {
//        TaskCenter::instance().performTask(flag, TaskSource::App);
//    } catch (app::exception const &e) {
//        LOG(ERROR) << e.what();
//    } catch (const std::exception &e) {
//        LOG(ERROR) << e.what();
//    } catch (...) {
//        LOG(ERROR) << "MessageStrategy other start exception";
//    }

}