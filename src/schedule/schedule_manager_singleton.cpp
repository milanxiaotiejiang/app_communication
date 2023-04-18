//
// Created by Looper on 2023/4/18.
//

#include "schedule/schedule_manager_singleton.h"

void ScheduleManagerSingleton::start(ros::NodeHandle handle) {
    getScheduleManager().run();
}

void ScheduleManagerSingleton::trigger_task_update() {
    getScheduleManager().trigger_task_update();
}