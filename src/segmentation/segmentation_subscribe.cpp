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
#include "task/manager/manual.h"
#include "leave/HotWindNote.h"
#include "leave/auto_maintenance_mode.h"
#include "leave/ParamManager.h"
#include "leave/sensor/sensor_center.h"

SegmentationSubscribe::SegmentationSubscribe(ros::NodeHandle handle) {
    sub_node_control_ = handle.subscribe("/segmentation_task", 1, &SegmentationSubscribe::segmentationSubscribeCallback,
                                         this);
    sub_order_control_ = handle.subscribe("/segmentation_order", 1,
                                          &SegmentationSubscribe::segmentationOrderSubscribeCallback,
                                          this);
    sub_test_control_ = handle.subscribe("/segmentation_test", 1,
                                         &SegmentationSubscribe::segmentationTestSubscribeCallback,
                                         this);
}

void SegmentationSubscribe::segmentationSubscribeCallback(const std_msgs::Int32 &flag_result) {
    auto flag = flag_result.data;

//    MapPo map = SegmentationDataBase::instance().getDbMap();
//    auto generateMat = SegmentationCenter::instance().generateMat();

    try {
        TaskCenter::instance().performTask(flag, TaskSource::Cloud, 1);

    } catch (app::exception const &e) {
        LOG(ERROR) << e.what();
    } catch (const std::exception &e) {
        LOG(ERROR) << e.what();
    } catch (...) {
        LOG(ERROR) << "MessageStrategy other start exception";
    }

}

void SegmentationSubscribe::segmentationOrderSubscribeCallback(const std_msgs::Int32 &flag_result) {
    auto flag = flag_result.data;
    try {
        if (flag == 0) {
            ManualManager::instance().pause();
        } else if (flag == 1) {
            ManualManager::instance().resume();
        } else if (flag == 2) {
            ZooInnerStatus::instance().setUrgencyStopStatus(true);
        } else if (flag == 3) {
            ZooInnerStatus::instance().setUrgencyStopStatus(false);
        } else if (flag == 100) {
            ManualManager::instance().backToBase(false);
        } else if (flag == 1010) {
            ZooInnerStatus::instance().setIsCharging(true);
        } else if (flag == 1011) {
            ZooInnerStatus::instance().setIsCharging(false);
        }
    } catch (app::exception const &e) {
        LOG(ERROR) << e.what();
    } catch (const std::exception &e) {
        LOG(ERROR) << e.what();
    } catch (...) {
        LOG(ERROR) << "MessageStrategy other start exception";
    }
}

void SegmentationSubscribe::segmentationTestSubscribeCallback(const std_msgs::Int32 &flag_result) {
    auto flag = flag_result.data;
    ParamManager::instance().setMaintenanceStartTime(flag);
    AutoMaintenanceModeManager::instance().reset();
}
