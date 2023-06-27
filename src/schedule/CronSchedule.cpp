#include "schedule/CronSchedule.h"
#include <tuple>

using namespace std::chrono;
using namespace date;

namespace croncpp {


    void printSet(std::set<Months> s) {
        for (std::set<Months>::iterator it = s.begin(); it != s.end(); it++) {
            std::cout << static_cast<int>(*it) << " ";
        }
        std::cout << "end print " << " " << std::endl;
    }

    std::tuple<bool, std::chrono::system_clock::time_point>
    CronSchedule::calculate_from(const std::chrono::system_clock::time_point &from) const {
        auto curr = from;

        bool done = false;
        auto max_iterations = std::numeric_limits<uint16_t>::max();

        while (!done && --max_iterations > 0) {
            bool date_changed = false;
            year_month_day ymd = date::floor<days>(curr);

            // Add months until one of the allowed days are found, or stay at the current one.

            if (data.get_months().find(static_cast<Months>(unsigned(ymd.month()))) == data.get_months().end()) {
                auto next_month = ymd + months{1};
                sys_days s = next_month.year() / next_month.month() / 1;
                curr = s;
                date_changed = true;
            }
                // If all days are allowed (or the field is ignored via '?'), then the 'day of week' takes precedence.
            else if (data.get_day_of_month().size() != CronData::value_of(DayOfMonth::Last)) {
                // Add days until one of the allowed days are found, or stay at the current one.
                if (data.get_day_of_month().find(static_cast<DayOfMonth>(unsigned(ymd.day()))) ==
                    data.get_day_of_month().end()) {
                    sys_days s = ymd;
                    curr = s;
                    curr += days{1};
                    date_changed = true;
                }
            } else {
                //Add days until the current weekday is one of the allowed weekdays
                year_month_weekday ymw = date::floor<days>(curr);

                if (data.get_day_of_week().find(static_cast<DayOfWeek>(ymw.weekday().c_encoding())) ==
                    data.get_day_of_week().end()) {
                    sys_days s = ymd;
                    curr = s;
                    curr += days{1};
                    date_changed = true;
                }
            }

            if (!date_changed) {
                auto date_time = to_calendar_time(curr);
                if (data.get_hours().find(static_cast<Hours>(date_time.hour)) == data.get_hours().end()) {
                    curr += hours{1};
                    curr -= minutes{date_time.min};
                    curr -= seconds{date_time.sec};
                } else if (data.get_minutes().find(static_cast<Minutes >(date_time.min)) == data.get_minutes().end()) {
                    curr += minutes{1};
                    curr -= seconds{date_time.sec};
                } else if (data.get_seconds().find(static_cast<Seconds>(date_time.sec)) == data.get_seconds().end()) {
                    curr += seconds{1};
                } else {
                    done = true;
                }
            }
        }

        return std::make_tuple(max_iterations > 0, curr);
    }

    std::tuple<bool, std::chrono::system_clock::time_point>
    CronSchedule::calculate_from_end(const std::chrono::system_clock::time_point &from, int endYear) const {
        auto curr = from;

        struct tm time;
        //这里使用的system时间，可能会与clock.now有8小时时差。只是在这里比较的话应该不影响
        time.tm_year = endYear - 1900;//tm中的年份比实际年份小1900，需要减掉
        time.tm_mon = static_cast<int>(*data.get_months().begin());//
        time.tm_mday = static_cast<int>(*data.get_day_of_month().begin());
        time.tm_hour = static_cast<int>(*data.get_hours().begin());
        time.tm_min = static_cast<int>(*data.get_minutes().begin());
        time.tm_sec = static_cast<int>(*data.get_seconds().begin());
        time_t ltime_old = mktime(&time);
        //  std::cout<<time.tm_year << "  "<< time.tm_mon<<"  "<<time.tm_mday<<" "<< time.tm_hour<<"  "<<time.tm_min<<"  "<<time.tm_sec<<std::endl;
        std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
        std::time_t now_c = std::chrono::system_clock::to_time_t(now);
        std::tm now_tm = *std::localtime(&now_c);
        now_tm.tm_mon += 1;
        //   std::cout<<now_tm.tm_year << "  "<< now_tm.tm_mon<<"  "<<now_tm.tm_mday<<" "<< now_tm.tm_hour<<"  "<<now_tm.tm_min<<"  "<<now_tm.tm_sec<<std::endl;
        bool beExpired = false;
        time_t ltime_new = mktime(&now_tm);
        beExpired = ltime_new > ltime_old;
        if (!beExpired) curr += hours{1};
        //  std::cout <<"calculate_from_end:   "<< ltime_new<<"   "<<ltime_old << std::endl;
        return std::make_tuple(true, curr);
    }
}