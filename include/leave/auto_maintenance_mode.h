//
// Created by Looper on 2023/6/21.
//

#ifndef APP_COMMUNICATION_AUTO_MAINTENANCE_MODE_H
#define APP_COMMUNICATION_AUTO_MAINTENANCE_MODE_H

#include <mutex>
#include <condition_variable>
#include <thread>
#include "chrono"

class AutoMaintenanceModeManager {

private:
    std::mutex auto_maintenance_mutex;
    std::condition_variable auto_maintenance_cv;

    std::thread auto_maintenance_thread;
    std::thread back_base_thread;

    void auto_maintenance_thread_func();

    std::chrono::system_clock::time_point end_time_point;

    bool isResetTime;

    void autoMaintenance();

public:
    static auto &instance() {
        static AutoMaintenanceModeManager obj;
        return obj;
    }

    void run();

    void reset();

    std::chrono::system_clock::time_point calculate_end_point_time() const;

    static bool isMaintenanceMode();

    static bool isTimeInRange(long maintenanceStartTime);

    static std::chrono::system_clock::time_point calculate_next_time(long maintenanceStartTime);
};


#endif //APP_COMMUNICATION_AUTO_MAINTENANCE_MODE_H
