//
// Created by Looper on 2023/3/29.
//

#include "exploration/path_exploration_preview_task.h"

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

#include "db/segmentation_data_base.h"

#include "geometry_msgs/Pose2D.h"

#include "segmentation/SegmentationCenter.h"
#include "exploration/ExplorationCenter.h"
#include "simulation.h"

void TaskExploration::task2RealTask(const TaskVo &task, RealTask &realTask) {
    auto uuid = boost::uuids::to_string(boost::uuids::random_generator()());
    realTask.setId(uuid);

    realTask.setMapId(task.getOMapId());
    realTask.setTaskId(task.getId());
    realTask.setName(task.getName());
//    realTask.setRate(task.getRate());
    realTask.setMode(task.getMode());

    realTask.setWorkStatus(task.getWorkStatus());

    realTask.setPrincipal(task.isPrincipal());

    realTask.setZoned(task.getZones());
    realTask.setPartition(task.isPartition());
    realTask.setSubregions(task.getSubregions());
    realTask.setKnife(task.isKnife());

    realTask.setDeliveries(task.getDeliveries());

    realTask.setSource(task.getSource());
    realTask.setLaunchPeople(task.getLaunchPeople());
    realTask.setLaunchTime(std::time(nullptr));
}

void TaskExploration::mapElevatorOutside2RealPoint(const MapPo &map, RealPoint &realPoint) {
    RealPosition realPosition(map.elevator_position_x, map.elevator_position_y, map.elevator_position_z);
    RealOrientation realOrientation(map.elevator_orientation_x, map.elevator_orientation_y,
                                    map.elevator_orientation_z, map.elevator_orientation_w);
    realPoint.realPosition = std::move(realPosition);
    realPoint.realOrientation = std::move(realOrientation);
}

void TaskExploration::mapElevatorInside2RealPoint(const MapPo &map, RealPoint &realPoint) {
    RealPosition realPosition(map.elevator_inside_position_x, map.elevator_inside_position_y,
                              map.elevator_inside_position_z);
    RealOrientation realOrientation(map.elevator_inside_orientation_x, map.elevator_inside_orientation_y,
                                    map.elevator_inside_orientation_z, map.elevator_inside_orientation_w);
    realPoint.realPosition = std::move(realPosition);
    realPoint.realOrientation = std::move(realOrientation);
}

RoomCoverage TaskExploration::explorationPlanningPath(const RealTask &task) {
    TaskMode mode = SqliteDataBase::TaskModeFromInt(task.getMode());

    ExplorationCenter &explorationCenter = ExplorationCenter::instance();
    SegmentationCenter &segmentationCenter = SegmentationCenter::instance();

    const cv::Mat &baseMap = segmentationCenter.generateMat();
    const cv::Point2d &mapOrigin = MapAttributeSingleton::instance().getMapOrigin();

    std::vector<geometry_msgs::Pose2D> exploration_path;
    std::vector<cv::Point> point_path;
    std::vector<std::vector<geometry_msgs::Pose2D>> complex_path;

    RoomCoverage coverage;
    bool preLoaded = false;


    if (mode == TaskMode::Zoned) {
        std::vector<ZoneVo> zones = task.getZoned();
        for (const auto &zone: zones) {
            std::vector<std::vector<cv::Point>> polygon_array;

            std::vector<cv::Point> cvPoints;
            for (const auto &point: zone.getPoints()) {
                cvPoints.emplace_back(point.getX(), point.getY());
            }

            polygon_array.push_back(cvPoints);

            cv::Mat zoned_image = cv::Mat::zeros(baseMap.rows, baseMap.cols, CV_8UC1);
            cv::fillPoly(zoned_image, polygon_array, cv::Scalar(255));
            std::vector<geometry_msgs::Pose2D> sub_exploration_path;
            std::vector<cv::Point> sub_point_path;
            std::vector<std::vector<geometry_msgs::Pose2D>> sub_complex_path;
            try {
                explorationCenter.generatePlanningPathRect(task.getMapId(), zoned_image,
                                                           task.isVerifyMode() ? ENERGY_FUNCTIONAL_EXPLORER_MODE
                                                                               : Environment::instance().explorer_mode,
                                                           !task.isVerifyMode(),
                                                           sub_exploration_path, sub_point_path, sub_complex_path);

            } catch (app::exception const &e) {
                LOG(ERROR) << e.what();
            }

            for (const auto &item: sub_exploration_path) {
                exploration_path.push_back(item);
            }
            for (const auto &item: sub_point_path) {
                point_path.push_back(item);
            }
            for (const auto &vec: sub_complex_path) {
                complex_path.emplace_back(vec.begin(), vec.end());
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
            coverage.setComplexList(obtainSubregion.getComplexList());
        } else {
            if (task.isAsyncMap()) {

                explorationCenter
                        .generatePlanningPathFull(
                                task.getMapId(),
                                task.isVerifyMode() ? ENERGY_FUNCTIONAL_EXPLORER_MODE
                                                    : Environment::instance().explorer_mode,
                                true,
                                exploration_path, point_path, complex_path);
            } else {
                preLoaded = true;
                auto obtainCoverage = explorationCenter.obtainCoveragePath();
                coverage.setPoseList(obtainCoverage.getPoseList());
                coverage.setPointList(obtainCoverage.getPointList());
                coverage.setComplexList(obtainCoverage.getComplexList());
            }

        }

    } else if (mode == TaskMode::Subregion) {
        std::vector<SubregionVo> subregions = task.getSubregions();
        cv::Mat segmented_map;
        std::vector<Room> rooms;
        segmentationCenter.storage2Memory(segmented_map, rooms);

        for (const auto &subregion: subregions) {
            std::vector<geometry_msgs::Pose2D> sub_exploration_path;
            std::vector<cv::Point> sub_point_path;
            std::vector<std::vector<geometry_msgs::Pose2D>> sub_complex_path;

            const cv::Mat &oneMap = segmentationCenter.choiceOneRoom(segmented_map, rooms,
                                                                     subregion.getSubregionValue());
            explorationCenter.generatePlanningPathSub(task.getMapId(), oneMap,
                                                      task.isVerifyMode() ? ENERGY_FUNCTIONAL_EXPLORER_MODE
                                                                          : Environment::instance().explorer_mode,
                                                      !task.isVerifyMode(),
                                                      sub_exploration_path, sub_point_path, sub_complex_path);

            for (const auto &item: sub_exploration_path) {
                exploration_path.push_back(item);
            }
            for (const auto &item: sub_point_path) {
                point_path.push_back(item);
            }
            for (const auto &item: sub_complex_path) {
                complex_path.emplace_back(item.begin(), item.end());
            }
        }

    } else if (mode == TaskMode::Line) {
        explorationCenter.infinitelyNearBoundary(task.getMapId(), exploration_path, point_path, complex_path);
    } else if (mode == TaskMode::Delivery) {
        std::vector<PoseVo> poseList;
        std::vector<PointVo> pointList;
        std::vector<std::vector<PoseVo>> complexList;

        std::vector<DeliveryVo> deliverys = task.getDeliveries();
        for (const auto &delivery: deliverys) {
            geometry_msgs::Pose2D pose2D;
            pose2D.x = delivery.getPoseVo().getX();
            pose2D.y = delivery.getPoseVo().getY();
            pose2D.theta = delivery.getPoseVo().getTheta();
            exploration_path.push_back(pose2D);

            poseList.push_back(delivery.getPoseVo());
            pointList.emplace_back(delivery.getPoseVo().getX(), delivery.getPoseVo().getY());
            complexList.push_back(poseList);
        }

        coverage.setPoseList(poseList);
        coverage.setPointList(pointList);
        coverage.setComplexList(complexList);
    }

    boost::uuids::uuid uuid = boost::uuids::random_generator()();
    std::string uuid_string = boost::uuids::to_string(uuid);
    coverage.setCoverageId(uuid_string);

    if (preLoaded) {

    } else {
        if (!task.isVerifyMode())
            explorationCenter.pathPublish(exploration_path);

        TaskExploration::planningPath2RoomCoverage(coverage, exploration_path, point_path, complex_path);
    }

    if (!task.isAsyncMap()) {
        explorationCenter.cacheRoomCoverage(coverage);
    }
    return coverage;
}

void TaskExploration::planningPath2RoomCoverage(RoomCoverage &roomCoverage,
                                                const std::vector<geometry_msgs::Pose2D> &exploration_path,
                                                const std::vector<cv::Point> &point_path,
                                                const std::vector<std::vector<geometry_msgs::Pose2D>> &complex_path) {
    std::vector<PoseVo> poseList;
    std::vector<PointVo> pointList;
    std::vector<std::vector<PoseVo>> complexList;
    for (const auto &item: exploration_path) {
        poseList.emplace_back(item.y, item.x, item.theta);
    }
    for (const auto &item: point_path) {
        pointList.emplace_back(item.x, item.y);
    }
    for (const auto &vec: complex_path) {
        std::vector<PoseVo> complex;
        for (const auto &item: vec) {
            complex.emplace_back(item.y, item.x, item.theta);
        }
        complexList.push_back(complex);
    }
    roomCoverage.setPointList(pointList);
    roomCoverage.setPoseList(poseList);
    roomCoverage.setComplexList(complexList);
}
