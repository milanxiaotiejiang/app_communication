//
// Created by Looper on 2023/2/14.
//

#ifndef APP_COMMUNICATION_RESERVED_CALL_H
#define APP_COMMUNICATION_RESERVED_CALL_H

#include "task/call/head_tail_call.h"

class ReservedCall : public HeadTailPointCall {
protected:

    void handleManualOperation() override;

    void handleSpecialOperation() override;

    void handleErrorOperation() override;

    void handleStop() override;

    void handleExecuteTask(const RealTask &task) override;

    void handleFlowPoint(const RealPoint &point) override;

    void processControl(const RealPoint &point) override;

    void softwareInterruptTask(const RealPoint &point) override;

    void forceInterruptTask() override;

    void goodGame() override;

    void garbage() override;

    void handlePlannerPoint(const RealPoint &point) override;

    void updateProperty();

public:


    std::tuple<int, std::string, std::string> generateErrorByRealPoint(int errorId);

    void recordMotorError();

    void recordHlsError(int error_event);

    void recordLaserError(std::string error_event);
};


#endif //APP_COMMUNICATION_RESERVED_CALL_H
