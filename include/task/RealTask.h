//
// Created by Looper on 2022/11/5.
//

#ifndef APP_COMMUNICATION_REALTASK_H
#define APP_COMMUNICATION_REALTASK_H

#include "string"
#include "model/WorkStatus.h"
#include "model/Task.h"
#include "RealPoint.h"

class RealTask {
private:
    std::string id;
    int mode{};
    int rate{};
    WorkStatus work_status;
    std::vector<float> zoned;
    Combination combination;
    int combination_type;

    std::string launch_people;
    long launch_time;
    std::string time_mode;

    std::vector<RealPoint> planPoints;
    std::vector<RealPoint> realPoints;

public:
    RealTask() = default;

    RealTask(const string &id, int mode, int rate, const WorkStatus &workStatus) : id(id),
                                                                                   mode(mode),
                                                                                   rate(rate),
                                                                                   work_status(workStatus) {}

    const string &getId() const {
        return id;
    }

    void setId(const string &id) {
        RealTask::id = id;
    }

    const int &getCombinationType() const {
        return combination_type;
    }

    void setCombinationType(int type) {
        combination_type = type;
    }

    int getMode() const {
        return mode;
    }

    void setMode(int mode) {
        RealTask::mode = mode;
    }

    int getRate() const {
        return rate;
    }

    void setRate(int rate) {
        RealTask::rate = rate;
    }

    const WorkStatus &getWorkStatus() const {
        return work_status;
    }

    void setWorkStatus(const WorkStatus &workStatus) {
        work_status = workStatus;
    }

    const vector<float> &getZoned() const {
        return zoned;
    }

    void setZoned(const vector<float> &zoned) {
        RealTask::zoned = zoned;
    }

    const Combination &getCombination() const {
        return combination;
    }

    void setCombination(const Combination &combination) {
        RealTask::combination = combination;
    }

    const vector<RealPoint> &getPlanPoints() const {
        return planPoints;
    }

    void setPlanPoints(const vector<RealPoint> &planPoints) {
        RealTask::planPoints = planPoints;
    }

    const vector<RealPoint> &getRealPoints() const {
        return realPoints;
    }

    void setRealPoints(const vector<RealPoint> &realPoints) {
        RealTask::realPoints = realPoints;
    }

    const string &getLaunchPeople() const {
        return launch_people;
    }

    void setLaunchPeople(const string &launchPeople) {
        launch_people = launchPeople;
    }

    long getLaunchTime() const {
        return launch_time;
    }

    void setLaunchTime(long launchTime) {
        launch_time = launchTime;
    }

    const string &getTimeMode() const {
        return time_mode;
    }

    void setTimeMode(const string &timeMode) {
        time_mode = timeMode;
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
