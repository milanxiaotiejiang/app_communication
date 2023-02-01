//
// Created by Looper on 2022/11/5.
//

#include "task/point_planner.h"
#include "glog/logging.h"
#include "BaseThrowable.h"
#include "task/point_routine.h"

void PointPlanner::point2Goal(const RealPoint &point, move_base_msgs::MoveBaseGoal &goal) {
    goal.target_pose.header.frame_id = "map";
    goal.target_pose.header.stamp = ros::Time::now();
    goal.target_pose.pose.position.x = point.realPosition.x;
    goal.target_pose.pose.position.y = point.realPosition.y;
    goal.target_pose.pose.position.z = point.realPosition.z;
    goal.target_pose.pose.orientation.x = point.realOrientation.x;
    goal.target_pose.pose.orientation.y = point.realOrientation.y;
    goal.target_pose.pose.orientation.z = point.realOrientation.z;
    goal.target_pose.pose.orientation.w = point.realOrientation.w;
}

void PointPlanner::doneCd(const actionlib::SimpleClientGoalState &state,
                          const move_base_msgs::MoveBaseResultConstPtr &result) {
    PointRoutine::instance().pointDone(state);
}

void PointPlanner::activeCd() {
    PointRoutine::instance().pointActive();
}

void PointPlanner::feedbackCb(const move_base_msgs::MoveBaseFeedbackConstPtr &feedback) {
    PointRoutine::instance().pointFeedback(feedback->base_position);
}

void PointPlanner::initialize() {
    LOG(INFO) << "PointPlanner initialize ...";
    std::thread moveBaseThread([this]() {
        move_base = new actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction>("move_base", true);
        move_base->waitForServer();
        initialize_finish = true;
        LOG(INFO) << "PointPlanner open ...";
    });
    moveBaseThread.detach();
}

void PointPlanner::gotoPlannerPoint(const RealPoint &realPoint) {
    if (!initialize_finish) {
        throw app::exception(make_error_code(error::task_planner_failed_to_start));
    }
    move_base_msgs::MoveBaseGoal goal;
    point2Goal(realPoint, goal);
    move_base->sendGoal(goal, &doneCd, &activeCd, &feedbackCb);
}

void PointPlanner::cancelGoal() {
    move_base->cancelGoal();
}

void PointPlanner::backBasePoint() {
    move_base_msgs::MoveBaseGoal goal;
    goal.target_pose.header.frame_id = "map";
    goal.target_pose.header.stamp = ros::Time::now();
    goal.target_pose.pose.position.x = RETURN_POINT_X_;
    goal.target_pose.pose.position.y = 0;
    goal.target_pose.pose.position.z = 0;
    goal.target_pose.pose.orientation.x = 0;
    goal.target_pose.pose.orientation.y = 0;
    goal.target_pose.pose.orientation.z = 0;
    goal.target_pose.pose.orientation.w = 1;
    move_base->sendGoal(goal, &doneCd, &activeCd, &feedbackCb);
}
