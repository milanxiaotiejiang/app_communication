//
// Created by Looper on 2023/2/14.
//

#include "task/call/reserved_call.h"
#include "db/property_data_base.h"

void ReservedCall::handleManualOperation() {
    switch (epoll_manual) {
        case loop::manual_epoll::manual_back:
        case loop::manual_epoll::manual_force_back:
            break;
        case loop::manual_epoll::manual_task_over:
            break;
        default:
            break;
    }
    AsyncTaskCall::handleManualOperation();
}

void ReservedCall::handleSpecialOperation() {
    switch (epoll_special) {
        case loop::special_epoll::special_low_battery: {
            break;
        }
        case loop::special_epoll::special_branch_water: {
            break;
        }
        case loop::special_epoll::special_sewage_water: {
            break;
        }
        case loop::special_epoll::special_branch_sewage_water: {
            break;
        }
        case loop::special_epoll::special_dust_push_anomaly: {
            break;
        }
        case loop::special_epoll::special_wet_tow_anomaly: {
            break;
        }
        default:
            break;
    }
    AsyncTaskCall::handleSpecialOperation();
}

void ReservedCall::handleErrorOperation() {
    switch (epoll_error) {
        case loop::error_epoll::error_manual_clean_start:
            break;
        case loop::error_epoll::error_manual_clean_end:
            break;
        case loop::error_epoll::error_lift:
            break;
        case loop::error_epoll::error_electric_move:
            break;
        case loop::error_epoll::error_unrecoverable:
            break;
        default:
            LOG_IF(INFO, DEBUG_TASK) << "AsyncTaskCall handleErrorOperation : " << epoll_error << " ...";
            break;
    }
    AsyncTaskCall::handleErrorOperation();
}

void ReservedCall::handleStop() {
    if (first_urgency_stop) {
        first_urgency_stop = false;
    } else {
        switch (urgency_stop) {
            case loop::urgency_stop::trigger_urgency_stop:
                break;
            case loop::urgency_stop::recovery_urgency_stop:
                break;
            case loop::urgency_stop::release_urgency_stop:
                break;
            default:
                break;
        }
    }
    AsyncTaskCall::handleStop();
}

void ReservedCall::handleExecuteTask(const RealTask &task) {
    notifier.triggerTaskStart(task);
    AsyncTaskCall::handleExecuteTask(task);
}

void ReservedCall::handleFlowBlock(const RealBlock &block) {
    if (block.id == FLOW_SEIZE_SEAT) {
//        setFlow(event::flow::out_base_station);
    } else if (block.id == FLOW_OUT_STATION) {
    } else if (block.id == FLOW_END_SLEEP) {
    } else if (block.id == FLOW_IN_BASE_POINT) {
    } else if (block.id == FLOW_IN_STATION) {
    } else if (block.id == FLOW_CLOSE_MECHANISM) {
    } else if (block.id == FLOW_OPEN_MECHANISM) {
    }
    HeadTailPointCall::handleFlowBlock(block);
}

void ReservedCall::processControl(const RealBlock &block) {
    HeadTailPointCall::processControl(block);
}

void ReservedCall::handlePlannerBlock(const RealBlock &block) {
    if (block.id < 0) {
        return;
    }
    if (block.taskId.empty()) {
        return;
    }
    auto plannerPoints = block.plannerPoints;
    if (plannerPoints.empty()) {
        return;
    }
    int current_step = block.already_step + block.timely_step;
    if (current_step > plannerPoints.size()) {
        return;
    }
    auto point = plannerPoints[current_step];

    AsyncTaskCall::handlePlannerBlock(block);
}

void ReservedCall::feedBackPose(const geometry_msgs::Pose &pose) {
    notifier.triggerTaskProgress(pose);
}

void ReservedCall::forceInterruptTask(event::SB sb) {
    int errorId = FLOW_ERROR_UNRECOVERABLE;
    switch (epoll_error) {
        case loop::error_epoll::error_lift:
            errorId = FLOW_ERROR_LIFT;
            break;
        case loop::error_epoll::error_electric_move:
            errorId = FLOW_ELECTRIC_MOVE;
            break;
    }

    AsyncTaskCall::forceInterruptTask(sb);

    garbage(sb);
}

void ReservedCall::softwareInterruptTask(const RealBlock &block) {
    garbage(event::SB::sb_software);
}

void ReservedCall::goodGame(event::GG gg) {
    notifier.triggerTaskEnd();
    runTask;
    AsyncTaskCall::goodGame(gg);
}

void ReservedCall::garbage(event::SB sb) {
    AsyncTaskCall::garbage(sb);
}

void ReservedCall::reset() {
    AsyncTaskCall::reset();
}