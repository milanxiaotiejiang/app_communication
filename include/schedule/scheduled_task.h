//
// Created by Looper on 2023/4/18.
//

#ifndef APP_COMMUNICATION_SCHEDULED_TASK_H
#define APP_COMMUNICATION_SCHEDULED_TASK_H

#include "model/task.h"
#include "schedule/croncpp.h"
#include <functional>
#include <utility>

/**
 * 一个定时任务的所有信息，包括计时器、cron表达式、下次运行时间以及任务函数。
 */
struct ScheduledTask {
    TimerVo timer;
    cron::cronexpr cron_expression;
    std::chrono::system_clock::time_point next_run_time;
    std::function<void(ScheduledTask &)> taskFun;

    explicit ScheduledTask(std::function<void(ScheduledTask &)> taskFun) : taskFun(std::move(taskFun)) {}
};


#endif //APP_COMMUNICATION_SCHEDULED_TASK_H
