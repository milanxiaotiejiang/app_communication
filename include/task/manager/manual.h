//
// Created by admin1 on 22-11-25.
//

#ifndef APP_COMMUNICATION_MANUAL_H
#define APP_COMMUNICATION_MANUAL_H


#include "task/call/head_tail_call.h"
#include "model/ManualModel.h"

class ManualManager {
private:
    ManualManager() = default;

    ManualManager(ManualManager &) = delete;

    ManualManager &operator=(const ManualManager &) = delete;

public:
    ~ManualManager() = default;

private:
    std::shared_ptr<HeadTailPointCall> asyncTaskCall;
public:
    static auto &instance() {
        static ManualManager obj;
        return obj;
    }

    void setAsyncTaskCall(std::shared_ptr<HeadTailPointCall> asyncTaskCallPtr) {
        ManualManager::asyncTaskCall = asyncTaskCallPtr;
    }

    void backToBase(bool force);

    void resume();

    void pause();

    void enter_manual_mode();

    ManualModel quit_manual_mode();

    bool taskRunning();

    RealTask runningTask() const;

    std::vector<RealTask> runTaskList();

    std::vector<PointProgressVo> runTaskPointList();

    void shutdown();

    void reboot();

    void restore();
};


#endif //APP_COMMUNICATION_MANUAL_H
