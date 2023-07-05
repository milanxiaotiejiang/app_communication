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

#include <utility>

/**
 * 点位流转
 */
class PointRoutine {
private:
    PointRoutine() = default;

    PointRoutine(PointRoutine &) = delete;

    PointRoutine &operator=(const PointRoutine &) = delete;

public:
    ~PointRoutine() = default;

private:
    std::shared_ptr<AsyncTaskCall> asyncTaskCall;
public:
    static auto &instance() {
        static PointRoutine obj;
        return obj;
    }

    void setAsyncTaskCall(std::shared_ptr<AsyncTaskCall> asyncTaskCallPtr) {
        PointRoutine::asyncTaskCall = asyncTaskCallPtr;
    }

    void pathActive();

    void pathFeedback(const replan_msgs::ReplanFeedbackConstPtr &pose);

    void pathDone(const actionlib::SimpleClientGoalState &state);

};


#endif //APP_COMMUNICATION_POINT_ROUTINE_H
