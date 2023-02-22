//
// Created by admin1 on 22-11-25.
//

#ifndef APP_COMMUNICATION_MECHANISMMANAGER_H
#define APP_COMMUNICATION_MECHANISMMANAGER_H


#include "pub/PubInner.h"
#include "model/WorkStatus.h"

class MechanismManager {
public:
    static auto &instance() {
        static MechanismManager obj;
        return obj;
    }

public:
    void resetWorkStatus();

    void controlWorkStatus(const WorkStatus &workStatus);

    void forceControlWorkStatus(const WorkStatus &workStatus);

    void enterManualControl();

    void quitManualControl();

    void openHotWind();

    void closeHotWind();
};


#endif //APP_COMMUNICATION_MECHANISMMANAGER_H
