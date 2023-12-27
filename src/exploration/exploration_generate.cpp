//
// Created by Looper on 2023/2/21.
//

#include <geometry_msgs/Pose2D.h>
#include "exploration/exploration_generate.h"
#include "exploration/ExplorationCenter.h"
#include "segmentation/SegmentationCenter.h"
#include "exploration/path_exploration_preview_task.h"

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

#include "simulation.h"
#include "db/segmentation_data_base.h"

CoveragePathGenerator::CoveragePathGenerator() {
    CoveragePathGenerator::make_thread(run, this);
    coverage_planner_done = false;
    coverage_need_again = false;
    retried_again_count = 0;
}

void CoveragePathGenerator::realGenerator(std::vector<geometry_msgs::Pose2D> &exploration_path,
                                          std::vector<cv::Point> &point_path,
                                          std::vector<std::vector<geometry_msgs::Pose2D>> &complex_path) {
    ExplorationCenter::instance().generatePlanningPathFull(SegmentationDataBase::instance().getDbMap().id,
                                                           Environment::instance().explorer_mode, true,
                                                           exploration_path, point_path, complex_path);
}

[[noreturn]] void CoveragePathGenerator::execute() {
    for (;;) {
        std::unique_lock<std::mutex> lock(cv_mut);
        cv.wait(lock, [this]() {
            return coverage_obtain_path;
        });

        coverage_planner_done = false;

        roomCoverage.setCoverageId("");
        roomCoverage.setPointList(std::vector<PointVo>{});
        roomCoverage.setPoseList(std::vector<PoseVo>{});
        roomCoverage.setComplexList(std::vector<std::vector<PoseVo> >{});

        std::vector<geometry_msgs::Pose2D> exploration_path;
        std::vector<cv::Point> point_path;
        std::vector<std::vector<geometry_msgs::Pose2D>> complex_path;

        try {
            realGenerator(exploration_path, point_path, complex_path);
            ExplorationCenter::instance().pathPublish(exploration_path);
        } catch (app::exception const &e) {
            LOG(ERROR) << "CoveragePathGenerator app::exception : " << e.what();
        } catch (std::exception const &e) {
            LOG(ERROR) << "CoveragePathGenerator std::exception : " << e.what();
        } catch (...) {
            LOG(ERROR) << "CoveragePathGenerator exception ... ";
        }

        if (!exploration_path.empty() && !point_path.empty()) {
            boost::uuids::uuid uuid = boost::uuids::random_generator()();
            std::string uuid_string = boost::uuids::to_string(uuid);

            TaskExploration::planningPath2RoomCoverage(roomCoverage, exploration_path, point_path, complex_path);
            roomCoverage.setCoverageId(uuid_string);

            if (coverage_need_again) {
                coverage_need_again = false;
                LOG_IF(INFO, DEBUG_EXPLORATION_CACHE)
                                << "CoveragePathGenerator : 处理需要再次进行的规划请求，准备开始规划 ...";
            } else {
                coverage_planner_done = true;
                wait_cv.notify_one();

                coverage_obtain_path = false;
                retried_again_count = 0;
                LOG_IF(INFO, DEBUG_EXPLORATION_CACHE) << "CoveragePathGenerator : 规划全部完成，支持获取全覆盖路径 ...";
            }
        } else {
            retried_again_count++;
            if (retried_again_count > 2) {
                LOG_IF(INFO, DEBUG_EXPLORATION_CACHE)
                                << "CoveragePathGenerator : 全覆盖规划有异常情况，停止当前规划 ...";
                coverage_need_again = false;
                coverage_obtain_path = false;
                coverage_planner_done = true;
            } else {
                LOG_IF(INFO, DEBUG_EXPLORATION_CACHE)
                                << "CoveragePathGenerator : 全覆盖规划有异常情况，尝试重新规划 " << retried_again_count
                                << " ...";
                sleep(5);
                coverage_need_again = true;
            }
        }
    }
}

RoomCoverage CoveragePathGenerator::obtainCoveragePath(int overtime) {
    LOG_IF(INFO, DEBUG_EXPLORATION_CACHE) << "CoveragePathGenerator " << overtime;
    if (coverage_planner_done) {
        publish();
        return roomCoverage;
    }

    std::unique_lock<std::mutex> lck(wait_mutex);
    wait_cv.wait_for(lck, std::chrono::milliseconds(overtime));
    if (coverage_planner_done) {
        publish();
        return roomCoverage;
    } else {
        throw app::exception(make_error_code(error::coverage_path_overtime));
    }
}

void CoveragePathGenerator::publish() const {
    std::vector<geometry_msgs::Pose2D> exploration_path;
    for (const auto &item: roomCoverage.getPoseList()) {
        geometry_msgs::Pose2D pose2D;
        pose2D.x = item.getY();
        pose2D.y = item.getX();
        pose2D.theta = item.getTheta();
        exploration_path.push_back(pose2D);
    }
//    std::vector<std::vector<geometry_msgs::Pose2D>> complex_path;
//    std::vector<geometry_msgs::Pose2D> complex_exploration_path;
//    for (const auto &complex: roomCoverage.getComplexList()) {
//        for (const auto &item: complex) {
//            geometry_msgs::Pose2D pose2D;
//            pose2D.x = item.getY();
//            pose2D.y = item.getX();
//            pose2D.theta = item.getTheta();
//            complex_exploration_path.push_back(pose2D);
//        }
//        complex_path.push_back(complex_exploration_path);
//    }
    ExplorationCenter::instance().pathPublish(exploration_path);
}

void CoveragePathGenerator::preloadCoveragePath() {
    {
        std::unique_lock<std::mutex> lock(cv_mut);
        coverage_obtain_path = true;
    }
    cv.notify_one();
}

void CoveragePathGenerator::repaintCoveragePath() {
    if (coverage_need_again) {
        LOG_IF(INFO, DEBUG_EXPLORATION_CACHE) << "CoveragePathGenerator : 已通知需要重新规划，此处拦截多次的请求 ...";
        return;
    }

    if (coverage_planner_done) {
        {
            std::unique_lock<std::mutex> lock(cv_mut);
            coverage_obtain_path = true;
        }
        cv.notify_one();
    } else {
        coverage_need_again = true;
    }
}

/**
*
*/


SubregionPathGenerator::SubregionPathGenerator() {
    SubregionPathGenerator::make_thread(run, this);
    coverage_planner_done = false;
    coverage_need_again = false;
}

void SubregionPathGenerator::realGenerator(std::vector<geometry_msgs::Pose2D> &exploration_path,
                                           std::vector<cv::Point> &point_path,
                                           std::vector<std::vector<geometry_msgs::Pose2D>> &complex_path) {
    cv::Mat segmented_map;
    std::vector<Room> rooms;
    SegmentationCenter::instance().storage2Memory(segmented_map, rooms);
    ExplorationCenter::instance().generatePlanningSegmentationPath(SegmentationDataBase::instance().getDbMap().id,
                                                                   segmented_map, rooms,
                                                                   Environment::instance().explorer_mode, true,
                                                                   exploration_path, point_path, complex_path);
}

[[noreturn]] void SubregionPathGenerator::execute() {
    for (;;) {
        std::unique_lock<std::mutex> lock(cv_mut);
        cv.wait(lock, [this]() {
            return coverage_obtain_path;
        });

        coverage_planner_done = false;

        roomCoverage.setCoverageId("");
        roomCoverage.setPointList(std::vector<PointVo>{});
        roomCoverage.setPoseList(std::vector<PoseVo>{});
        roomCoverage.setComplexList(std::vector<std::vector<PoseVo> >{});

        std::vector<geometry_msgs::Pose2D> exploration_path;
        std::vector<cv::Point> point_path;
        std::vector<std::vector<geometry_msgs::Pose2D>> complex_path;

        try {
            realGenerator(exploration_path, point_path, complex_path);
            ExplorationCenter::instance().pathPublish(exploration_path);
        } catch (app::exception const &e) {
            LOG(ERROR) << "SubregionPathGenerator app::exception : " << e.what();
        } catch (std::exception const &e) {
            LOG(ERROR) << "SubregionPathGenerator std::exception : " << e.what();
        } catch (...) {
            LOG(ERROR) << "SubregionPathGenerator exception ... ";
        }

        if (!exploration_path.empty() && !point_path.empty()) {
            boost::uuids::uuid uuid = boost::uuids::random_generator()();
            std::string uuid_string = boost::uuids::to_string(uuid);

            TaskExploration::planningPath2RoomCoverage(roomCoverage, exploration_path, point_path, complex_path);
            roomCoverage.setCoverageId(uuid_string);

            if (coverage_need_again) {
                coverage_need_again = false;
                LOG_IF(INFO, DEBUG_EXPLORATION_CACHE)
                                << "SubregionPathGenerator : 分区处理需要再次进行的规划请求，准备开始规划 ...";
            } else {
                coverage_planner_done = true;
                wait_cv.notify_one();

                coverage_obtain_path = false;
                LOG_IF(INFO, DEBUG_EXPLORATION_CACHE)
                                << "SubregionPathGenerator : 分区规划全部完成，支持获取全覆盖路径 ...";
            }
        } else {
            LOG_IF(INFO, DEBUG_EXPLORATION_CACHE)
                            << "SubregionPathGenerator : 分区全覆盖规划有异常情况，停止当前规划 ...";
            coverage_need_again = false;
            coverage_obtain_path = false;
            coverage_planner_done = true;
        }
    }
}

RoomCoverage SubregionPathGenerator::obtainCoveragePath(int overtime) {
    LOG_IF(INFO, DEBUG_EXPLORATION_CACHE) << "SubregionPathGenerator " << overtime;
    if (coverage_planner_done) {
        publish();
        return roomCoverage;
    }

    std::unique_lock<std::mutex> lck(wait_mutex);
    wait_cv.wait_for(lck, std::chrono::milliseconds(overtime));
    if (coverage_planner_done) {
        publish();
        return roomCoverage;
    } else {
        throw app::exception(make_error_code(error::coverage_path_overtime));
    }
}

void SubregionPathGenerator::publish() const {
    std::vector<geometry_msgs::Pose2D> exploration_path;
    for (const auto &item: roomCoverage.getPoseList()) {
        geometry_msgs::Pose2D pose2D;
        pose2D.x = item.getY();
        pose2D.y = item.getX();
        pose2D.theta = item.getTheta();
        exploration_path.push_back(pose2D);
    }
//    std::vector<std::vector<geometry_msgs::Pose2D>> complex_path;
//    std::vector<geometry_msgs::Pose2D> complex_exploration_path;
//    for (const auto &complex: roomCoverage.getComplexList()) {
//        for (const auto &item: complex) {
//            geometry_msgs::Pose2D pose2D;
//            pose2D.x = item.getY();
//            pose2D.y = item.getX();
//            pose2D.theta = item.getTheta();
//            complex_exploration_path.push_back(pose2D);
//        }
//        complex_path.push_back(complex_exploration_path);
//    }
    ExplorationCenter::instance().pathPublish(exploration_path);
}

void SubregionPathGenerator::preloadCoveragePath() {
//    {
//        std::unique_lock<std::mutex> lock(cv_mut);
//        coverage_obtain_path = true;
//    }
//    cv.notify_one();
}

void SubregionPathGenerator::repaintCoveragePath() {
//    if (coverage_need_again) {
//        LOG_IF(INFO, DEBUG_EXPLORATION_CACHE) << "SubregionPathGenerator : 分区已通知需要重新规划，此处拦截多次的请求 ...";
//        return;
//    }
//
//    if (coverage_planner_done) {
//        {
//            std::unique_lock<std::mutex> lock(cv_mut);
//            coverage_obtain_path = true;
//        }
//        cv.notify_one();
//    } else {
//        coverage_need_again = true;
//    }
}