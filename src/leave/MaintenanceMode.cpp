//
// Created by Looper on 2023/5/12.
//

#include <std_msgs/Int32.h>
#include "leave/MaintenanceMode.h"
#include "leave/HotWindNote.h"
#include "manager/PublishInnerManager.h"

void MaintenanceModeSingleton::operateMaintenanceMode(bool open) {
    if (open) {

        HotWindNoteSingleton::instance().closeHotWind();

        maintenanceMode = true;

        std_msgs::Int32 data;
        data.data = 1;
//        PublishInnerManager::instance().publishMaintenanceMode(data);
    } else {

        maintenanceMode = false;

        std_msgs::Int32 data;
        data.data = 0;
//        PublishInnerManager::instance().publishMaintenanceMode(data);
    }
}

bool MaintenanceModeSingleton::isMaintenanceMode() {
    return maintenanceMode;
}
