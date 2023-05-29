//
// Created by admin1 on 22-11-29.
//

#ifndef APP_COMMUNICATION_FULLPLANWORKSTATUSMANAGER_H
#define APP_COMMUNICATION_FULLPLANWORKSTATUSMANAGER_H


#include "model/WorkStatus.h"

const std::string FULL_CLEANING_MODE = "full_cleaning_mode.txt";

class FullPlanWorkStatusManager {
public:
    static auto &instance() {
        static FullPlanWorkStatusManager obj;
        return obj;
    }

    WorkStatus getFullCleanWorkStatus();

};


#endif //APP_COMMUNICATION_FULLPLANWORKSTATUSMANAGER_H
