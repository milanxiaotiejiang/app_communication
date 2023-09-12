//
// Created by Looper on 2022/11/5.
//

#ifndef APP_COMMUNICATION_POINT_PLANNER_H
#define APP_COMMUNICATION_POINT_PLANNER_H

#include "RealBlock.h"
#include <actionlib/client/simple_action_client.h>
#include "move_base_msgs/MoveBaseAction.h"
#include "back_charge_msgs/CoreMoveAction.h"
#include "leave/reconfigure.h"
#include <replan_msgs/ReplanAction.h>
#include <nav_msgs/Path.h>
#include "std_msgs/Int32.h"

const float RETURN_POINT_X_ = -1.3;

typedef actionlib::SimpleActionClient<replan_msgs::ReplanAction> ReplanAction;
typedef actionlib::SimpleActionClient<back_charge_msgs::CoreMoveAction> CoreMoveAction;

class PointPlanner {
private:
    PointPlanner() = default;

    PointPlanner(PointPlanner &) = delete;

    PointPlanner &operator=(const PointPlanner &) = delete;

public:
    ~PointPlanner() = default;

private:
    bool initialize_finish = false;

    ros::Subscriber subscriber;

    ros::NodeHandle handle;
    DR xyGoalTolerance = DR("/move_base/DWAPlannerROS", "xy_goal_tolerance");
    DR yawGoalTolerance = DR("/move_base/DWAPlannerROS", "yaw_goal_tolerance");

    DR globalInflationRadius = DR("/move_base/global_costmap/inflation_layer", "inflation_radius");
    DR localInflationRadius = DR("/move_base/local_costmap/inflation_layer", "inflation_radius");

    std::shared_ptr<ReplanAction> share_replan = nullptr;
    std::shared_ptr<CoreMoveAction> core_move = nullptr;

    static void cpToPath(const std::vector<RealPoint> &points, replan_msgs::ReplanGoal &goal_path,
                         int mode, bool border_track);

    static void activeCB();

    static void feedBackCB(const replan_msgs::ReplanFeedbackConstPtr &feed_back);

    static void doneCB(const actionlib::SimpleClientGoalState &state, const replan_msgs::ReplanResultConstPtr &result);


    static void coreMoveActiveCB();

    static void coreMoveFeedBackCB(const back_charge_msgs::CoreMoveFeedbackConstPtr &feed_back);

    static void coreMoveDoneCB(const actionlib::SimpleClientGoalState &state,
                               const back_charge_msgs::CoreMoveResultConstPtr &result);


    void subscribeCallback(const std_msgs::Int32 &data);

public:
    static auto &instance() {
        static PointPlanner obj;
        return obj;
    }

    void initialize(ros::NodeHandle handle);

    bool waitForReplanServer();

    void resetForReplanServer();

    bool waitForCoreMoveServer();

    void goToPathFirst(const RealBlock &block);

    void goToPath(const RealBlock &block);

    void goToPoint(const RealBlock &block);

    void cancelPath();

    void backBasePoint();

    static RealPoint createBackBasePoint();
};


#endif //APP_COMMUNICATION_POINT_PLANNER_H
