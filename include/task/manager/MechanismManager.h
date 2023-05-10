//
// Created by admin1 on 22-11-25.
//

#ifndef APP_COMMUNICATION_MECHANISMMANAGER_H
#define APP_COMMUNICATION_MECHANISMMANAGER_H

#include "model/WorkStatus.h"

class MechanismManager {
private:
    long time_hot_wind_milliseconds;
    bool maintenanceMode;

    MechanismManager();

public:
    static auto &instance() {
        static MechanismManager obj;
        return obj;
    }

public:
    void resetWorkStatus();

    void resetBelowWorkStatus();

    void controlWorkStatus(const WorkStatus &workStatus, bool knife);

    void forceControlWorkStatus(const WorkStatus &workStatus, bool knife);

    void enterManualControl();

    void quitManualControl();

    void openHotWind();

    void closeHotWind(bool force);

    bool isHotWind();

    void openKnife();

    void closeKnife();

    void operateMaintenanceMode(bool open);

    bool isMaintenanceMode();
};


#endif //APP_COMMUNICATION_MECHANISMMANAGER_H
