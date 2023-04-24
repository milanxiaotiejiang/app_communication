//
// Created by Looper on 2022/11/5.
//

#ifndef APP_COMMUNICATION_POINT_ROUTINE_H
#define APP_COMMUNICATION_POINT_ROUTINE_H

#include <actionlib/client/simple_action_client.h>
#include "move_base_msgs/MoveBaseAction.h"
#include "task/async_task_call.h"
#include "task/RealTask.h"
#include <replan_msgs/ReplanAction.h>

/**
 * 点位流转
 */
class PointRoutine {
private:
    AsyncTaskCall *asyncTaskCall;
public:
    static auto &instance() {
        static PointRoutine obj;
        return obj;
    }

    void setAsyncTaskCall(AsyncTaskCall *asyncTaskCall) {
        PointRoutine::asyncTaskCall = asyncTaskCall;
    }

    void pointActive();

    void pointFeedback(geometry_msgs::Pose2D pose);

    void pointDone(const actionlib::SimpleClientGoalState &state);

    void pathActive();

    void pathFeedback(const replan_msgs::ReplanFeedbackConstPtr& pose);

    void pathDone(const actionlib::SimpleClientGoalState &state);

};


#endif //APP_COMMUNICATION_POINT_ROUTINE_H
