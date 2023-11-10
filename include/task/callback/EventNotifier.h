//
// Created by Looper on 2023/7/10.
//

#ifndef APP_COMMUNICATION_EVENTNOTIFIER_H
#define APP_COMMUNICATION_EVENTNOTIFIER_H

#include "ICallback.h"
#include <iostream>
#include <functional>
#include <tuple>
#include <unordered_map>
#include <typeindex>

class TaskEventNotifier {
private:
    std::shared_ptr<ITaskCallback> callback;

public:

    void setOnTaskCallback(const std::shared_ptr<ITaskCallback> &callbackPtr) {
        TaskEventNotifier::callback = callbackPtr;
    }

    void triggerTaskStart(const RealTask &task) {
        if (callback)
            callback->onTaskStart(task);
    }

    void triggerTaskProgress(const geometry_msgs::Pose &pose) {
        if (callback)
            callback->onTaskProgress(pose);
    }

    void triggerTaskEnd() {
        if (callback)
            callback->onTaskEnd();
    }

};


#endif //APP_COMMUNICATION_EVENTNOTIFIER_H
