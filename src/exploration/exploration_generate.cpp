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
}

void CoveragePathGenerator::realGenerator() {
    plannerDone = false;
    std::vector<geometry_msgs::Pose2D> exploration_path;
    std::vector<cv::Point> point_path;
    const cv::Mat &baseMap = SegmentationCenter::instance().generateMat();
    ExplorationCenter::instance().generatePlanningPath(baseMap, ExplorationModel::FULL,
                                                       BOUSTROPHEDON_EXPLORER_MODE, false,
                                                       cv::Point(0, 0),
                                                       exploration_path, point_path);

    ExplorationCenter::instance().pathPublish(exploration_path);
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

    roomCoverage = RoomCoverage(uuid_string, pointList, poseList);
    plannerDone = true;
}

void CoveragePathGenerator::execute() {
    for (;;) {
        std::unique_lock<std::mutex> lock(cv_mut);
        cv.wait(lock, [this]() {
            return obtainPath;
        });
        obtainPath = false;

        realGenerator();

        needAgain = false;
    }
}

RoomCoverage CoveragePathGenerator::obtainCoveragePath(int overtime) {
    LOG(INFO) << "CoveragePathGenerator " << overtime;
    if (plannerDone) {
        return roomCoverage;
    }

    std::condition_variable cv;
    std::mutex mutex;
    std::unique_lock<std::mutex> lck(mutex);
    cv.wait_for(lck, std::chrono::milliseconds(overtime));
    if (plannerDone) {
        return roomCoverage;
    } else {
        throw app::exception(make_error_code(error::coverage_path_overtime));
    }
}

void CoveragePathGenerator::preloadCoveragePath() {
    {
        std::unique_lock<std::mutex> lock(cv_mut);
        obtainPath = true;
    }
    cv.notify_one();
}

void CoveragePathGenerator::repaintCoveragePath() {
    if (needAgain) {
        LOG(INFO) << "CoveragePathGenerator : 已有状态可重新规划全覆盖路径，此处拦截多次的请求 ...";
        return;
    }

    if (plannerDone) {
        {
            std::unique_lock<std::mutex> lock(cv_mut);
            obtainPath = true;
        }
        cv.notify_one();
    } else {
        needAgain = true;
    }

}