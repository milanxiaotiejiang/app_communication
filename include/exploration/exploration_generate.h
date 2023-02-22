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

    std::atomic<bool> plannerDone;
    std::atomic<bool> needAgain;

    bool obtainPath;

    RoomCoverage roomCoverage;

    void realGenerator();

protected:
    void execute() override;

public:
    CoveragePathGenerator();

    RoomCoverage obtainCoveragePath(int overtime);

    void preloadCoveragePath();

    void repaintCoveragePath();
};


#endif //APP_COMMUNICATION_EXPLORATION_GENERATE_H
