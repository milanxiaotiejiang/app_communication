//
// Created by Looper on 2023/7/10.
//

#ifndef APP_COMMUNICATION_ICALLBACK_H
#define APP_COMMUNICATION_ICALLBACK_H

#include <geometry_msgs/Pose.h>
#include "task/RealTask.h"

class ITaskCallback {
public:
    virtual ~ITaskCallback() = default;

    virtual void onTaskStart(const RealTask &task) = 0;

    virtual void onTaskProgress(const geometry_msgs::Pose &pose) = 0;

    virtual void onTaskEnd() = 0;

};


#endif //APP_COMMUNICATION_ICALLBACK_H
