//
// Created by Looper on 2023/4/18.
//

#ifndef APP_COMMUNICATION_SCHEDULE_MANAGER_SINGLETON_H
#define APP_COMMUNICATION_SCHEDULE_MANAGER_SINGLETON_H

#include <ros/ros.h>
#include "schedule/schedule_manager.h"

class ScheduleManagerSingleton {
public:
    static ScheduleManagerSingleton &instance() {
        static ScheduleManagerSingleton instance;
        return instance;
    }

    ScheduleManager &getScheduleManager() {
        return scheduleManager;
    }

    void start(ros::NodeHandle handle);

    void trigger_task_update();

    ScheduleManagerSingleton(const ScheduleManagerSingleton &) = delete;

    ScheduleManagerSingleton &operator=(const ScheduleManagerSingleton &) = delete;

    ScheduleManagerSingleton(ScheduleManagerSingleton &&) = delete;

    ScheduleManagerSingleton &operator=(ScheduleManagerSingleton &&) = delete;

private:
    ScheduleManager scheduleManager;

    ScheduleManagerSingleton() = default;

    ~ScheduleManagerSingleton() = default;
};


#endif //APP_COMMUNICATION_SCHEDULE_MANAGER_SINGLETON_H
