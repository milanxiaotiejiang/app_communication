//
// Created by Looper on 2022/11/5.
//

#include "task/point_routine.h"
#include "task/status/state_machine.h"
#include <geometry_msgs/Pose2D.h>

void PointRoutine::pathActive() {

}

void PointRoutine::pathFeedback(const replan_msgs::ReplanFeedbackConstPtr &replan, int blockId) {
    replan_msgs::ReplanFeedback_<std::allocator<void>>::_base_position_type stamped = replan->base_position;
    asyncTaskCall->executeOnPathFeedBack(blockId,
                                         replan->current_step,
                                         replan->goal_step,
                                         replan->current_goal,
                                         stamped.pose);
}

void PointRoutine::pathDone(const actionlib::SimpleClientGoalState &state, int blockId) {
    event::error status = event::error::SUCCEEDED;
    if (state == actionlib::SimpleClientGoalState::StateEnum::SUCCEEDED) {
        status = event::error::SUCCEEDED;
    } else if (state == actionlib::SimpleClientGoalState::StateEnum::PENDING) {
        status = event::error::PENDING;
    } else if (state == actionlib::SimpleClientGoalState::StateEnum::ACTIVE) {
        status = event::error::ACTIVE;
    } else if (state == actionlib::SimpleClientGoalState::StateEnum::RECALLED) {
        status = event::error::RECALLED;
    } else if (state == actionlib::SimpleClientGoalState::StateEnum::REJECTED) {
        status = event::error::REJECTED;
    } else if (state == actionlib::SimpleClientGoalState::StateEnum::PREEMPTED) {
        status = event::error::PREEMPTED;
    } else if (state == actionlib::SimpleClientGoalState::StateEnum::ABORTED) {
        status = event::error::ABORTED;
    } else if (state == actionlib::SimpleClientGoalState::StateEnum::LOST) {
        status = event::error::LOST;
    }
    asyncTaskCall->executeOnPathDone(blockId, status, state.getText());
}

void PointRoutine::crash() {
    asyncTaskCall->executeOnPathDone(-1, event::error::CRASH, "crash");
}

void PointRoutine::pointActive() {

}

void PointRoutine::pointFeedback(const back_charge_msgs::CoreMoveFeedbackConstPtr &pose) {
    back_charge_msgs::CoreMoveFeedback_<std::allocator<void>>::_current_position_type stamped = pose->current_position;
}

void PointRoutine::pointDone(const actionlib::SimpleClientGoalState &state) {
    event::error status = event::error::SUCCEEDED;
    if (state == actionlib::SimpleClientGoalState::StateEnum::SUCCEEDED) {
        status = event::error::SUCCEEDED;
    } else if (state == actionlib::SimpleClientGoalState::StateEnum::PENDING) {
        status = event::error::PENDING;
    } else if (state == actionlib::SimpleClientGoalState::StateEnum::ACTIVE) {
        status = event::error::ACTIVE;
    } else if (state == actionlib::SimpleClientGoalState::StateEnum::RECALLED) {
        status = event::error::RECALLED;
    } else if (state == actionlib::SimpleClientGoalState::StateEnum::REJECTED) {
        status = event::error::REJECTED;
    } else if (state == actionlib::SimpleClientGoalState::StateEnum::PREEMPTED) {
        status = event::error::PREEMPTED;
    } else if (state == actionlib::SimpleClientGoalState::StateEnum::ABORTED) {
        status = event::error::ABORTED;
    } else if (state == actionlib::SimpleClientGoalState::StateEnum::LOST) {
        status = event::error::LOST;
    }
    asyncTaskCall->executeOnPointDone(status);
}
