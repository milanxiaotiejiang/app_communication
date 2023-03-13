//
// Created by Looper on 2023/3/3.
//

#include "task/feedback.h"
#include "segmentation/SegmentationCenter.h"
#include "segmentation/map_attribute.h"
#include "db/segmentation_data_base.h"
#include "simulation.h"

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

TaskFeedback::TaskFeedback() {
    TaskFeedback::make_thread(run, this);
}

void TaskFeedback::execute() {
    for (;;) {
        std::unique_lock<std::mutex> lock(cv_mut);
        cv.wait(lock, [this] {
            return !orderDeque.empty() || !dataDeque.empty();
        });

        if (!orderDeque.empty()) {
            if (orderDeque.back() == 0) {//end
                end();
            } else if (orderDeque.back() == 1) {//start
                start();
            }
            orderDeque.clear();
        } else if (!dataDeque.empty()) {
            geometry_msgs::Pose2D data = dataDeque.back();
            dataDeque.clear();
            feedback(data);
        }

    }
}

void TaskFeedback::start() {
    points.clear();

    map_origin = MapAttribute::instance().getMapOrigin();

    const cv::Mat room_map = SegmentationCenter::instance().generateMat();
    rows = room_map.rows;
    cols = room_map.cols;

    savePath = path::data_base_config_dir() + run_task_id + ".pgm";

    auto plan = SegmentationDataBase::instance().getDbPlan(SegmentationDataBase::instance().getDbMap().id);
    double grid_spacing_in_meter = plan.robot_radius * std::sqrt(2);//网格正方形的边长
    double grid_spacing_in_pixel = grid_spacing_in_meter / map_resolution_from_subscription;
    LOG(INFO) << "grid size: " << grid_spacing_in_meter << " m   (" << grid_spacing_in_pixel << " px)";
    spacing_half = (int) std::floor(0.5 * grid_spacing_in_pixel);

    auto map = room_map.clone();
    area_px = 0;
    for (int v = 0; v < map.rows; ++v) {
        for (int u = 0; u < map.cols; ++u) {
            if (map.at<uchar>(v, u) >= 250)
                area_px++;
        }
    }

    geometry_msgs::Pose2D robot_position = MapAttribute::instance().getRobotPositionPose();
    const cv::Point &start_point = poseTransferPoint(robot_position.x, robot_position.y);
    cv::Point p0 = poseTransferPoint(planPoseVos[0].getX(), planPoseVos[0].getY());
    cv::line(map, start_point, p0, cv::Scalar(100), spacing_half * 2);
    for (int i = 1; i < planPoseVos.size(); ++i) {
        cv::Point ps = poseTransferPoint(planPoseVos[i - 1].getX(), planPoseVos[i - 1].getY());
        cv::Point pe = poseTransferPoint(planPoseVos[i].getX(), planPoseVos[i].getY());
        cv::line(map, ps, pe, cv::Scalar(100), spacing_half * 2);
    }
    cv::Point pl = poseTransferPoint(planPoseVos[planPoseVos.size() - 1].getX(),
                                     planPoseVos[planPoseVos.size() - 1].getY());
    cv::line(map, pl, start_point, cv::Scalar(100), spacing_half * 2);
    plan_px = 0;
    for (int v = 0; v < map.rows; ++v) {
        for (int u = 0; u < map.cols; ++u) {
            if (map.at<uchar>(v, u) == 100)
                plan_px++;
        }
    }

}

void TaskFeedback::end() {
    if (!points.empty()) {
        planPoseVos.clear();
        points.clear();
    }
}

void TaskFeedback::feedback(geometry_msgs::Pose2D data) {

    cv::Point point = poseTransferPoint(data.x, data.y);

    auto pair = points.insert(PointVo(point.x, point.y));
    if (pair.second) {
        auto task_mat = SegmentationCenter::instance().generateMat().clone();

        for (const auto &item: points) {
            cv::circle(task_mat, cv::Point(item.getX(), item.getY()), spacing_half, cv::Scalar(200), CV_FILLED);
        }

        int clear_px = 0;
        for (int v = 0; v < task_mat.rows; ++v) {
            for (int u = 0; u < task_mat.cols; ++u) {
                if (task_mat.at<uchar>(v, u) == 200)
                    clear_px++;
            }
        }

        if (Environment::instance().isRealEnvironment) {
            LOG(INFO) << "### area_px : " << area_px << " , plan_px : " << plan_px << "  "
                      << run_task_id << " 真实面积/总面积 = " << (clear_px * 1.0 / area_px)
                      << " , 真实面积/规划面积 = " << (clear_px * 1.0 / plan_px);
        }

        CvUtils::savePgm(savePath, task_mat.clone());
    }
}

cv::Point TaskFeedback::poseTransferPoint(float x, float y) {
    cv::Point point(cols - (y - map_origin.x) / map_resolution_from_subscription,
                    rows - (x - map_origin.y) / map_resolution_from_subscription);
    return point;
}

void TaskFeedback::triggerStart(std::string taskId, const std::vector<RealPoint> &points) {
    {
        std::unique_lock<std::mutex> lock(cv_mut);
        TaskFeedback::run_task_id = taskId;
        planPoseVos.clear();
        for (const auto &point: points) {
            planPoseVos.emplace_back(point.realPosition.x, point.realPosition.y, .0);
        }
        orderDeque.push_back(1);
    }
    cv.notify_one();
}

void TaskFeedback::triggerEnd() {
    {
        std::unique_lock<std::mutex> lock(cv_mut);
        orderDeque.push_back(2);
    }
    cv.notify_one();
}

void TaskFeedback::triggerFeedback(geometry_msgs::Pose2D data) {
    {
        std::unique_lock<std::mutex> lock(cv_mut);
        dataDeque.push_back(data);
    }
    cv.notify_one();
}
