//
// Created by Looper on 2023/2/14.
//

#ifndef APP_COMMUNICATION_RESERVED_CALL_H
#define APP_COMMUNICATION_RESERVED_CALL_H

#include "task/call/head_tail_call.h"

class ReservedCall : public HeadTailPointCall {
private:
    bool first_urgency_stop = true;

protected:

    void handleManualOperation() override;

    void handleSpecialOperation() override;

    void handleErrorOperation() override;

    void handleStop() override;

    void handleExecuteTask(const RealTask &task) override;

    void handleFlowBlock(const RealBlock &block) override;

    void processControl(const RealBlock &block) override;

    void softwareInterruptTask(const RealBlock &block) override;

    void forceInterruptTask(event::SB sb) override;

    void goodGame(event::GG gg) override;

    void garbage(event::SB sb) override;

    void handlePlannerBlock(const RealBlock &block) override;

public:


    std::tuple<int, std::string, std::string> generateErrorByRealPoint(int errorId);

    void recordMotorError();

    void recordMopError();

    void recordHlsError(int error_event);

    void recordLaserError(std::string error_event);
};


#endif //APP_COMMUNICATION_RESERVED_CALL_H
