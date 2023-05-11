//
// Created by Looper on 2022/11/5.
//

#include "task/point_planner.h"
#include "glog/logging.h"
#include "BaseThrowable.h"
#include "task/point_routine.h"
#include "future/node/node_control.h"

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
    move_base_msgs::MoveBaseFeedback_<allocator<void>>::_base_position_type stamped = feedback->base_position;
    geometry_msgs::PoseStamped_<allocator<void>>::_pose_type pose = stamped.pose;
    geometry_msgs::Pose_<allocator<void>>::_position_type &point = pose.position;
    geometry_msgs::Pose2D pose2D;
    pose2D.x = point.x;
    pose2D.y = point.y;
    pose2D.theta = 0.;
    PointRoutine::instance().pointFeedback(pose2D);
}

void PointPlanner::initialize(ros::NodeHandle handle) {
    PointPlanner::handle = handle;
    LOG(INFO) << "PointPlanner initialize ...";
    initialize_finish = true;
}

bool PointPlanner::waitForMoveBaseServer() {
    share_move_base.reset();
    share_move_base = std::make_shared<MoveBaseAction>("move_base", true);
//    share_move_base->waitForServer();
    return share_move_base->waitForServer(ros::Duration(5));
}

void PointPlanner::resetForMoveBaseServer() {
    share_move_base.reset();
}

void PointPlanner::gotoPlannerPoint(const RealPoint &realPoint) {
    if (!initialize_finish) {
        throw app::exception(make_error_code(error::task_planner_failed_to_start));
    }
    LOG(INFO) << "AsyncTaskFramework : gotoPlannerPoint " << realPoint.realPosition << " ...";
    move_base_msgs::MoveBaseGoal goal;
    point2Goal(realPoint, goal);
    share_move_base->sendGoal(goal, &doneCd, &activeCd, &feedbackCb);
}

void PointPlanner::gotoPlannerFirstPoint(const RealPoint &realPoint) {
    xyGoalTolerance.d(0.15);
    yawGoalTolerance.d(0.15);
    gotoPlannerPoint(realPoint);
}

void PointPlanner::cancelGoal() {
    share_move_base->cancelGoal();
}

void PointPlanner::backBasePoint() {
    xyGoalTolerance.d(0.1);
    yawGoalTolerance.d(0.1);
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
    share_move_base->sendGoal(goal, &doneCd, &activeCd, &feedbackCb);
}
