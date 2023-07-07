//
// Created by admin1 on 22-11-25.
//

#ifndef APP_COMMUNICATION_MECHANISMMANAGER_H
#define APP_COMMUNICATION_MECHANISMMANAGER_H

#include "model/WorkStatus.h"
#include "leave/HotWindNote.h"

class MechanismManager {
private:
    MechanismManager() = default;

    MechanismManager(MechanismManager &) = delete;

    MechanismManager &operator=(const MechanismManager &) = delete;

public:
    ~MechanismManager() = default;

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

    void openKnife();

    void closeKnife();
};


#endif //APP_COMMUNICATION_MECHANISMMANAGER_H
