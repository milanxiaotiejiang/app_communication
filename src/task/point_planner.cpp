//
// Created by Looper on 2022/11/5.
//

#include "task/point_planner.h"

#include <memory>
#include "simulation.h"
#include "BaseThrowable.h"
#include "task/point_routine.h"
#include "db/SqliteDataBase.h"

void PointPlanner::cpToPath(const std::vector<RealPoint> &points, replan_msgs::ReplanGoal &goal_path,
                            int mode, bool border_track) {
//    LOG(WARNING) << "PointPlanner send to replan path size : " << points.size()
//                 << "  , mode : " << mode
//                 << "  , border_track : " << border_track;
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
    goal_path.mode = mode;
    goal_path.border_track = border_track;
}

void PointPlanner::activeCB() {
    PointRoutine::instance().pathActive();
}

void PointPlanner::feedBackCB(const replan_msgs::ReplanFeedbackConstPtr &feed_back) {
    PointRoutine::instance().pathFeedback(feed_back, atomicBlockId.load());
}

void
PointPlanner::doneCB(const actionlib::SimpleClientGoalState &state, const replan_msgs::ReplanResultConstPtr &result) {
    PointRoutine::instance().pathDone(state, atomicBlockId.exchange(-1));
}

void PointPlanner::coreMoveActiveCB() {
    PointRoutine::instance().pointActive();
}

void PointPlanner::coreMoveFeedBackCB(const back_charge_msgs::CoreMoveFeedbackConstPtr &feed_back) {
    PointRoutine::instance().pointFeedback(feed_back);
}

void PointPlanner::coreMoveDoneCB(const actionlib::SimpleClientGoalState &state,
                                  const back_charge_msgs::CoreMoveResultConstPtr &result) {
//    LOG(WARNING) << "PointPlanner coreMove result : " << state.getText();
    PointRoutine::instance().pointDone(state);
}

void PointPlanner::initialize(ros::NodeHandle handle) {
    PointPlanner::handle = handle;
    LOG_IF(INFO, DEBUG_FIRING) << "PointPlanner initialize ...";

    subscriber = handle.subscribe("/move_base_crash", 1, &PointPlanner::subscribeCallback, this);
    initialize_finish = true;
}

bool PointPlanner::waitForReplanServer() {
    resetForReplanServer();
    share_replan = std::make_shared<ReplanAction>("replan", true);
    return share_replan->waitForServer(ros::Duration(5));
}

void PointPlanner::resetForReplanServer() {
    if (share_replan != nullptr)
        share_replan.reset();
}

bool PointPlanner::waitForCoreMoveServer() {
    core_move = std::make_shared<CoreMoveAction>("core_move_action", true);
    return core_move->waitForServer(ros::Duration(10));
}

void PointPlanner::setPathFirst() {
    xyGoalTolerance.d(0.15);
    yawGoalTolerance.d(0.15);
}

void PointPlanner::goToPath(const RealBlock &block) {
    if (!initialize_finish) {
        throw app::exception(make_error_code(error::task_planner_failed_to_start));
    }
    bool isLine = SqliteDataBase::TaskModeFromInt(block.mode) == TaskMode::Line;
    ros::NodeHandle nh;
    if (isLine) {
        float default_inscribed_radius = 0.22;
        float default_inflation_radius = 0.3;
        nh.setParam("/move_base/global_costmap/inflation_layer/inscribed_radius", default_inscribed_radius);
        nh.setParam("/move_base/local_costmap/inflation_layer/inscribed_radius", default_inscribed_radius);
        nh.setParam("/default_inscribed_radius", default_inscribed_radius);
        nh.setParam("/default_inflation_radius", default_inflation_radius);
        globalInflationRadius.d(default_inflation_radius);
        localInflationRadius.d(default_inflation_radius);
    } else {
        float default_inscribed_radius = 0.3;
        float default_inflation_radius = 0.5;
        nh.setParam("/move_base/global_costmap/inflation_layer/inscribed_radius", default_inscribed_radius);
        nh.setParam("/move_base/local_costmap/inflation_layer/inscribed_radius", default_inscribed_radius);
        nh.setParam("/default_inscribed_radius", default_inscribed_radius);
        nh.setParam("/default_inflation_radius", default_inflation_radius);
        globalInflationRadius.d(default_inflation_radius);
        localInflationRadius.d(default_inflation_radius);
    }

//    LOG(WARNING) << "PointPlanner block step --  current_step : " << block.current_step
//                 << "  , goal_step : " << block.goal_step
//                 << "  , current_goal : " << block.current_goal
//                 << "  , plannerPoints.size : " << block.plannerPoints.size();

    int mode = block.inClean ? replan_msgs::ReplanGoal::PATH : replan_msgs::ReplanGoal::POINT_NO_NEED_ARRIVE;
    if (block.mustArrive)
        mode = replan_msgs::ReplanGoal::POINT_MUST_ARRIVE;
    replan_msgs::ReplanGoal path;
    if (block.goal_step >= block.plannerPoints.size()) {
        cpToPath(std::vector<RealPoint>{block.plannerPoints[block.plannerPoints.size() - 1]},
                 path, mode, SqliteDataBase::TaskModeFromInt(block.mode) == TaskMode::Line);
    } else {
        cpToPath(std::vector<RealPoint>{block.plannerPoints.begin() + block.goal_step, block.plannerPoints.end()},
                 path, mode, SqliteDataBase::TaskModeFromInt(block.mode) == TaskMode::Line);
    }
    atomicBlockId.store(block.id);
    share_replan->sendGoal(path, &doneCB, &activeCB, &feedBackCB);
}

void PointPlanner::goToPoint(const RealPoint &point, bool mustArrive) {
    if (!initialize_finish) {
        throw app::exception(make_error_code(error::task_planner_failed_to_start));
    }
    if (point.core_move) {
        back_charge_msgs::CoreMoveGoal goal;
        goal.cmd = 1;
        goal.target_pose.pose.position.x = point.realPosition.x;
        goal.target_pose.pose.position.y = point.realPosition.y;
        core_move->sendGoal(goal, &coreMoveDoneCB, &coreMoveActiveCB, &coreMoveFeedBackCB);
    } else {
        replan_msgs::ReplanGoal path;
        cpToPath(std::vector<RealPoint>{point}, path,
                 mustArrive ? replan_msgs::ReplanGoal::POINT_MUST_ARRIVE
                            : replan_msgs::ReplanGoal::POINT_NO_NEED_ARRIVE,
                 false);
        share_replan->sendGoal(path, &doneCB, &activeCB, &feedBackCB);
    }
}

void PointPlanner::cancelPath() {
    share_replan->cancelGoal();
}

void PointPlanner::cancelPoint() {
    core_move->cancelGoal();
}

void PointPlanner::backBasePoint() {
    xyGoalTolerance.d(0.1);
    yawGoalTolerance.d(0.1);
    auto backBasePoint = createBackBasePoint();
    replan_msgs::ReplanGoal path;
    cpToPath(std::vector<RealPoint>{backBasePoint}, path, replan_msgs::ReplanGoal::POINT_MUST_ARRIVE, false);
    atomicBlockId.store(-1);
    share_replan->sendGoal(path, &doneCB, &activeCB, &feedBackCB);
}

RealPoint PointPlanner::createBackBasePoint() {
    RealPoint realPoint;
    float x = RETURN_POINT_X_;
    if (!Environment::instance().isRealEnvironment)
        x = RETURN_POINT_X_TEST_;
    RealPosition realPosition(x, 0, 0);
    RealOrientation realOrientation(0, 0, 0, 1);
    realPoint.realPosition = std::move(realPosition);
    realPoint.realOrientation = std::move(realOrientation);
    return realPoint;
}

void PointPlanner::subscribeCallback(const std_msgs::Int32 &data) {
    if (data.data == 1) {
        PointRoutine::instance().crash();
    }
}
