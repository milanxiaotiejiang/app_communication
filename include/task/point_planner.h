//
// Created by Looper on 2022/11/5.
//

#ifndef APP_COMMUNICATION_POINT_PLANNER_H
#define APP_COMMUNICATION_POINT_PLANNER_H

#include "RealPoint.h"
#include <actionlib/client/simple_action_client.h>
#include "move_base_msgs/MoveBaseAction.h"

class PointPlanner {
private:
    bool initialize_finish = false;

    actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> *move_base;

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

    void initialize();

    void gotoPlannerPoint(const RealPoint &realPoint);

    void cancelGoal();

    void backBasePoint();
};


#endif //APP_COMMUNICATION_POINT_PLANNER_H
