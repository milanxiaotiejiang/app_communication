//
// Created by Looper on 2023/6/21.
//

#include "iostream"
#include "leave/auto_maintenance_mode.h"
#include "leave/ParamManager.h"
#include "time.h"
#include "chrono"
#include "glog/logging.h"

bool AutoMaintenanceModeManager::isMaintenanceMode() {
    bool isCollectDust = ParamManager::instance().getCollectDust();
    bool isAutoOil = ParamManager::instance().getAutoOil();
    if (!isCollectDust && !isAutoOil) {
        return false;
    }
    long maintenanceStartTime = ParamManager::instance().getMaintenanceStartTime();
    if (isTimeInRange(maintenanceStartTime)) {
        return false;
    }
    return true;
}

bool AutoMaintenanceModeManager::isTimeInRange(long maintenanceStartTime) {
    auto now = std::chrono::system_clock::now();
    auto now_c = std::chrono::system_clock::to_time_t(now);
    std::tm now_tm = *std::localtime(&now_c);

    int start_minutes = maintenanceStartTime;
    int end_minutes = maintenanceStartTime + 12 * 60;
    int current_minutes = now_tm.tm_hour * 60 + now_tm.tm_min;

    if (start_minutes <= end_minutes) {
        return current_minutes >= start_minutes && current_minutes < end_minutes;
    } else { // crosses midnight
        return current_minutes >= start_minutes || current_minutes < end_minutes;
    }
}

#include <catch2/catch.hpp>

//TEST_CASE() {
//    std::cout << "isTimeInRange 0 " << MaintenanceModeManager::isTimeInRange(0) << std::endl;
//    std::cout << "isTimeInRange 2 " << MaintenanceModeManager::isTimeInRange(2 * 60 + 0) << std::endl;
//    std::cout << "isTimeInRange 4 " << MaintenanceModeManager::isTimeInRange(4 * 60 + 0) << std::endl;
//    std::cout << "isTimeInRange 6 " << MaintenanceModeManager::isTimeInRange(6 * 60 + 0) << std::endl;
//    std::cout << "isTimeInRange 8 " << MaintenanceModeManager::isTimeInRange(8 * 60 + 0) << std::endl;
//    std::cout << "isTimeInRange 10 " << MaintenanceModeManager::isTimeInRange(10 * 60 + 0) << std::endl;
//    std::cout << "isTimeInRange 12 " << MaintenanceModeManager::isTimeInRange(12 * 60 + 0) << std::endl;
//    std::cout << "isTimeInRange 14 " << MaintenanceModeManager::isTimeInRange(14 * 60 + 0) << std::endl;
//    std::cout << "isTimeInRange 16 " << MaintenanceModeManager::isTimeInRange(16 * 60 + 0) << std::endl;
//    std::cout << "isTimeInRange 18 " << MaintenanceModeManager::isTimeInRange(18 * 60 + 0) << std::endl;
//    std::cout << "isTimeInRange 20 " << MaintenanceModeManager::isTimeInRange(20 * 60 + 0) << std::endl;
//    std::cout << "isTimeInRange 22 " << MaintenanceModeManager::isTimeInRange(22 * 60 + 0) << std::endl;
//    std::cout << "isTimeInRange 24 " << MaintenanceModeManager::isTimeInRange(24 * 60 + 0) << std::endl;
//}