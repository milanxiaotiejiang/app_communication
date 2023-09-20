//
// Created by Looper on 2023/2/14.
//

#ifndef APP_COMMUNICATION_RESERVED_CALL_H
#define APP_COMMUNICATION_RESERVED_CALL_H

#include "task/call/head_tail_call.h"

class ReservedCall : public HeadTailPointCall {
private:
    bool first_urgency_stop = true;

    bool low_battery_back_charge_escalation = false;
    bool clean_water_level_check_failed_escalation = false;
    bool dirty_water_level_check_failed_escalation = false;
    bool motor_error_recovery_failed_escalation = false;
    bool mop_error_recovery_success_escalation = false;

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

    void feedBackPose(const geometry_msgs::Pose &pose) override;

    void reset() override;

public:

    std::tuple<int, std::string, std::string> generateErrorByRealPoint(int errorId);

};


#endif //APP_COMMUNICATION_RESERVED_CALL_H
