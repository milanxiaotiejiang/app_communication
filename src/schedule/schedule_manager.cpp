//
// Created by Looper on 2023/4/17.
//

#include "schedule/schedule_manager.h"

#include "schedule/CronTest.h"

#include <iostream>
#include <sstream>
#include <iomanip>
#include <chrono>
#include <utility>
#include <vector>

#include "db/task_data_base.h"
#include "db/segmentation_data_base.h"
#include "schedule/schedule_manager_singleton.h"

#include "task/TaskCenter.h"
#include "task/manager/manual.h"

/**
croncpp库是一个简单的C++库，用于解析和处理CRON表达式。它的主要功能是根据CRON表达式计算下一次任务执行的时间。以下是croncpp库提供的主要方法和类型：
cron::cronexpr：这是一个表示CRON表达式的类。

cron::make_cron(const std::string&)：这个函数接受一个表示CRON表达式的字符串，返回一个cron::cronexpr对象。如果给定的字符串不是有效的CRON表达式，这个函数将抛出一个cron::bad_cronexpr异常。

cron::cron_next(const cron::cronexpr&, const std::chrono::system_clock::time_point&)：
 这个函数接受一个cron::cronexpr对象和一个表示当前时间的std::chrono::system_clock::time_point对象。它返回一个std::chrono::system_clock::time_point对象，表示下一次任务执行的时间。

cron::cron_next(const cron::cronexpr&, const std::chrono::system_clock::time_point&, const std::chrono::system_clock::time_point&)：
 这个函数与上面的cron::cron_next类似，但它接受一个额外的参数std::chrono::system_clock::time_point，表示任务的结束时间。如果下一次执行任务的时间超过了结束时间，这个函数将返回一个特殊的时间点，表示任务不再执行。

cron::bad_cronexpr：这是一个从std::runtime_error派生的异常类，用于表示无效的CRON表达式。
 当cron::make_cron函数遇到无效的CRON表达式时，它将抛出此异常。

croncpp库相对简单，主要用于处理CRON表达式和计算下一次任务执行的时间。要使用这个库执行定时任务，您需要自己实现任务调度和执行的逻辑，如示例代码所示。
*/

ScheduledTask ScheduleManager::buildTask(TimerVo timer, cron::cronexpr cron_expression,
                                         std::chrono::system_clock::time_point next_run_time,
                                         const std::function<void(ScheduledTask &)> &taskFun) {
    ScheduledTask scheduledTask(taskFun);
    scheduledTask.timer = std::move(timer);
    scheduledTask.cron_expression = std::move(cron_expression);
    scheduledTask.next_run_time = next_run_time;
    return scheduledTask;
}

/**
 * 在这个函数中，您可以根据需要动态更新任务列表
 */
void ScheduleManager::update_task_schedule() {
    MapPo map = SegmentationDataBase::instance().getDbMap();

    std::time_t now = std::time(nullptr);
    std::tm *ltm = std::localtime(&now);
    int currentYear = 1900 + ltm->tm_year;
    int currentMon = 1 + ltm->tm_mon;
    int currentDay = ltm->tm_mday;
    LOG(INFO) << "update_task_schedule "
              << "  年: " << currentYear
              << "  月: " << currentMon
              << "  日: " << currentDay
              << "  地图ID: " << map.id;

    const std::vector<TimerVo> &timerList = TaskDataBase::instance().loadTimerFoMap(map.id);

    std::vector<TimerVo> availableTimer;

    for (const auto &timer: timerList) {
        if (!timer.isExecute()) {
            continue;
        }

        if (!timer.isNever() && std::chrono::system_clock::now() > get_end_time(timer)) {
            continue;
        }

        if (timer.isSkip()) {
            continue;
        }

        availableTimer.push_back(timer);
    }

    tasks.clear();

    for (const auto &timer: availableTimer) {
        try {
            cron::cronexpr cron_expression = cron::make_cron("0 " + timer.getTimerRule());
            auto next_run_time = cron::cron_next(cron_expression, std::chrono::system_clock::now());
            ScheduledTask scheduledTask = buildTask(
                    timer, cron_expression, next_run_time, [this](ScheduledTask &task) {
                        this->handleTask(task);
                    }
            );
            LOG(INFO) << "定时名称 ： " << timer.getTimerName() << " ， 定时规则 ： " << timer.getTimerRule()
                      << " ， 下次执行时间 ： " << format_time_point(scheduledTask.next_run_time);
            tasks.push_back(scheduledTask);

        } catch (const cron::bad_cronexpr &ex) {
            std::cerr << "Invalid cron expression: " << ex.what() << std::endl;
        }
    }
}

/**
 * 当一个线程正在访问或修改tasks时，其他线程必须等待直到互斥锁被解锁。这可以防止多个线程同时访问和修改tasks，从而避免数据竞争。
 * 当一个线程需要等待任务更新通知时（例如，在 task_update_thread_func 中），它会解锁互斥锁，以便其他线程可以访问tasks。
 * 这通过 std::condition_variable::wait 方法实现，它在等待期间自动解锁互斥锁。当任务更新通知到达时，wait方法返回，并在返回之前自动重新锁定互斥锁。
 */

void ScheduleManager::task_update_thread_func() {
    while (true) {
        {
            std::unique_lock<std::mutex> lock(task_mutex);
            // 等待其他地方发出通知
            task_update_cv.wait(lock);

            // 收到通知，更新任务列表
            update_task_schedule();
        }

        std::this_thread::sleep_for(std::chrono::seconds(10));
    }
}

void ScheduleManager::task_loop_thread_func() {
    while (true) {
        auto now = std::chrono::system_clock::now();

        {
            std::unique_lock<std::mutex> lock(task_mutex);

            //使用迭代器遍历tasks向量，以便在循环过程中删除元素
            for (auto task_iter = tasks.begin(); task_iter != tasks.end();) {
                auto &task = *task_iter;
                if (now >= task.next_run_time) {
                    //判断任务是否已过期
                    if (!task.timer.isNever() && task.next_run_time > get_end_time(task.timer)) {
                        //如果任务已过期，将其从tasks向量中删除
                        task_iter = tasks.erase(task_iter);
                    } else {
                        //执行任务
                        task.taskFun(task);
                        //更新任务的下次运行时间
                        task.next_run_time = cron::cron_next(task.cron_expression, now);
                        LOG(INFO) << "定时名称 ： " << task.timer.getTimerName() << " ， 定时规则 ： " << task.timer.getTimerRule()
                                  << " ， 下次执行时间 ： " << format_time_point(task.next_run_time);
                        //继续遍历下一个任务
                        ++task_iter;
                    }
                } else {
                    // 继续遍历下一个任务
                    ++task_iter;
                }
            }
        }

        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

void ScheduleManager::handleTask(const ScheduledTask &scheduledTask) {
    if (ManualManager::instance().taskRunning()) {
        LOG(INFO) << "当前有任务在执行，定时清扫被取消";
    } else {
        try {
            long taskId = scheduledTask.timer.getTaskId();
            int rate = scheduledTask.timer.getRate();
            TaskCenter::instance().performTask(taskId, TaskSource::Self, rate);
        } catch (app::exception const &e) {
            LOG(ERROR) << "定时 ： " << e.what();
        } catch (const std::exception &e) {
            LOG(ERROR) << "定时 ： " << e.what();
        } catch (...) {
            LOG(ERROR) << "定时未知错误";
        }
    }

}

ScheduleManager::ScheduleManager() {
    update_task_schedule();
    task_update_thread = std::thread(&ScheduleManager::task_update_thread_func, this);
    task_loop_thread = std::thread(&ScheduleManager::task_loop_thread_func, this);
}

void ScheduleManager::run() {
    task_update_thread.detach();
    task_loop_thread.detach();
}

void ScheduleManager::trigger_task_update() {
    // 在这个函数中，您可以触发任务列表的更新
    task_update_cv.notify_one();
}

std::string ScheduleManager::format_time_point(const std::chrono::system_clock::time_point &time_point) {
    std::time_t tt = std::chrono::system_clock::to_time_t(time_point);
    std::tm tm = *std::localtime(&tt);

    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}

std::chrono::system_clock::time_point ScheduleManager::get_end_time(const TimerVo &timer) {
    std::tm end_time{};
    end_time.tm_year = timer.getEndYear() - 1900;
    end_time.tm_mon = timer.getEndMonth() - 1;
    end_time.tm_mday = timer.getEndDay();
    end_time.tm_hour = 0;
    end_time.tm_min = 0;
    end_time.tm_sec = 0;
    end_time.tm_isdst = -1;

    auto time_c = std::mktime(&end_time);

    time_c += 24 * 60 * 60;

    return std::chrono::system_clock::from_time_t(time_c);
}