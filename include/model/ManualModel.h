//
// Created by mi on 2023/8/16.
//

#ifndef APP_COMMUNICATION_MANUALMODEL_H
#define APP_COMMUNICATION_MANUALMODEL_H

#include <ostream>
#include "string"
#include "nlohmann/json.hpp"

using json = nlohmann::json;

class ManualModel {
private:
    bool isCharging = false;//充电状态，可延伸为是否在基站

    bool isManualMode = false;//是否是手动模式中
    bool isUnrecoverableError = false;//是否是程序错误
    bool isUrgencyStop = false;//是否是急停

    bool isWaitTask = false;//是否等到任务，false则是有任务
    bool isContinueWork = false;//是否可触发暂停逻辑

    bool isWorkMode = false;
    bool isOffMap = false;//是否在地图外
    bool isRestrictedZone = false;//是否在禁区内
    bool isMaxPassable = false;//是否在最大可通行区域内
    bool isPlanPath = false;//是否可规划出回基站的路径

public:
    ManualModel() = default;

    void setIsCharging(bool isCharging) {
        ManualModel::isCharging = isCharging;
    }

    void setIsManualMode(bool isManualMode) {
        ManualModel::isManualMode = isManualMode;
    }

    void setIsUnrecoverableError(bool isUnrecoverableError) {
        ManualModel::isUnrecoverableError = isUnrecoverableError;
    }

    void setIsUrgencyStop(bool isUrgencyStop) {
        ManualModel::isUrgencyStop = isUrgencyStop;
    }

    void setIsWaitTask(bool isWaitTask) {
        ManualModel::isWaitTask = isWaitTask;
    }

    void setIsContinueWork(bool isContinueWork) {
        ManualModel::isContinueWork = isContinueWork;
    }

    void setIsWorkMode(bool isWorkMode) {
        ManualModel::isWorkMode = isWorkMode;
    }

    void setIsOffMap(bool isOffMap) {
        ManualModel::isOffMap = isOffMap;
    }

    void setIsRestrictedZone(bool isRestrictedZone) {
        ManualModel::isRestrictedZone = isRestrictedZone;
    }

    void setIsMaxPassable(bool isMaxPassable) {
        ManualModel::isMaxPassable = isMaxPassable;
    }

    void setIsPlanPath(bool isPlanPath) {
        ManualModel::isPlanPath = isPlanPath;
    }

    friend std::ostream &operator<<(std::ostream &os, const ManualModel &model) {
        os << "isCharging: " << model.isCharging << " isManualMode: " << model.isManualMode << " isUnrecoverableError: "
           << model.isUnrecoverableError << " isUrgencyStop: " << model.isUrgencyStop << " isWaitTask: "
           << model.isWaitTask << " isContinueWork: " << model.isContinueWork << " isWorkMode: " << model.isWorkMode
           << " isOffMap: " << model.isOffMap << " isRestrictedZone: " << model.isRestrictedZone << " isMaxPassable: "
           << model.isMaxPassable << " isPlanPath: " << model.isPlanPath;
        return os;
    }

    friend void to_json(json &j, const ManualModel &b) {
        j = json{
                {"isCharging",           b.isCharging},
                {"isManualMode",         b.isManualMode},
                {"isUnrecoverableError", b.isUnrecoverableError},
                {"isUrgencyStop",        b.isUrgencyStop},
                {"isWaitTask",           b.isWaitTask},
                {"isContinueWork",       b.isContinueWork},
                {"isWorkMode",           b.isWorkMode},
                {"isOffMap",             b.isOffMap},
                {"isRestrictedZone",     b.isRestrictedZone},
                {"isMaxPassable",        b.isMaxPassable},
                {"isPlanPath",           b.isPlanPath}
        };
    }

    friend void from_json(const json &j, ManualModel &b) {
        j.at("isCharging").get_to(b.isCharging);
        j.at("isManualMode").get_to(b.isManualMode);
        j.at("isUnrecoverableError").get_to(b.isUnrecoverableError);
        j.at("isUrgencyStop").get_to(b.isUrgencyStop);
        j.at("isWaitTask").get_to(b.isWaitTask);
        j.at("isContinueWork").get_to(b.isContinueWork);
        j.at("isWorkMode").get_to(b.isWorkMode);
        j.at("isOffMap").get_to(b.isOffMap);
        j.at("isRestrictedZone").get_to(b.isRestrictedZone);
        j.at("isMaxPassable").get_to(b.isMaxPassable);
        j.at("isPlanPath").get_to(b.isPlanPath);
    }
};


#endif //APP_COMMUNICATION_MANUALMODEL_H
