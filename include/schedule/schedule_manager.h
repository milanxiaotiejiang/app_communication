//
// Created by Looper on 2023/4/17.
//

#ifndef APP_COMMUNICATION_SCHEDULE_MANAGER_H
#define APP_COMMUNICATION_SCHEDULE_MANAGER_H

#include "model/task.h"

#include <mutex>
#include <condition_variable>
#include <thread>

#include "schedule/scheduled_task.h"

class ScheduleManager {
private:

    //在访问和修改tasks时，使用std::unique_lock对象锁定互斥锁来实现的。这可以防止多个线程同时访问和修改tasks，从而避免数据竞争和不一致的情况。
    std::mutex task_mutex;
    std::vector<ScheduledTask> tasks;

    std::condition_variable task_update_cv;

    std::thread task_update_thread;
    std::thread task_loop_thread;

    /**
     * 用于创建一个ScheduledTask对象。它接收一个计时器、cron表达式、下次运行时间以及任务函数作为参数。
     */
    static ScheduledTask buildTask(TimerVo timer, cron::cronexpr cron_expression,
                                   std::chrono::system_clock::time_point next_run_time,
                                   const std::function<void(ScheduledTask &)> &taskFun);

    /**
     * 用于更新任务列表。
     * 首先，它从数据库中加载任务，然后过滤掉无效的任务。
     * 接着，它创建一个ScheduledTask对象，并将其添加到任务列表中。
     */
    void update_task_schedule();

    /**
     * 用于在一个单独的线程中处理任务列表的更新。它会等待其他地方发出通知，然后更新任务列表。
     */
    void task_update_thread_func();

    /**
     * 用于在一个单独的线程中处理任务的执行。它会遍历任务列表，检查每个任务的下次运行时间，如果到达了运行时间，就执行任务。
     */
    void task_loop_thread_func();

    void handleTask(const ScheduledTask &scheduledTask);

    static std::vector<std::string> split(const std::string &str, char delimiter);

public:
    ScheduleManager();

    /**
     * 启动任务更新线程和任务执行线程。
     */
    void run();

    /**
     * 触发任务列表的更新。当您需要更新任务列表时，可以调用这个方法。
     */
    void trigger_task_update();

    /**
     * 将一个std::chrono::system_clock::time_point对象转换为一个可读的字符串格式。
     */
    static std::string format_time_point(const std::chrono::system_clock::time_point &time_point);

    static std::chrono::system_clock::time_point get_end_time(const TimerVo &timer);

    static std::string fix_cron_expression(const std::string &cron_expression);

    static std::vector<std::time_t> cronTimePoints(const std::string &cron_expression, std::time_t endTime);

    static bool hasSameTimePoint(const std::vector<std::time_t> &originalPoints,
                                 const std::string &targetExpression, std::time_t endTime);
};

#endif //APP_COMMUNICATION_SCHEDULE_MANAGER_H
