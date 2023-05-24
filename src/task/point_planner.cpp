//
// Created by Looper on 2022/11/5.
//

#include "task/point_planner.h"
#include "glog/logging.h"
#include "BaseThrowable.h"
#include "task/point_routine.h"
#include "simulation.h"

void PointPlanner::cpToPath(const std::vector<RealPoint> &points, replan_msgs::ReplanGoal &goal_path) {
    LOG(WARNING) << "PointPlanner send to replan path size : " << points.size();
    nav_msgs::Path path;
    path.header.frame_id = "map";
    path.header.stamp = ros::Time::now();
    for (auto item: points) {
        geometry_msgs::PoseStamped pose;
        pose.header.frame_id = "map";
        pose.header.stamp = ros::Time::now();
        pose.pose.position.x = item.realPosition.x;
        pose.pose.position.y = item.realPosition.y;
        pose.pose.position.z = item.realPosition.z;
        pose.pose.orientation.x = item.realOrientation.x;
        pose.pose.orientation.y = item.realOrientation.y;
        pose.pose.orientation.z = item.realOrientation.z;
        pose.pose.orientation.w = item.realOrientation.w;
        path.poses.push_back(pose);
    }
    goal_path.source_path = path;
}

void PointPlanner::activeCB() {
    PointRoutine::instance().pathActive();
}

void PointPlanner::feedBackCB(const replan_msgs::ReplanFeedbackConstPtr &feed_back) {
    PointRoutine::instance().pathFeedback(feed_back);
}

void
PointPlanner::doneCB(const actionlib::SimpleClientGoalState &state, const replan_msgs::ReplanResultConstPtr &result) {
    LOG(WARNING) << "PointPlanner pathCd result " << state.getText();
    PointRoutine::instance().pathDone(state);
}

void PointPlanner::initialize(ros::NodeHandle handle) {
    PointPlanner::handle = handle;
    LOG(INFO) << "PointPlanner initialize ...";
    initialize_finish = true;
}

bool PointPlanner::waitForReplanServer() {
    share_replan.reset();
    share_replan = std::make_shared<ReplanAction>("replan", true);
    return share_replan->waitForServer(ros::Duration(5));
}

void PointPlanner::resetForReplanServer() {
    share_replan.reset();
}

void PointPlanner::goToPathFirst(const RealBlock &block) {
    xyGoalTolerance.d(0.15);
    yawGoalTolerance.d(0.15);
    goToPath(block);
}

void PointPlanner::goToPath(const RealBlock &block) {
    if (!initialize_finish) {
        throw app::exception(make_error_code(error::task_planner_failed_to_start));
    }
    replan_msgs::ReplanGoal path;
    cpToPath(std::vector<RealPoint>{block.plannerPoints.begin() + block.already_step, block.plannerPoints.end()}, path);
    share_replan->sendGoal(path, &doneCB, &activeCB, &feedBackCB);
}

void PointPlanner::cancelPath() {
    share_replan->cancelGoal();
}

void PointPlanner::backBasePoint() {
    xyGoalTolerance.d(0.1);
    yawGoalTolerance.d(0.1);
    auto backBasePoint = createBackBasePoint();
    replan_msgs::ReplanGoal path;
    cpToPath(std::vector<RealPoint>{backBasePoint}, path);
    share_replan->sendGoal(path, &doneCB, &activeCB, &feedBackCB);
}

RealPoint PointPlanner::createBackBasePoint() {
    RealPoint realPoint;
    RealPosition realPosition(RETURN_POINT_X_, 0, 0);
    RealOrientation realOrientation(0, 0, 0, 1);
    realPoint.realPosition = std::move(realPosition);
    realPoint.realOrientation = std::move(realOrientation);
    return realPoint;
}
