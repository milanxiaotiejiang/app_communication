//
// Created by Looper on 2022/11/5.
//

#include "task/point_generator.h"

#include "exploration/ExplorationCenter.h"
#include "BaseThrowable.h"
#include "segmentation/map_attribute.h"
#include <Eigen/Dense>
#include <eigen_conversions/eigen_msg.h>
#include "exploration/radian_angle_conversion.h"
#include "model/ViewPart.h"
#include "manager/ViewPartManager.h"
#include "model/Combination.h"
#include "manager/CombinationManager.h"
#include "exploration/line.h"
#include "tool/Variable.h"
#include "segmentation/SegmentationCenter.h"
#include "task/model/CombinationPoseVo.h"
#include "exploration/path_exploration_preview_task.h"

RealPoint PointGenerator::buildPoint(int id, const RealTask &task) {
    RealPoint point;
    point.id = id;
    point.taskId = task.getId();

    point.renew = task.isRenew();
    if (task.isRenew()) {
        point.newTaskId = task.getTaskId();
    } else {
        if (task.getMode() == 7) {
            point.oldTaskId = task.getCombination().getCombinationID();
        }
    }

    point.name = task.getName();
    point.rate = task.getRate();
    point.mode = task.getMode();

    point.knife = task.isKnife();
    point.work_status = task.getWorkStatus();
    return point;
}

void PointGenerator::pose2RealPoint(RealTask &realTask, std::vector<PoseVo> poseList,
                                    std::vector<RealPoint> &realPointList) {
    auto originPose = MapAttribute::instance().getMapOriginPose();

    for (size_t point_index = 0; point_index < poseList.size(); ++point_index) {
        double theta = 0.;

        auto current_point = cv::Point2f(poseList[point_index].getX(), poseList[point_index].getY());

        cv::Point2f vector(0, 0);
        if (point_index < poseList.size() - 1) {
            auto next_point = cv::Point2f(poseList[point_index + 1].getX(), poseList[point_index + 1].getY());
            vector = next_point - current_point;
            if (vector.x != 0 || vector.y != 0) {
                theta = std::atan2(vector.y, vector.x);
            }
        } else {
            auto next_point = cv::Point2f(originPose.position.x, originPose.position.y);
            vector = next_point - current_point;
            if (vector.x != 0 || vector.y != 0) {
                theta = std::atan2(vector.y, vector.x);
            }
        }
        poseList[point_index].setTheta(theta);
    }


    std::vector<geometry_msgs::PoseStamped> path_pose_stamped(poseList.size());
    std_msgs::Header header;
    header.stamp = ros::Time::now();
    header.frame_id = "/map";
    for (size_t i = 0; i < poseList.size(); ++i) {
        PoseVo &vo = poseList[i];
        path_pose_stamped[i].header = header;
        path_pose_stamped[i].header.seq = i;
        path_pose_stamped[i].pose.position.x = vo.getX();
        path_pose_stamped[i].pose.position.y = vo.getY();
        path_pose_stamped[i].pose.position.z = 0.;
        Eigen::Quaterniond quaternion;
        quaternion = Eigen::AngleAxisd(vo.getTheta(), Eigen::Vector3d::UnitZ());
        tf::quaternionEigenToMsg(quaternion, path_pose_stamped[i].pose.orientation);
    }


    int rate = realTask.getRate();
    int totalStep = path_pose_stamped.size() * rate;
    int accumulation = 0;

    geometry_msgs::Pose::_position_type lastPose;
    for (int i = 0; i < rate; ++i) {
        for (int j = 0; j < path_pose_stamped.size(); ++j) {
            auto pose = path_pose_stamped[j];

            RealPosition realPosition(pose.pose.position.x, pose.pose.position.y, pose.pose.position.z);
            RealOrientation realOrientation(pose.pose.orientation.x, pose.pose.orientation.y,
                                            pose.pose.orientation.z, pose.pose.orientation.w);
            RealProgress realProgress(j + 1, totalStep, i + 1, rate);

            auto realPoint = buildPoint(accumulation, realTask);

            realPoint.realPosition = std::move(realPosition);
            realPoint.realOrientation = std::move(realOrientation);
            realPoint.realProgress = std::move(realProgress);

            realPoint.timeout = conversion::cal_distance(lastPose, pose.pose.position) * 20 + 5;//掉头5s

            realPoint.inClean = j != 0;

            realPointList.push_back(realPoint);

            accumulation++;

            lastPose.x = pose.pose.position.x;
            lastPose.y = pose.pose.position.y;
            lastPose.z = pose.pose.position.z;
        }
    }

    realTask.setTotalStep(totalStep);
    realTask.setTotalFrequency(rate);
}

void PointGenerator::combinationPose2RealPoint(RealTask realTask, std::vector<CombinationPoseVo> poseList,
                                               vector<RealPoint> &realPointList) {
    auto originPose = MapAttribute::instance().getMapOriginPose();

    for (size_t point_index = 0; point_index < poseList.size(); ++point_index) {
        double theta = 0.;

        auto current_point = cv::Point2f(poseList[point_index].getX(), poseList[point_index].getY());

        cv::Point2f vector(0, 0);
        if (point_index < poseList.size() - 1) {
            auto next_point = cv::Point2f(poseList[point_index + 1].getX(), poseList[point_index + 1].getY());
            vector = next_point - current_point;
            if (vector.x != 0 || vector.y != 0) {
                theta = std::atan2(vector.y, vector.x);
            }
        } else {
            auto next_point = cv::Point2f(originPose.position.x, originPose.position.y);
            vector = next_point - current_point;
            if (vector.x != 0 || vector.y != 0) {
                theta = std::atan2(vector.y, vector.x);
            }
        }
        poseList[point_index].setTheta(theta);
    }


    std_msgs::Header header;
    header.stamp = ros::Time::now();
    header.frame_id = "/map";

    int rate = realTask.getRate();
    int totalStep = poseList.size() * rate;
    int accumulation = 0;

    geometry_msgs::Pose::_position_type lastPose;
    for (int i = 0; i < rate; ++i) {
        for (int j = 0; j < poseList.size(); ++j) {

            CombinationPoseVo &vo = poseList[j];

            geometry_msgs::PoseStamped pose;
            pose.header = header;
            pose.header.seq = i;
            pose.pose.position.x = vo.getX();
            pose.pose.position.y = vo.getY();
            pose.pose.position.z = 0.;
            Eigen::Quaterniond quaternion;
            quaternion = Eigen::AngleAxisd(vo.getTheta(), Eigen::Vector3d::UnitZ());
            tf::quaternionEigenToMsg(quaternion, pose.pose.orientation);


            RealPosition realPosition(pose.pose.position.x, pose.pose.position.y, pose.pose.position.z);
            RealOrientation realOrientation(pose.pose.orientation.x, pose.pose.orientation.y,
                                            pose.pose.orientation.z, pose.pose.orientation.w);
            RealProgress realProgress(j + 1, totalStep, i + 1, rate);

            auto realPoint = buildPoint(accumulation, realTask);
            realPoint.realPosition = std::move(realPosition);
            realPoint.realOrientation = std::move(realOrientation);
            realPoint.realProgress = std::move(realProgress);

            realPoint.timeout = conversion::cal_distance(lastPose, pose.pose.position) * 20 + 5;//掉头5s

            realPoint.inClean = j != 0;

            realPointList.push_back(realPoint);

            accumulation++;

            lastPose.x = pose.pose.position.x;
            lastPose.y = pose.pose.position.y;
            lastPose.z = pose.pose.position.z;
        }
    }

    realTask.setTotalStep(totalStep);
    realTask.setTotalFrequency(rate);
}

std::vector<RealPoint> CoveragePointGenerator::taskGeneratePointList(RealTask &task) {
    std::vector<RealPoint> taskPointList;
    auto taskId = task.getId();
    auto roomCoverage = ExplorationCenter::instance().findRoomCoverage(taskId, false);
    auto poseList = roomCoverage.getPoseList();
    std::vector<RealPoint> realPoints;
    pose2RealPoint(task, poseList, realPoints);
    return realPoints;
}

std::vector<RealPoint> RectanglePointGenerator::taskGeneratePointList(RealTask &task) {
    std::vector<float> zoned = task.getZoned0();
    if (zoned.size() != 8)
        throw app::exception(make_error_code(error::room_mb_file_open_fail));

    std::vector<std::vector<cv::Point>> polygon_array;
    auto map = SegmentationCenter::instance().generateMat();

    std::vector<cv::Point> cvPoints;
    for (int i = 0; i < zoned.size(); i = i + 2) {
        Point point(zoned[i], zoned[i + 1]);
        auto cvPoint = MapAttribute::instance().rosPoint2MapPoint(map, point);
        cvPoints.push_back(cvPoint);
    }

    polygon_array.push_back(cvPoints);
    cv::Mat zoned_image = cv::Mat::zeros(map.rows, map.cols, CV_8UC1);
    cv::fillPoly(zoned_image, polygon_array, cv::Scalar(255));

    std::vector<geometry_msgs::Pose2D> exploration_path;
    std::vector<cv::Point> point_path;
    ExplorationCenter::instance().generatePlanningPathRect(zoned_image, BOUSTROPHEDON_EXPLORER_MODE,
                                                           exploration_path, point_path);

    ExplorationCenter::instance().pathPublish(exploration_path);

    std::vector<PoseVo> poseList;
    for (const auto &item: exploration_path) {
        poseList.emplace_back(item.y, item.x, item.theta);
    }
    std::vector<RealPoint> realPoints;
    pose2RealPoint(task, poseList, realPoints);

    return realPoints;
}

bool
CombinationPointGenerator::generateRecPointListForViewPart(std::vector<Point> zoned,
                                                           std::vector<CombinationPoseVo> &pointList) {
    if (zoned.empty() || zoned.size() != 4) {
        return false;
    }

    int index = 0;
    float min_sum = zoned[index].getX() + zoned[index].getY();

    Point start_point;
    for (int i = 1; i < zoned.size(); i++) {
        if (min_sum > (zoned[i].getX() + zoned[i].getY())) {
            min_sum = zoned[i].getX() + zoned[i].getY();
            index = i;
        }
    }

    int index_last, index_next;
    index_last = index - 1;
    index_next = index + 1;

    if (index == 0) {
        index_last = 3;
    }
    if (index == 3) {
        index_next = 0;
    }

    Point point = zoned[index];
    Point point_last = zoned[index_last];
    Point point_next = zoned[index_next];

    float x_len, y_len;
    x_len = conversion::cal_distance(point, point_last);
    y_len = conversion::cal_distance(point, point_next);

    start_point.setXandY(point.getX(), point.getY());//存放目标起点坐标


    float x_dir[2], y_dir[2];
    x_dir[0] = point_last.getX() - point.getX();
    x_dir[1] = point_last.getY() - point.getY();
    y_dir[0] = point_next.getX() - point.getX();
    y_dir[1] = point_next.getY() - point.getY();


    int num = 12;
    if (x_len <= y_len) {

        CombinationPoseVo first(point.getX(), point.getY(), 0, 0);
        pointList.push_back(first);

        if (x_len > 1.5) {
            num = ((int) ((x_len + 0.5) / 0.3) + 1 + 1) * 2;
        }

        for (int i = 0; i < num - 1; i++) {
            float x_param, y_param;
            if (i % 4 < 2)
                y_param = 1;
            else
                y_param = 0;
            x_param = (float) (((i) / 2 + (i) % 2)) / (num / 2 - 1);

            CombinationPoseVo temp_task(point.getX() + x_param * x_dir[0] + y_param * y_dir[0],
                                        point.getY() + x_param * x_dir[1] + y_param * y_dir[1],
                                        0, i + 1
            );
            pointList.push_back(temp_task);
        }
    } else {
        CombinationPoseVo first(point.getX(), point.getY(), 0, 0);
        pointList.push_back(first);


        if (y_len > 1.5) {
            num = ((int) ((y_len + 0.5) / 0.3) + 1 + 1) * 2;
        }

        for (int i = 0; i < num - 1; i++) {
            float x_param, y_param;
            if (i % 4 < 2)
                x_param = 1;
            else
                x_param = 0;
            y_param = (float) (((i) / 2 + (i) % 2)) / (num / 2 - 1);


            CombinationPoseVo temp_task(point.getX() + x_param * x_dir[0] + y_param * y_dir[0],
                                        point.getY() + x_param * x_dir[1] + y_param * y_dir[1],
                                        0, i + 1
            );
            pointList.push_back(temp_task);
        }
    }

    return true;
}

std::vector<RealPoint> CombinationPointGenerator::taskGeneratePointList(RealTask &task) {
    auto combination = task.getCombination();

    ViewPartList viewPartListTemp;
    if (!ViewPartManager::get_instance()->GetViewPartList(viewPartListTemp)) {
        throw app::exception(make_error_code(error::combination_point_get_view_part_fail));
    }

    std::string combination_id_temp = combination.getCombinationID();
    CombinationBrief combination_brief_temp;
    if (CombinationManager::get_instance()->GetCombination(combination_brief_temp, combination_id_temp) != SUCCESS_) {
        throw app::exception(make_error_code(error::combination_brief_get_fail));
    }
    CombinationDetail combination_detail_temp(combination_brief_temp);
    vector<string> partNotMached = combination_brief_temp.toDetail(combination_detail_temp, viewPartListTemp);
    if (partNotMached.size() > 0) {
        for (auto &item: partNotMached) {
            CombinationManager::get_instance()->DelatePartID(item);
        }
    }

    std::vector<ViewPart> viewPartList = combination_detail_temp.getViewPartList();
    if (viewPartList.size() <= 0) {
        throw app::exception(make_error_code(error::combination_pointlist_empty));
    }

    std::vector<CombinationPoseVo> pointList;
    for (auto &item: viewPartList) {
        switch (item.getMode()) {
            case 1: {
                auto zoned = item.getZoned();
                if (!generateRecPointListForViewPart(zoned, pointList)) {
                    LOG(ERROR) << "加载矩形" << item.getPartID() << "失败";
                }
                break;
            }
        }
    }

    if (pointList.empty()) {
        throw app::exception(make_error_code(error::combination_pointlist_empty));
    }

    std::vector<PoseVo> poseList;
    for (const auto &item: pointList) {
        poseList.push_back(PoseVo(item.getY(), item.getX(), 0));
    }

    std::vector<geometry_msgs::Pose2D> exploration_path;
    for (const auto &item: poseList) {
        geometry_msgs::Pose2D pose;
        pose.x = item.getX();
        pose.y = item.getY();
        exploration_path.push_back(pose);
    }
    ExplorationCenter::instance().pathPublish(exploration_path);

    std::vector<RealPoint> realPoints;
    combinationPose2RealPoint(task, pointList, realPoints);
    return realPoints;
}

std::vector<RealPoint> FullPointGenerator::taskGeneratePointList(RealTask &task) {
    std::vector<RealPoint> taskPointList;
    auto taskId = task.getId();
    auto roomCoverage = ExplorationCenter::instance().findRoomCoverage(taskId, true);
    auto poseList = roomCoverage.getPoseList();
    std::vector<RealPoint> realPoints;
    pose2RealPoint(task, poseList, realPoints);
    return realPoints;
}

std::vector<RealPoint> ExplorationGenerator::taskGeneratePointList(RealTask &task) {

    auto coverage = TaskExploration::explorationPlanningPath(task);

    const auto &poseList = coverage.getPoseList();
    std::vector<RealPoint> realPoints;
    pose2RealPoint(task, poseList, realPoints);

    return realPoints;
}
