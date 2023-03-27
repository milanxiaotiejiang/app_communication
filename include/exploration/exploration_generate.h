//
// Created by Looper on 2023/2/21.
//

#ifndef APP_COMMUNICATION_EXPLORATION_GENERATE_H
#define APP_COMMUNICATION_EXPLORATION_GENERATE_H

#include <atomic>
#include "future/async_call.h"
#include "condition_variable"
#include "mutex"
#include "model/RoomVo.h"

class CoveragePathGenerator : public AsyncCall {
private:
    std::condition_variable cv;
    std::mutex cv_mut;

    std::atomic<bool> coverage_planner_done;
    std::atomic<bool> coverage_need_again;

    bool coverage_obtain_path = false;

    RoomCoverage roomCoverage;

    std::condition_variable wait_cv;
    std::mutex wait_mutex;

    void realGenerator(std::vector<geometry_msgs::Pose2D> &exploration_path,
                       std::vector<cv::Point> &point_path);

protected:
    [[noreturn]] void execute() override;

public:
    CoveragePathGenerator();

    RoomCoverage obtainCoveragePath(int overtime);

    void preloadCoveragePath();

    void repaintCoveragePath();

    void publish() const;
};

class SubregionPathGenerator : public AsyncCall {
private:
    std::condition_variable cv;
    std::mutex cv_mut;

    std::atomic<bool> coverage_planner_done;
    std::atomic<bool> coverage_need_again;

    bool coverage_obtain_path = false;

    RoomCoverage roomCoverage;

    std::condition_variable wait_cv;
    std::mutex wait_mutex;

    void realGenerator(std::vector<geometry_msgs::Pose2D> &exploration_path,
                       std::vector<cv::Point> &point_path);

protected:
    [[noreturn]] void execute() override;

public:
    SubregionPathGenerator();

    RoomCoverage obtainCoveragePath(int overtime);

    void preloadCoveragePath();

    void repaintCoveragePath();

    void publish() const;
};


#endif //APP_COMMUNICATION_EXPLORATION_GENERATE_H
