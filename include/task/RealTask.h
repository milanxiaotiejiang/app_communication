//
// Created by Looper on 2022/11/5.
//

#ifndef APP_COMMUNICATION_REALTASK_H
#define APP_COMMUNICATION_REALTASK_H

#include "string"
#include "model/WorkStatus.h"
#include "model/Task.h"
#include "RealPoint.h"
#include "model/RoomVo.h"

class RealTask {
private:
    std::string id;

    bool renew;

    std::string map_id;
    long task_id;
    std::string name;
    int rate;
    int mode;

    WorkStatus work_status;

    bool principal;

    std::vector<std::vector<PointVo>> zoned;
    bool partition;
    std::vector<int> subregions;
    bool knife;

    std::vector<float> zoned0;
    Combination combination;
    int combination_type;
    std::string time_mode;

    std::string source;
    std::string launch_people;
    long launch_time;

    std::vector<RealPoint> planPoints;
    std::vector<RealPoint> realPoints;

public:
    RealTask() = default;

    const std::string &getId() const {
        return id;
    }

    void setId(const std::string &id) {
        RealTask::id = id;
    }

    bool isRenew() const {
        return renew;
    }

    void setRenew(bool renew) {
        RealTask::renew = renew;
    }

    const std::string &getMapId() const {
        return map_id;
    }

    void setMapId(const std::string &mapId) {
        map_id = mapId;
    }

    long getTaskId() const {
        return task_id;
    }

    void setTaskId(long taskId) {
        task_id = taskId;
    }

    const std::string &getName() const {
        return name;
    }

    void setName(const std::string &name) {
        RealTask::name = name;
    }

    int getRate() const {
        return rate;
    }

    void setRate(int rate) {
        RealTask::rate = rate;
    }

    int getMode() const {
        return mode;
    }

    void setMode(int mode) {
        RealTask::mode = mode;
    }

    const WorkStatus &getWorkStatus() const {
        return work_status;
    }

    void setWorkStatus(const WorkStatus &workStatus) {
        work_status = workStatus;
    }

    bool isPrincipal() const {
        return principal;
    }

    void setPrincipal(bool principal) {
        RealTask::principal = principal;
    }

    const std::vector<std::vector<PointVo>> &getZoned() const {
        return zoned;
    }

    void setZoned(const std::vector<std::vector<PointVo>> &zoned) {
        RealTask::zoned = zoned;
    }

    bool isPartition() const {
        return partition;
    }

    void setPartition(bool partition) {
        RealTask::partition = partition;
    }

    const std::vector<int> &getSubregions() const {
        return subregions;
    }

    void setSubregions(const std::vector<int> &subregions) {
        RealTask::subregions = subregions;
    }

    bool isKnife() const {
        return knife;
    }

    void setKnife(bool knife) {
        RealTask::knife = knife;
    }

    const std::vector<float> &getZoned0() const {
        return zoned0;
    }

    void setZoned0(const std::vector<float> &zoned0) {
        RealTask::zoned0 = zoned0;
    }

    const Combination &getCombination() const {
        return combination;
    }

    void setCombination(const Combination &combination) {
        RealTask::combination = combination;
    }

    int getCombinationType() const {
        return combination_type;
    }

    void setCombinationType(int combinationType) {
        combination_type = combinationType;
    }

    const std::string &getTimeMode() const {
        return time_mode;
    }

    void setTimeMode(const std::string &timeMode) {
        time_mode = timeMode;
    }

    const std::string &getSource() const {
        return source;
    }

    void setSource(const std::string &source) {
        RealTask::source = source;
    }

    const std::string &getLaunchPeople() const {
        return launch_people;
    }

    void setLaunchPeople(const std::string &launchPeople) {
        launch_people = launchPeople;
    }

    long getLaunchTime() const {
        return launch_time;
    }

    void setLaunchTime(long launchTime) {
        launch_time = launchTime;
    }

    const std::vector<RealPoint> &getPlanPoints() const {
        return planPoints;
    }

    void setPlanPoints(const std::vector<RealPoint> &planPoints) {
        RealTask::planPoints = planPoints;
    }

    const std::vector<RealPoint> &getRealPoints() const {
        return realPoints;
    }

    void setRealPoints(const std::vector<RealPoint> &realPoints) {
        RealTask::realPoints = realPoints;
    }

    void assignmentPoint(RealPoint &realPoint, int pointId) {
        realPoint = RealPoint(pointId, id, mode, work_status);
    }

    void changeArrivalStatus(const RealPoint &point) {
        bool isContains;
        for (auto realPoint: realPoints) {
            if (realPoint.getId() == point.getId()) {
                isContains = true;
                if (realPoint.realError.arrive) {
                    break;
                } else {
                    if (point.realError.arrive) {
                        realPoint.realError.arrive = point.realError.arrive;
                    }
                }
            }
        }
        if (!isContains) {
            realPoints.push_back(point);
        }

    }

};

#endif //APP_COMMUNICATION_REALTASK_H
