#pragma once

#include <functional>
#include <chrono>
#include <utility>
#include "CronData.h"
#include "CronSchedule.h"
#include "glog/logging.h"
namespace croncpp
{
    class TaskInformation
    {
        public:
            virtual ~TaskInformation() = default;
            virtual std::chrono::system_clock::duration get_delay() const = 0;
            virtual std::string get_name() const = 0;
            virtual std::string get_schedule() const = 0;
            virtual std::string get_endschedule() const = 0;
    };

    class TaskCron : public TaskInformation
    {
        public:
            using TaskFunction = std::function<void(const TaskInformation&)>;

            TaskCron(std::string name, std::string sch, std::string end_sch, bool neverend,int endYear,const CronSchedule schedule, const CronSchedule end_schedule, TaskFunction task)
                    : name(std::move(name)), s_schedule(std::move(sch)),s_endschedule(std::move(end_sch)),never_end(neverend),end_year(endYear),schedule(std::move(schedule)), end_schedule(std::move(end_schedule)),task(std::move(task))
            {
            }
            /*TaskCron(std::string name, const CronSchedule schedule, TaskFunction task)
                    : name(std::move(name)), schedule(std::move(schedule)), task(std::move(task))
            {
            }*/

            void execute(std::chrono::system_clock::time_point now)
            {
                // Next Schedule is still the current schedule, calculate delay (actual execution - planned execution)
                delay = now - next_schedule;

                last_run = now;
                task(*this);
            }

            std::chrono::system_clock::duration get_delay() const override
            {
                return delay;
            }

            TaskCron(const TaskCron& other) = default;

            TaskCron& operator=(const TaskCron&) = default;

            bool calculate_next(std::chrono::system_clock::time_point from);

            bool operator>(const TaskCron& other) const
            {
                return next_schedule > other.next_schedule;
            }

            bool operator<(const TaskCron& other) const
            {
                return next_schedule < other.next_schedule;
            }

            bool is_expired(std::chrono::system_clock::time_point now) const;

            std::chrono::system_clock::duration
            time_until_expiry(std::chrono::system_clock::time_point now) const;

            std::string get_name() const override
            {
                return name;
            }
            std::string get_schedule() const override
            {
                return s_schedule;
            }
            std::string get_endschedule() const override
            {
                return s_endschedule;
            }

            std::string get_status(std::chrono::system_clock::time_point now) const;

        private:
            std::string name;
            std::string s_schedule;
            std::string s_endschedule;
            CronSchedule schedule;
            CronSchedule end_schedule;
            std::chrono::system_clock::time_point next_schedule;
            std::chrono::system_clock::time_point next_schedule_end;
            std::chrono::system_clock::duration delay = std::chrono::seconds(-1);
            TaskFunction task;
            bool valid = false;
            bool never_end = true;
            int end_year = 3000;
            std::chrono::system_clock::time_point last_run = std::numeric_limits<std::chrono::system_clock::time_point>::min();
    };
}

inline bool operator==(const std::string &lhs, const croncpp::TaskCron &rhs)
{
    return lhs == rhs.get_name();
}

inline bool operator==(const croncpp::TaskCron &lhs, const std::string &rhs)
{
    return lhs.get_name() == rhs;
}

inline bool operator!=(const std::string &lhs, const croncpp::TaskCron &rhs)
{
    return !(lhs == rhs);
}

inline bool operator!=(const croncpp::TaskCron &lhs, const std::string &rhs)
{
    return !(lhs == rhs);
}
