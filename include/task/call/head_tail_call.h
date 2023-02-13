//
// Created by Looper on 2022/11/26.
//

#ifndef APP_COMMUNICATION_HEAD_TAIL_CALL_H
#define APP_COMMUNICATION_HEAD_TAIL_CALL_H

#include "task/async_task_call.h"

class HeadTailPointCall : public AsyncTaskCall {
private:
    void callGoFirstPoint(RealPoint point);

    void exchangeFrontPoint(const RealPoint &point);

public:
    void handleFlowPoint(const RealPoint &point) override;

    void processControl(const RealPoint &point) override;

    bool canIssuedTask(const std::string &launchPeople);

    bool taskRunning();
};


#endif //APP_COMMUNICATION_HEAD_TAIL_CALL_H
