//
// Created by Looper on 2022/11/5.
//

#ifndef APP_COMMUNICATION_POINT_PLANNER_H
#define APP_COMMUNICATION_POINT_PLANNER_H

#include "RealPoint.h"
#include <actionlib/client/simple_action_client.h>
#include "move_base_msgs/MoveBaseAction.h"
#include "leave/reconfigure.h"

const float RETURN_POINT_X_ = -1.3;

typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> MoveBaseAction;

class PointPlanner {
private:
    bool initialize_finish = false;

    ros::NodeHandle handle;
    DR xyGoalTolerance = DR("/move_base/DWAPlannerROS", "xy_goal_tolerance");
    DR yawGoalTolerance = DR("/move_base/DWAPlannerROS", "yaw_goal_tolerance");

    std::shared_ptr<MoveBaseAction> share_move_base;

    static void point2Goal(const RealPoint &point, move_base_msgs::MoveBaseGoal &goal);

    static void doneCd(const actionlib::SimpleClientGoalState &state,
                       const move_base_msgs::MoveBaseResultConstPtr &result);

    static void activeCd();

    static void feedbackCb(const move_base_msgs::MoveBaseFeedbackConstPtr &feedback);

public:
    static auto &instance() {
        static PointPlanner obj;
        return obj;
    }

    void initialize(ros::NodeHandle handle);

    bool waitForMoveBaseServer();

    void resetForMoveBaseServer();

    void gotoPlannerPoint(const RealPoint &realPoint);

    void gotoPlannerFirstPoint(const RealPoint &realPoint);

    void cancelGoal();

    void backBasePoint();
};


#endif //APP_COMMUNICATION_POINT_PLANNER_H
