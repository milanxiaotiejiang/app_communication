#include "schedule/TaskCron.h"
#include <iostream>

using namespace std::chrono;

namespace croncpp {

    bool TaskCron::calculate_next(std::chrono::system_clock::time_point from) {
        auto result = schedule.calculate_from(from);
        //    std::cout <<  "end_schedule################## "<< " "<<std::endl;
        auto result_end = end_schedule.calculate_from_end(from, end_year);
        //std::cout <<  "end_schedule !!!!!!!!!!!!!!!! "<< " "<<std::endl;
        // auto result_end = end_schedule.calculate_from_end(from);

        // In case the calculation fails, the task will no longer expire.
        bool ret = never_end || std::get<0>(result_end);
        valid = std::get<0>(result) && ret;
        if (valid) {
            next_schedule = std::get<1>(result);
            next_schedule_end = std::get<1>(result_end);

            // Make sure that the task is allowed to run.
            last_run = next_schedule - 10s;
        }

        return valid;
    }

    bool TaskCron::is_expired(std::chrono::system_clock::time_point now) const {
        auto tnt = time_until_expiry(now).count();

        // std::cout <<"until_expiry is: "<< tnt << std::endl;
        //LOG_IF(INFO, DEBUG_TIMER) << "until_expiry is: "<< tnt;
        return valid && now >= last_run && time_until_expiry(now) <= 5s;
    }

    std::chrono::system_clock::duration TaskCron::time_until_expiry(std::chrono::system_clock::time_point now) const {
        system_clock::duration d{};
        system_clock::duration d_end{};

        // Explicitly return 0s instead of a possibly negative duration when it has expired.
        if (now >= next_schedule) {
            d = 0s;
        } else {
            d = next_schedule - now;
        }
        //增加对截止日期的判断  Paul Wang
        if (never_end) {
            //  std::cout <<"never_end: "<< (never_end) << std::endl;
            return d;
        } else {
            //   auto result_end = end_schedule.calculate_from(now);
            //  auto next_schedule_end = std::get<1>(result_end);
            /*        std::time_t now_c = std::chrono::system_clock::to_time_t(now);
             std::tm now_tm = *std::localtime(&now_c);
             now_tm.tm_mon +=1;
          //   std::cout<<"!!!!!! "<<now_tm.tm_year << "  "<< now_tm.tm_mon<<"  "<<now_tm.tm_mday<<" "<< now_tm.tm_hour<<"  "<<now_tm.tm_min<<"  "<<now_tm.tm_sec<<std::endl;

         std::time_t now_c1 = std::chrono::system_clock::to_time_t(next_schedule_end);
             std::tm now_tm1 = *std::localtime(&now_c1);
             now_tm1.tm_mon +=1;*/
            //  std::cout<<"$$$$ "<<now_tm1.tm_year << "  "<< now_tm1.tm_mon<<"  "<<now_tm1.tm_mday<<" "<< now_tm1.tm_hour<<"  "<<now_tm1.tm_min<<"  "<<now_tm1.tm_sec<<std::endl;

            //   std::cout <<"next_schedule_end: "<< (next_schedule_end-now).count() << std::endl;
            if (now >= next_schedule_end) {
                d_end = 0s;
//                std::cout <<"11超出截止日期: "<<  std::endl;
            } else {
                d_end = next_schedule_end - now;
//                std::cout <<"not 11  超出截止日期: "<<  std::endl;
            }
            if (d_end == 0s) {
                //   std::cout <<"超出截止日期: "<<  std::endl;
                return 10s;//超出截止日期
            } else {
                //  std::cout <<"not 超出截止日期: "<< std::endl;
                return d;
            }

        }
        return d;

    }

    std::string TaskCron::get_status(std::chrono::system_clock::time_point now) const {
        std::string s = "'";
        s += get_name();
        s += "' expires in ";
        s += std::to_string(duration_cast<milliseconds>(time_until_expiry(now)).count());
        s += "ms => ";

        auto dt = CronSchedule::to_calendar_time(next_schedule);
        s += std::to_string(dt.year) + "-";
        s += std::to_string(dt.month) + "-";
        s += std::to_string(dt.day) + " ";
        s += std::to_string(dt.hour) + ":";
        s += std::to_string(dt.min) + ":";
        s += std::to_string(dt.sec);
        return s;
    }
}
