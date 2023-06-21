//
// Created by Looper on 2023/6/21.
//

#ifndef APP_COMMUNICATION_AUTO_MAINTENANCE_MODE_H
#define APP_COMMUNICATION_AUTO_MAINTENANCE_MODE_H


class AutoMaintenanceModeManager {

private:

public:
    static auto &instance() {
        static AutoMaintenanceModeManager obj;
        return obj;
    }

    bool isMaintenanceMode();

    static bool isTimeInRange(long maintenanceStartTime);
};


#endif //APP_COMMUNICATION_AUTO_MAINTENANCE_MODE_H
