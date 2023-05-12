//
// Created by Looper on 2023/5/12.
//

#ifndef APP_COMMUNICATION_MAINTENANCEMODE_H
#define APP_COMMUNICATION_MAINTENANCEMODE_H


class MaintenanceModeSingleton {
public:
    static MaintenanceModeSingleton &instance() {
        static MaintenanceModeSingleton instance;
        return instance;
    }

    void operateMaintenanceMode(bool open);

    bool isMaintenanceMode();

    MaintenanceModeSingleton(const MaintenanceModeSingleton &) = delete;

    MaintenanceModeSingleton &operator=(const MaintenanceModeSingleton &) = delete;

    MaintenanceModeSingleton(MaintenanceModeSingleton &&) = delete;

    MaintenanceModeSingleton &operator=(MaintenanceModeSingleton &&) = delete;

private:
    bool maintenanceMode = false;

    MaintenanceModeSingleton() = default;

    ~MaintenanceModeSingleton() = default;
};


#endif //APP_COMMUNICATION_MAINTENANCEMODE_H
