//
// Created by Looper on 2022/11/5.
//

#include "task/point_routine.h"
#include "task/status/state_machine.h"

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

    asyncTaskCall->executeOnNext(status);
}

void PointRoutine::pointActive() {

}

void PointRoutine::pointFeedback(geometry_msgs::PoseStamped_<allocator<void>> stamped) {
//    LOG(INFO) << "feedbackCb : " << stamped.pose.position.x << " " << stamped.pose.position.y;
}
