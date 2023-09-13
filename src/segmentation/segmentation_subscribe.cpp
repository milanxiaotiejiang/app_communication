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
            ManualManager::instance().backToBase(true);
        } else if (flag == 1010) {
            ZooInnerStatus::instance().setIsCharging(true);
        } else if (flag == 1011) {
            ZooInnerStatus::instance().setIsCharging(false);
        } else if (flag == 10000) {
            ManualManager::instance().quit_manual_mode();
        } else if (flag == 10001) {
            ManualManager::instance().enter_manual_mode();
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
//    auto flag = flag_result.data;
//    if (flag == 0) {
//        volatile int *a = (int *) (NULL);
//        *a = 1;
//    } else if (flag == 1) {
//        throw app::exception(make_error_code(error::the_current_state_is_uncontrollable));
//    }
    const cv::Mat &map = SegmentationCenter::instance().generateMat();
//    bool isOffMap = false;
//    bool isRestrictedZone = false;
//    bool isMaxPassable = false;
//    bool isPlanPath = false;
//    SegmentationCenter::instance()
//            .isRestrictedZone(map, isOffMap, isRestrictedZone, isMaxPassable, isPlanPath, true);
//    LOG(INFO) << "  isOffMap : " << isOffMap
//              << "  isRestrictedZone : " << isRestrictedZone
//              << "  isMaxPassable : " << isMaxPassable
//              << "  isPlanPath : " << isPlanPath;
    if (flag_result.data == 1) {
        int height = 250;
        auto pl = MapAttributeSingleton::instance().mapPoint2RosPoint(map.rows, map.cols, cv::Point(180, height - 20));
        auto pr = MapAttributeSingleton::instance().mapPoint2RosPoint(map.rows, map.cols, cv::Point(180, height + 20));

        MapPo &po = SegmentationDataBase::instance().getDbMap();
        Gate gate(po.id, 15, height, 280, height,
                  pl.getX(), pl.getY(), 0, 0, 0, 0, 0,
                  pr.getX(), pr.getY(), 0, 0, 0, 0, 0);
        SegmentationDataBase::instance().saveGate(gate);
    } else if (flag_result.data == 2) {
        MapPo &po = SegmentationDataBase::instance().getDbMap();
        Gate gate(po.id, 123, 231, 292, 231,
                  -0.0480371669563, 3.03767555864, -0.0023247943396,
                  -0.0040943493407, -0.00849210578278, -0.0298977331954, 0.999508502211,
                  2.23944492753, 2.95870282466, 0.00108490549205,
                  -0.00406858102555, -0.00276490081865, 0.99982629203, 0.0179774229821);
        SegmentationDataBase::instance().saveGate(gate);
    }

}
