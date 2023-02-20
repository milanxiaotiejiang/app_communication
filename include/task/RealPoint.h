//
// Created by Looper on 2022/11/5.
//

#ifndef APP_COMMUNICATION_REALPOINT_H
#define APP_COMMUNICATION_REALPOINT_H

#include <utility>
#include <ostream>

#include "model/WorkStatus.h"

class RealPosition {
public:
    double x;
    double y;
    double z;

    RealPosition() {}

    RealPosition(double x, double y, double z) : x(x), y(y), z(z) {}

    friend std::ostream &operator<<(std::ostream &os, const RealPosition &position) {
        os << "x: " << position.x << " y: " << position.y << " z: " << position.z;
        return os;
    }
};

class RealOrientation {
public:
    double x;
    double y;
    double z;
    double w;

    RealOrientation(double x, double y, double z, double w) : x(x), y(y), z(z), w(w) {}

    friend std::ostream &operator<<(std::ostream &os, const RealOrientation &orientation) {
        os << "x: " << orientation.x << " y: " << orientation.y << " z: " << orientation.z << " w: " << orientation.w;
        return os;
    }
};

class RealProgress {
public:
    int currentStep;
    int totalStep;
    int currentFrequency;
    int totalFrequency;

    RealProgress(int currentStep, int totalStep, int currentFrequency, int totalFrequency) : currentStep(currentStep),
                                                                                             totalStep(totalStep),
                                                                                             currentFrequency(
                                                                                                     currentFrequency),
                                                                                             totalFrequency(
                                                                                                     totalFrequency) {}

    friend std::ostream &operator<<(std::ostream &os, const RealProgress &progress) {
        os << "totalStep: " << progress.totalStep << " totalFrequency: " << progress.totalFrequency
           << " currentStep: " << progress.currentStep << " currentFrequency: " << progress.currentFrequency;
        return os;
    }
};

class RealError {
public:
    bool arrive;
    std::string error_msg;
    int timeout = 0;

    RealError(bool arrive, const std::string &errorMsg) : arrive(arrive), error_msg(errorMsg) {}


};

class SpecialInfo {
public:
    bool clean_water_level_check_failed_{false};
    bool dirty_water_level_check_failed_{false};
    bool motor_error_{false};
};

class RealPoint {
public:
    int id{};
    std::string task_id{};
    int mode;
    bool inClean;
    RealPosition realPosition{0, 0, 0};
    RealOrientation realOrientation{0, 0, 0, 0};
    RealProgress realProgress{0, 0, 0, 0};
    WorkStatus work_status;
    RealError realError{false, ""};
    SpecialInfo specialInfo;

    RealPoint() {}

    RealPoint(int id, const std::string &taskId, int mode, WorkStatus work_status) : id(id), task_id(taskId),
                                                                                     mode(mode),
                                                                                     work_status(work_status) {}

    int getId() const {
        return id;
    }

    void setId(int id) {
        RealPoint::id = id;
    }

    const std::string &getTaskId() const {
        return task_id;
    }

    void setTaskId(const std::string &taskId) {
        task_id = taskId;
    }

    int getMode() const {
        return mode;
    }

    void setMode(int mode) {
        RealPoint::mode = mode;
    }

    bool isInClean() const {
        return inClean;
    }

    void setInClean(bool inClean) {
        RealPoint::inClean = inClean;
    }

    const RealPosition &getRealPosition() const {
        return realPosition;
    }

    void setRealPosition(const RealPosition &realPosition) {
        RealPoint::realPosition = realPosition;
    }

    const RealOrientation &getRealOrientation() const {
        return realOrientation;
    }

    void setRealOrientation(const RealOrientation &realOrientation) {
        RealPoint::realOrientation = realOrientation;
    }

    const RealProgress &getRealProgress() const {
        return realProgress;
    }

    void setRealProgress(const RealProgress &realProgress) {
        RealPoint::realProgress = realProgress;
    }

    const WorkStatus &getWorkStatus() const {
        return work_status;
    }

    void setWorkStatus(const WorkStatus &workStatus) {
        work_status = workStatus;
    }

    const RealError &getRealError() const {
        return realError;
    }

    const SpecialInfo &getSpecialInfo() const {
        return specialInfo;
    }

    void setRealError(const RealError &realError) {
        RealPoint::realError = realError;
    }

    friend std::ostream &operator<<(std::ostream &os, const RealPoint &point) {
        os << "id: " << point.id << " realProgress: " << point.realProgress;
        return os;
    }
};

#endif //APP_COMMUNICATION_REALPOINT_H
