//
// Created by Looper on 2022/11/26.
//

#ifndef APP_COMMUNICATION_HEAD_TAIL_CALL_H
#define APP_COMMUNICATION_HEAD_TAIL_CALL_H

#include "task/async_task_call.h"

class HeadTailPointCall : public AsyncTaskCall {
private:
    void callGoFirstPoint(RealBlock block);

    void exchangeFrontPoint(const RealBlock &point);

protected:
    void handleFlowBlock(const RealBlock &block) override;

    void processControl(const RealBlock &block) override;

    virtual void softwareInterruptTask(const RealBlock &point) = 0;

    void callOpenMechanism(const WorkStatus &status, bool knife, function<void()> f) override;

    void callCloseMechanism(function<void()> f) override;

public:
    bool canIssuedTask(const RealTask &task);

    bool taskRunning();
};


#endif //APP_COMMUNICATION_HEAD_TAIL_CALL_H
