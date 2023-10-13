//
// Created by Looper on 2023/6/21.
//

#include "iostream"
#include "leave/auto_maintenance_mode.h"
#include "leave/ParamManager.h"
#include "time.h"
#include "chrono"
#include "simulation.h"
#include "schedule/schedule_manager.h"
#include "simulation.h"
#include "manager/PublishInnerManager.h"
#include "future/timer_call.h"
#include "task/subscribe/zoo_inner_status.h"
#include "task/manager/manual.h"

void AutoMaintenanceModeManager::run() {
    end_time_point = calculate_end_point_time();
    auto_maintenance_thread = std::thread(&AutoMaintenanceModeManager::auto_maintenance_thread_func, this);
    auto_maintenance_thread.detach();
}

[[noreturn]] void AutoMaintenanceModeManager::auto_maintenance_thread_func() {
    while (true) {

        std::unique_lock<std::mutex> lk(auto_maintenance_mutex);
        LOG_IF(INFO, DEBUG_MAINTENANCE)
                        << "自动维护下次执行时间 ： " << ScheduleManager::format_time_point(end_time_point);

        auto_maintenance_cv.wait_until(lk, end_time_point, [this]() {
            return isResetTime;
        });
        if (isResetTime) {
            isResetTime = false;
        } else {
            if (ZooInnerStatus::instance().getIsCharging()) {
                autoMaintenance();
            } else {
                back_base_thread = std::thread([this]() {
                    try {
                        ManualManager::instance().backToBase(true);
                    } catch (app::exception const &e) {
                        LOG(ERROR) << e.what();
                    } catch (const std::exception &e) {
                        LOG(ERROR) << e.what();
                    } catch (...) {
                        LOG(ERROR) << "MessageStrategy other start exception";
                    }

                    int count = 0;
                    while (count < 10) {
                        std::this_thread::sleep_for(std::chrono::minutes(1));
                        count++;
                        LOG_IF(INFO, DEBUG_MAINTENANCE) << "持续检查是否返回基站了 " << count << " ...";
                        bool inBaseStation = ZooInnerStatus::instance().getIsCharging();
                        if (inBaseStation)
                            count = 10;
                    }

                    if (ZooInnerStatus::instance().getIsCharging()) {
                        autoMaintenance();
                    } else {
                        LOG(ERROR) << "规定时间内未返回基站...";
                    }

                });
                back_base_thread.detach();
            }
        }
        end_time_point = calculate_end_point_time();
    }
}

void AutoMaintenanceModeManager::reset() {
    {
        std::unique_lock<std::mutex> lk(auto_maintenance_mutex);
        isResetTime = true;
    }
    auto_maintenance_cv.notify_one();
}

std::chrono::system_clock::time_point AutoMaintenanceModeManager::calculate_end_point_time() const {
    long maintenanceStartTime = ParamManager::instance().getMaintenanceStartTime();
    auto next_time_point = calculate_next_time(maintenanceStartTime);
    return next_time_point;
}

void AutoMaintenanceModeManager::autoMaintenance() {
    if (ParamManager::instance().getAutoOil() && ParamManager::instance().getCollectDust()) {
        LOG_IF(INFO, DEBUG_MAINTENANCE) << "autoOil and collectDust 1 ";
        async::TimerCall::instance().baseLoop()
                ->scheduleLater(std::chrono::seconds(1), []() {
                    LOG_IF(INFO, DEBUG_MAINTENANCE) << "autoOil status " << ParamManager::instance().getAutoOil();
                    if (ParamManager::instance().getAutoOil()) {
                        PublishInnerManager::instance().publishOil();
                        LOG_IF(INFO, DEBUG_MAINTENANCE) << "autoOil publish ";
                    }
                });

        async::TimerCall::instance().baseLoop()
                ->scheduleLater(std::chrono::minutes(5), []() {
                    LOG_IF(INFO, DEBUG_MAINTENANCE)
                                    << "collectDust status " << ParamManager::instance().getCollectDust();
                    if (ParamManager::instance().getCollectDust()) {
                        PublishInnerManager::instance().publishCollectDust();
                        LOG_IF(INFO, DEBUG_MAINTENANCE) << "collectDust publish ";
                    }
                });
    } else if (ParamManager::instance().getAutoOil()) {
        LOG_IF(INFO, DEBUG_MAINTENANCE) << "autoOil and collectDust 2 ";
        async::TimerCall::instance().baseLoop()
                ->scheduleLater(std::chrono::seconds(1), []() {
                    LOG_IF(INFO, DEBUG_MAINTENANCE) << "autoOil status " << ParamManager::instance().getAutoOil();
                    if (ParamManager::instance().getAutoOil()) {
                        PublishInnerManager::instance().publishOil();
                        LOG_IF(INFO, DEBUG_MAINTENANCE) << "autoOil publish ";
                    }
                });
    } else if (ParamManager::instance().getCollectDust()) {
        LOG_IF(INFO, DEBUG_MAINTENANCE) << "autoOil and collectDust 3 ";
        async::TimerCall::instance().baseLoop()
                ->scheduleLater(std::chrono::seconds(1), []() {
                    LOG_IF(INFO, DEBUG_MAINTENANCE)
                                    << "collectDust status " << ParamManager::instance().getCollectDust();
                    if (ParamManager::instance().getCollectDust()) {
                        PublishInnerManager::instance().publishCollectDust();
                        LOG_IF(INFO, DEBUG_MAINTENANCE) << "collectDust publish ";
                    }
                });
    }

    async::TimerCall::instance().baseLoop()
            ->scheduleLater(std::chrono::minutes(30), []() {
                LOG_IF(INFO, DEBUG_MAINTENANCE) << "hardware_reset ... ";
                PublishInnerManager::instance().pubHardwareReset();
            });

}

bool AutoMaintenanceModeManager::isMaintenanceMode() {
//    bool isCollectDust = ParamManager::instance().getCollectDust();
//    bool isAutoOil = ParamManager::instance().getAutoOil();
//    if (!isCollectDust && !isAutoOil) {
//        return false;
//    }
    long maintenanceStartTime = ParamManager::instance().getMaintenanceStartTime();
    return isTimeInRange(maintenanceStartTime);
}

bool AutoMaintenanceModeManager::isTimeInRange(long maintenanceStartTime) {
    auto now = std::chrono::system_clock::now();
    auto now_c = std::chrono::system_clock::to_time_t(now);
    std::tm now_tm = *std::localtime(&now_c);

    int start_minutes = maintenanceStartTime;
    int end_minutes = maintenanceStartTime + 4 * 60;
    int current_minutes = now_tm.tm_hour * 60 + now_tm.tm_min;

    if (start_minutes <= end_minutes) {
        return current_minutes >= start_minutes && current_minutes < end_minutes;
    } else { // crosses midnight
        return current_minutes >= start_minutes || current_minutes < end_minutes;
    }
}

#include <catch2/catch.hpp>

//TEST_CASE() {
//    std::cout << "isTimeInRange 0 " << AutoMaintenanceModeManager::isTimeInRange(0) << std::endl;
//    std::cout << "isTimeInRange 2 " << AutoMaintenanceModeManager::isTimeInRange(2 * 60 + 0) << std::endl;
//    std::cout << "isTimeInRange 4 " << AutoMaintenanceModeManager::isTimeInRange(4 * 60 + 0) << std::endl;
//    std::cout << "isTimeInRange 6 " << AutoMaintenanceModeManager::isTimeInRange(6 * 60 + 0) << std::endl;
//    std::cout << "isTimeInRange 8 " << AutoMaintenanceModeManager::isTimeInRange(8 * 60 + 0) << std::endl;
//    std::cout << "isTimeInRange 10 " << AutoMaintenanceModeManager::isTimeInRange(10 * 60 + 0) << std::endl;
//    std::cout << "isTimeInRange 12 " << AutoMaintenanceModeManager::isTimeInRange(12 * 60 + 0) << std::endl;
//    std::cout << "isTimeInRange 14 " << AutoMaintenanceModeManager::isTimeInRange(14 * 60 + 0) << std::endl;
//    std::cout << "isTimeInRange 16 " << AutoMaintenanceModeManager::isTimeInRange(16 * 60 + 0) << std::endl;
//    std::cout << "isTimeInRange 18 " << AutoMaintenanceModeManager::isTimeInRange(18 * 60 + 0) << std::endl;
//    std::cout << "isTimeInRange 20 " << AutoMaintenanceModeManager::isTimeInRange(20 * 60 + 0) << std::endl;
//    std::cout << "isTimeInRange 22 " << AutoMaintenanceModeManager::isTimeInRange(22 * 60 + 0) << std::endl;
//    std::cout << "isTimeInRange 24 " << AutoMaintenanceModeManager::isTimeInRange(24 * 60 + 0) << std::endl;
//}

std::chrono::system_clock::time_point AutoMaintenanceModeManager::calculate_next_time(long maintenanceStartTime) {
    int hour = maintenanceStartTime / 60;
    int minute = maintenanceStartTime % 60;

    auto now = std::chrono::system_clock::now();
    auto now_c = std::chrono::system_clock::to_time_t(now);
    std::tm next_tm = *std::localtime(&now_c);

    next_tm.tm_hour = hour;
    next_tm.tm_min = minute;
    next_tm.tm_sec = 0;

    auto next_time_c = std::mktime(&next_tm);
    auto next_time = std::chrono::system_clock::from_time_t(next_time_c);

    // 如果计算出的时间已经过去，将其向前推一天
    if (next_time <= now) {
        next_time += std::chrono::hours(24);
    }

    return next_time;
}

//TEST_CASE() {
//    std::cout << "calculate_next_time 0 " << ScheduleManager::format_time_point(
//            AutoMaintenanceModeManager::calculate_next_time(0)
//    ) << std::endl;
//    std::cout << "calculate_next_time 4 " << ScheduleManager::format_time_point(
//            AutoMaintenanceModeManager::calculate_next_time(4 * 60)
//    ) << std::endl;
//    std::cout << "calculate_next_time 8 " << ScheduleManager::format_time_point(
//            AutoMaintenanceModeManager::calculate_next_time(8 * 60)
//    ) << std::endl;
//    std::cout << "calculate_next_time 12 " << ScheduleManager::format_time_point(
//            AutoMaintenanceModeManager::calculate_next_time(12 * 60)
//    ) << std::endl;
//    std::cout << "calculate_next_time 16 " << ScheduleManager::format_time_point(
//            AutoMaintenanceModeManager::calculate_next_time(16 * 60)
//    ) << std::endl;
//    std::cout << "calculate_next_time 20 " << ScheduleManager::format_time_point(
//            AutoMaintenanceModeManager::calculate_next_time(20 * 60)
//    ) << std::endl;
//    std::cout << "calculate_next_time 23 " << ScheduleManager::format_time_point(
//            AutoMaintenanceModeManager::calculate_next_time(23 * 60)
//    ) << std::endl;
//}