//
// Created by Looper on 2023/3/29.
//

#include "exploration/path_exploration_preview_task.h"

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

#include "db/segmentation_data_base.h"
#include "db/task_data_base.h"

#include "geometry_msgs/Pose.h"
#include "geometry_msgs/Pose2D.h"

#include "segmentation/SegmentationCenter.h"
#include "exploration/ExplorationCenter.h"
#include "exploration/handle_exploration_display.h"

void TaskExploration::task2RealTask(const Task &task, RealTask &realTask) {
    realTask.setId(task.getTaskId());
    realTask.setMode(task.getMode());
    realTask.setRate(task.getRate());
    realTask.setZoned0(task.getZoned());
    realTask.setCombination(task.getCombination());
    realTask.setLaunchPeople(task.getLaunchPeople());
    realTask.setLaunchTime(task.getLaunchTime());

    realTask.setRenew(false);
}

void TaskExploration::task2RealTask(const TaskVo &task, RealTask &realTask) {
    auto uuid = boost::uuids::to_string(boost::uuids::random_generator()());
    realTask.setId(uuid);
    realTask.setMapId(task.getOMapId());
    realTask.setTaskId(task.getId());
    realTask.setName(task.getName());
    realTask.setRate(task.getRate());
    realTask.setMode(task.getMode());

    realTask.setWorkStatus(task.getWorkStatus());

    realTask.setPrincipal(task.isPrincipal());

    realTask.setZoned(task.getZones());
    realTask.setPartition(task.isPartition());
    realTask.setSubregions(task.getSubregions());
    realTask.setKnife(task.isKnife());

    realTask.setSource(task.getSource());
    realTask.setLaunchPeople(task.getLaunchPeople());
    realTask.setLaunchTime(std::time(nullptr));

    realTask.setRenew(true);
}

RoomCoverage TaskExploration::explorationPlanningPath(const RealTask &task) {
    TaskMode mode = SqliteDataBase::TaskModeFromInt(task.getMode());

    ExplorationCenter &explorationCenter = ExplorationCenter::instance();
    SegmentationCenter &segmentationCenter = SegmentationCenter::instance();

    const cv::Mat &baseMap = segmentationCenter.generateMat();

    std::vector<geometry_msgs::Pose2D> exploration_path;
    std::vector<cv::Point> point_path;

    RoomCoverage coverage;
    bool preLoaded;


    if (mode == TaskMode::Zoned) {
        vector<std::vector<PointVo>> zones = task.getZoned();
        for (const auto &zone: zones) {
            std::vector<std::vector<cv::Point>> polygon_array;

            std::vector<cv::Point> cvPoints;
            for (const auto &item: zone) {
                cvPoints.emplace_back(item.getX(), item.getY());
            }

            polygon_array.push_back(cvPoints);

            cv::Mat zoned_image = cv::Mat::zeros(baseMap.rows, baseMap.cols, CV_8UC1);
            cv::fillPoly(zoned_image, polygon_array, cv::Scalar(255));
            std::vector<geometry_msgs::Pose2D> sub_exploration_path;
            std::vector<cv::Point> sub_point_path;
            try {
                explorationCenter.generatePlanningPathRect(zoned_image, BOUSTROPHEDON_EXPLORER_MODE,
                                                           sub_exploration_path, sub_point_path);

            } catch (app::exception const &e) {
                LOG(ERROR) << e.what();
            }

            for (const auto &item: sub_exploration_path) {
                exploration_path.push_back(item);
            }
            for (const auto &item: sub_point_path) {
                point_path.push_back(item);
            }
        }

//        重叠区域是否需要处理
//        cv::Mat zoned_image = cv::Mat::zeros(baseMap.rows, baseMap.cols, CV_8UC1);
//
//        for (const auto &zone: zones) {
//            std::vector<std::vector<cv::Point>> polygon_array;
//
//            std::vector<cv::Point> cvPoints;
//            for (const auto &item: zone) {
//                cvPoints.emplace_back(item.getX(), item.getY());
//            }
//
//            polygon_array.push_back(cvPoints);
//
//            cv::Mat zeros_image = cv::Mat::zeros(baseMap.rows, baseMap.cols, CV_8UC1);
//            cv::fillPoly(zeros_image, polygon_array, cv::Scalar(255));
//
//            cv::imshow("zeros_image", zeros_image);
//            cv::waitKey();
//
//            cv::bitwise_or(zoned_image, zeros_image, zoned_image);
//
//            cv::imshow("zoned_image", zoned_image);
//            cv::waitKey();
//        }
//
//        std::vector<geometry_msgs::Pose2D> sub_exploration_path;
//        std::vector<cv::Point> sub_point_path;
//        try {
//            explorationCenter.generatePlanningPathRect(zoned_image, BOUSTROPHEDON_EXPLORER_MODE,
//                                                       sub_exploration_path, sub_point_path);
//
//        } catch (app::exception const &e) {
//            LOG(ERROR) << e.what();
//        }
//
//        for (const auto &item: sub_exploration_path) {
//            exploration_path.push_back(item);
//        }
//        for (const auto &item: sub_point_path) {
//            point_path.push_back(item);
//        }

    } else if (mode == TaskMode::Cover) {
        if (task.isPartition()) {
//            cv::Mat segmented_map;
//            std::vector<Room> rooms;
//            segmentationCenter.storage2Memory(segmented_map, rooms);
//            explorationCenter.generatePlanningSegmentationPath(baseMap, segmented_map, rooms,
//                                                               BOUSTROPHEDON_EXPLORER_MODE,
//                                                               exploration_path, point_path);
            preLoaded = true;
            auto obtainSubregion = explorationCenter.obtainSubregionPath();
            coverage.setPoseList(obtainSubregion.getPoseList());
            coverage.setPointList(obtainSubregion.getPointList());
        } else {
//            explorationCenter.generatePlanningPathFull(baseMap, BOUSTROPHEDON_EXPLORER_MODE,
//                                                       exploration_path, point_path);
            preLoaded = true;
            auto obtainCoverage = explorationCenter.obtainCoveragePath();
            coverage.setPoseList(obtainCoverage.getPoseList());
            coverage.setPointList(obtainCoverage.getPointList());
        }

    } else if (mode == TaskMode::Subregion) {
        std::vector<int> subregions = task.getSubregions();
        cv::Mat segmented_map;
        std::vector<Room> rooms;
        segmentationCenter.storage2Memory(segmented_map, rooms);

        for (const auto subregionId: subregions) {
            std::vector<geometry_msgs::Pose2D> sub_exploration_path;
            std::vector<cv::Point> sub_point_path;

            const cv::Mat &oneMap = segmentationCenter.choiceOneRoom(segmented_map, rooms, subregionId);
            explorationCenter.generatePlanningPathSub(oneMap, BOUSTROPHEDON_EXPLORER_MODE,
                                                      sub_exploration_path, sub_point_path);

            for (const auto &item: sub_exploration_path) {
                exploration_path.push_back(item);
            }
            for (const auto &item: sub_point_path) {
                point_path.push_back(item);
            }
        }

    } else if (mode == TaskMode::Line) {
        explorationCenter.infinitelyNearBoundary(baseMap, exploration_path, point_path);
    }

    boost::uuids::uuid uuid = boost::uuids::random_generator()();
    string uuid_string = boost::uuids::to_string(uuid);
    coverage.setCoverageId(uuid_string);

    if (preLoaded) {

    } else {
        explorationCenter.pathPublish(exploration_path);

        std::vector<PoseVo> poseList;
        std::vector<PointVo> pointList;
        for (const auto &item: exploration_path) {
            poseList.emplace_back(item.y, item.x, item.theta);
        }
        for (const auto &item: point_path) {
            pointList.emplace_back(item.x, item.y);
        }

        coverage.setPointList(pointList);
        coverage.setPoseList(poseList);
    }

    explorationCenter.cacheRoomCoverage(coverage);

    return coverage;
}
