//
// Created by Looper on 2023/3/3.
//

#ifndef APP_COMMUNICATION_FEEDBACK_H
#define APP_COMMUNICATION_FEEDBACK_H

#include <condition_variable>
#include <geometry_msgs/PoseStamped.h>
#include <deque>
#include "future/async_call.h"

typedef geometry_msgs::PoseStamped::_pose_type _pose_type;

class TaskFeedback : public AsyncCall {
public:
    std::condition_variable cv;
    std::mutex cv_mut;

    std::deque<int> orderDeque;
    std::deque<_pose_type> dataDeque;

    void execute() override;

};


#endif //APP_COMMUNICATION_FEEDBACK_H
