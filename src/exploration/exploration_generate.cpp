//
// Created by Looper on 2023/2/21.
//

#include <geometry_msgs/Pose2D.h>
#include "exploration/exploration_generate.h"
#include "exploration/ExplorationCenter.h"
#include "segmentation/SegmentationCenter.h"

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

CoveragePathGenerator::CoveragePathGenerator() {
    CoveragePathGenerator::make_thread(run, this);
    coverage_planner_done = false;
    coverage_need_again = false;
}

void CoveragePathGenerator::realGenerator(std::vector<geometry_msgs::Pose2D> &exploration_path,
                                          std::vector<cv::Point> &point_path) {
    const cv::Mat &baseMap = SegmentationCenter::instance().generateMat();
    ExplorationCenter::instance().generatePlanningPath(baseMap, ExplorationModel::FULL,
                                                       BOUSTROPHEDON_EXPLORER_MODE, false,
                                                       cv::Point(0, 0),
                                                       exploration_path, point_path);
}

[[noreturn]] void CoveragePathGenerator::execute() {
    for (;;) {
        std::unique_lock<std::mutex> lock(cv_mut);
        cv.wait(lock, [this]() {
            return coverage_obtain_path;
        });

        coverage_planner_done = false;

        std::vector<geometry_msgs::Pose2D> exploration_path;
        std::vector<cv::Point> point_path;

        try {
            realGenerator(exploration_path, point_path);
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

            std::vector<PoseVo> poseList;
            std::vector<PointVo> pointList;
            for (const auto &item: exploration_path) {
                poseList.emplace_back(item.y, item.x, item.theta);
            }
            for (const auto &item: point_path) {
                pointList.emplace_back(item.x, item.y);
            }
            roomCoverage.setPointList(pointList);
            roomCoverage.setPoseList(poseList);
            roomCoverage.setCoverageId(uuid_string);

            if (coverage_need_again) {
                coverage_need_again = false;
                LOG(INFO) << "CoveragePathGenerator : 处理需要再次进行的规划请求，准备开始规划 ...";
            } else {
                coverage_planner_done = true;
                wait_cv.notify_one();

                coverage_obtain_path = false;
                LOG(INFO) << "CoveragePathGenerator : 规划全部完成，支持获取全覆盖路径 ...";
            }
        } else {
            LOG(INFO) << "CoveragePathGenerator : 全覆盖规划有异常情况，停止当前规划 ...";
            coverage_need_again = false;

            coverage_obtain_path = false;
        }
    }
}

RoomCoverage CoveragePathGenerator::obtainCoveragePath(int overtime) {
    LOG(INFO) << "CoveragePathGenerator " << overtime;
    if (coverage_planner_done) {
        return roomCoverage;
    }

    std::unique_lock<std::mutex> lck(wait_mutex);
    wait_cv.wait_for(lck, std::chrono::milliseconds(overtime));
    if (coverage_planner_done) {
        return roomCoverage;
    } else {
        throw app::exception(make_error_code(error::coverage_path_overtime));
    }
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
        LOG(INFO) << "CoveragePathGenerator : 已通知需要重新规划，此处拦截多次的请求 ...";
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