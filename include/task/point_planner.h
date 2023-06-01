//
// Created by Looper on 2022/11/5.
//

#ifndef APP_COMMUNICATION_POINT_PLANNER_H
#define APP_COMMUNICATION_POINT_PLANNER_H

#include "RealBlock.h"
#include <actionlib/client/simple_action_client.h>
#include "move_base_msgs/MoveBaseAction.h"
#include "leave/reconfigure.h"
#include <replan_msgs/ReplanAction.h>
#include <nav_msgs/Path.h>

const float RETURN_POINT_X_ = -1.3;

typedef actionlib::SimpleActionClient<replan_msgs::ReplanAction> ReplanAction;

class PointPlanner {
private:
    bool initialize_finish = false;

    ros::NodeHandle handle;
    DR xyGoalTolerance = DR("/move_base/DWAPlannerROS", "xy_goal_tolerance");
    DR yawGoalTolerance = DR("/move_base/DWAPlannerROS", "yaw_goal_tolerance");

    DR globalInflationRadius = DR("/move_base/global_costmap/inflation_layer", "inflation_radius");
    DR localInflationRadius = DR("/move_base/local_costmap/inflation_layer", "inflation_radius");

    void setInscribedRadius(double inscribed_radius) {
        ros::NodeHandle nh;
        nh.setParam("/move_base/global_costmap/inflation_layer/inscribed_radius",
                    inscribed_radius);
        nh.setParam("/move_base/local_costmap/inflation_layer/inscribed_radius",
                    inscribed_radius);
    }

    std::shared_ptr<ReplanAction> share_replan;

    static void cpToPath(const std::vector<RealPoint> &points, replan_msgs::ReplanGoal &goal_path,
                         int mode, bool border_track);

    static void activeCB();

    static void feedBackCB(const replan_msgs::ReplanFeedbackConstPtr &feed_back);

    static void doneCB(const actionlib::SimpleClientGoalState &state, const replan_msgs::ReplanResultConstPtr &result);

public:
    static auto &instance() {
        static PointPlanner obj;
        return obj;
    }

    void initialize(ros::NodeHandle handle);

    bool waitForReplanServer();

    void resetForReplanServer();

    void goToPathFirst(const RealBlock &block);

    void goToPath(const RealBlock &block);

    void cancelPath();

    void backBasePoint();

    static RealPoint createBackBasePoint();
};


#endif //APP_COMMUNICATION_POINT_PLANNER_H
