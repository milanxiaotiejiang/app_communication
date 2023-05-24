//
// Created by admin1 on 22-11-25.
//

#ifndef APP_COMMUNICATION_MANUAL_H
#define APP_COMMUNICATION_MANUAL_H


#include "task/call/head_tail_call.h"

class ManualManager {
private:
    HeadTailPointCall *asyncTaskCall;
public:
    static auto &instance() {
        static ManualManager obj;
        return obj;
    }

    void setAsyncTaskCall(HeadTailPointCall *asyncTaskCall) {
        ManualManager::asyncTaskCall = asyncTaskCall;
    }

    void backToBase(bool force);

    void resume();

    void pause();

    void enter_manul_mode();

    void quit_manual_mode();

    bool taskRunning();

    RealTask runningTask() const;

    std::vector<RealTask> runTaskList();

    std::vector<RealBlock> runTaskBlock();

    void shutdown();

    void reboot();

};


#endif //APP_COMMUNICATION_MANUAL_H
