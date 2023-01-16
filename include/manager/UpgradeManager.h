//
// Created by mi on 2022/8/1.
//

#ifndef APP_COMMUNICATION_UPGRADEMANAGER_H
#define APP_COMMUNICATION_UPGRADEMANAGER_H

#include "model/Combination.h"
#include "model/UpgradeModel.h"
#include "model/CleanHistory.h"
#include <tool/base64.h>
#include <ros/package.h>
#include <nlohmann/json.hpp>
#include "glog/logging.h"
#include "tool/write_file.hpp"
#include "model/TimerInfo.h"

using json = nlohmann::json;

class UpgradeManager {
public:
    static auto &instance() {
        static UpgradeManager obj;
        return obj;
    }

    static void updateCombinationPrincipal();

    static void updateViewPartPrincipal();

    static void updateCleanHistoryPrincipal();

    static void updateCombinationBase64();

    static void updateViewPartBase64();

    static void updateCleanHistoryBase64();

    static void updateTimeInfoBase64();

    static void updateTeachPoint();

    static void updateCombinationPrincipalWork();

    void checkWhetherFileExists();

    void createFile(const string &dataBasePath, const string &currentFilePath) const;

    void removePolygonViewPart();

    void removeCombination(vector<string> keys);
};


#endif //APP_COMMUNICATION_UPGRADEMANAGER_H
