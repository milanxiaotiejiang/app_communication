//
// Created by Looper on 2022/11/5.
//

#ifndef APP_COMMUNICATION_REALPOINT_H
#define APP_COMMUNICATION_REALPOINT_H

#include <utility>
#include <ostream>

#include "model/WorkStatus.h"

class RealPosition {
//private:
//    void _move(RealPosition &&position);
//
//    void _move(RealPosition &position);

public:
    double x;
    double y;
    double z;

    // only move
//    RealPosition(RealPosition &&position);
//
//    RealPosition &operator=(RealPosition &&position);
//
//    RealPosition(RealPosition &position);
//
//    RealPosition operator=(RealPosition &position);

    RealPosition() = default;

    RealPosition(double x, double y, double z) : x(x), y(y), z(z) {}

    friend std::ostream &operator<<(std::ostream &os, const RealPosition &position) {
        os << "x: " << position.x << " y: " << position.y << " z: " << position.z;
        return os;
    }
};

class RealOrientation {
//private:
//    void _move(RealOrientation &&orientation);
//
//    void _move(RealOrientation &orientation);

public:
    double x;
    double y;
    double z;
    double w;

    // only move
//    RealOrientation(RealOrientation &&orientation);
//
//    RealOrientation &operator=(RealOrientation &&orientation);
//
//    RealOrientation(RealOrientation &orientation);
//
//    RealOrientation operator=(RealOrientation &orientation);

    RealOrientation() = default;

    RealOrientation(double x, double y, double z, double w) : x(x), y(y), z(z), w(w) {}

    friend std::ostream &operator<<(std::ostream &os, const RealOrientation &orientation) {
        os << "x: " << orientation.x << " y: " << orientation.y << " z: " << orientation.z << " w: " << orientation.w;
        return os;
    }
};

class RealProgress {
//private:
//    void _move(RealProgress &&progress);
//
//    void _move(RealProgress &progress);

public:
    int currentStep;
    int totalStep;
    int currentFrequency;
    int totalFrequency;

    // only move
//    RealProgress(RealProgress &&progress);
//
//    RealProgress &operator=(RealProgress &&progress);
//
//    RealProgress(RealProgress &progress);
//
//    RealProgress operator=(RealProgress &progress);

    RealProgress() = default;

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
//private:
//    void _move(RealError &&error);
//
//    void _move(RealError &error);

public:
    bool arrive;
    std::string error_msg;
    int timeout = 0;

    // only move
//    RealError(RealError &&error);
//
//    RealError &operator=(RealError &&error);
//
//    RealError(RealError &error);
//
//    RealError operator=(RealError &error);

    RealError() = default;

    RealError(bool arrive, const std::string &errorMsg) : arrive(arrive), error_msg(errorMsg) {}

};

class SpecialInfo {
//private:
//    void _move(SpecialInfo &&specialInfo);
//
//    void _move(SpecialInfo &specialInfo);

public:
    bool clean_water_level_check_failed_{false};
    bool dirty_water_level_check_failed_{false};
    bool motor_error_{false};

    // only move
//    SpecialInfo(SpecialInfo &&specialInfo);
//
//    SpecialInfo &operator=(SpecialInfo &&specialInfo);
//
//    SpecialInfo(SpecialInfo &specialInfo);
//
//    SpecialInfo operator=(SpecialInfo &specialInfo);

    SpecialInfo() = default;

};

class RealPoint {
//private:
//    void _move(RealPoint &&point);
//
//    void _move(RealPoint &point);

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

    // only move
//    RealPoint(RealPoint &&point);
//
//    RealPoint &operator=(RealPoint &&point);
//
//    RealPoint(RealPoint &point);
//
//    RealPoint operator=(RealPoint &point);

    RealPoint() = default;

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

    const RealOrientation &getRealOrientation() const {
        return realOrientation;
    }

    const RealProgress &getRealProgress() const {
        return realProgress;
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

    friend std::ostream &operator<<(std::ostream &os, const RealPoint &point) {
        os << "id: " << point.id << " realProgress: " << point.realProgress;
        return os;
    }
};

//void RealPosition::_move(RealPosition &&position) {
//    this->x = std::move(position.x);
//    this->y = std::move(position.y);
//    this->z = std::move(position.z);
//}
//
//void RealPosition::_move(RealPosition &position) {
//    this->x = std::move(position.x);
//    this->y = std::move(position.y);
//    this->z = std::move(position.z);
//}
//
//RealPosition::RealPosition(RealPosition &&position) {
//    _move(std::move(position));
//}
//
//RealPosition &RealPosition::operator=(RealPosition &&position) {
//    if (this != &position)
//        _move(std::move(position));
//    return *this;
//}
//
//RealPosition::RealPosition(RealPosition &position) {
//    _move(std::move(position));
//}
//
//RealPosition RealPosition::operator=(RealPosition &position) {
//    if (this != &position)
//        _move(std::move(position));
//    return *this;
//}
//
//void RealOrientation::_move(RealOrientation &&orientation) {
//    this->x = std::move(orientation.x);
//    this->y = std::move(orientation.y);
//    this->z = std::move(orientation.z);
//    this->w = std::move(orientation.w);
//}
//
//void RealOrientation::_move(RealOrientation &orientation) {
//    this->x = std::move(orientation.x);
//    this->y = std::move(orientation.y);
//    this->z = std::move(orientation.z);
//    this->w = std::move(orientation.w);
//}
//
//RealOrientation::RealOrientation(RealOrientation &&orientation) {
//    _move(std::move(orientation));
//}
//
//RealOrientation &RealOrientation::operator=(RealOrientation &&orientation) {
//    if (this != &orientation)
//        _move(std::move(orientation));
//    return *this;
//}
//
//RealOrientation::RealOrientation(RealOrientation &orientation) {
//    _move(std::move(orientation));
//}
//
//RealOrientation RealOrientation::operator=(RealOrientation &orientation) {
//    if (this != &orientation)
//        _move(std::move(orientation));
//    return *this;
//}
//
//void RealProgress::_move(RealProgress &&progress) {
//    this->currentStep = std::move(progress.currentStep);
//    this->totalStep = std::move(progress.totalStep);
//    this->currentFrequency = std::move(progress.currentFrequency);
//    this->totalFrequency = std::move(progress.totalFrequency);
//}
//
//void RealProgress::_move(RealProgress &progress) {
//    this->currentStep = std::move(progress.currentStep);
//    this->totalStep = std::move(progress.totalStep);
//    this->currentFrequency = std::move(progress.currentFrequency);
//    this->totalFrequency = std::move(progress.totalFrequency);
//}
//
//RealProgress::RealProgress(RealProgress &&progress) {
//    _move(std::move(progress));
//}
//
//RealProgress &RealProgress::operator=(RealProgress &&progress) {
//    if (this != &progress)
//        _move(std::move(progress));
//    return *this;
//}
//
//RealProgress::RealProgress(RealProgress &progress) {
//    _move(std::move(progress));
//}
//
//RealProgress RealProgress::operator=(RealProgress &progress) {
//    if (this != &progress)
//        _move(std::move(progress));
//    return *this;
//}
//
//void RealError::_move(RealError &&error) {
//    this->arrive = std::move(error.arrive);
//    this->error_msg = std::move(error.error_msg);
//    this->timeout = std::move(error.timeout);
//}
//
//void RealError::_move(RealError &error) {
//    this->arrive = std::move(error.arrive);
//    this->error_msg = std::move(error.error_msg);
//    this->timeout = std::move(error.timeout);
//}
//
//RealError::RealError(RealError &&error) {
//    _move(std::move(error));
//}
//
//RealError &RealError::operator=(RealError &&error) {
//    if (this != &error)
//        _move(std::move(error));
//    return *this;
//}
//
//RealError::RealError(RealError &error) {
//    _move(std::move(error));
//}
//
//RealError RealError::operator=(RealError &error) {
//    if (this != &error)
//        _move(std::move(error));
//    return *this;
//}
//
//void SpecialInfo::_move(SpecialInfo &&specialInfo) {
//    this->clean_water_level_check_failed_ = std::move(specialInfo.clean_water_level_check_failed_);
//    this->dirty_water_level_check_failed_ = std::move(specialInfo.dirty_water_level_check_failed_);
//    this->motor_error_ = std::move(specialInfo.motor_error_);
//}
//
//void SpecialInfo::_move(SpecialInfo &specialInfo) {
//    this->clean_water_level_check_failed_ = std::move(specialInfo.clean_water_level_check_failed_);
//    this->dirty_water_level_check_failed_ = std::move(specialInfo.dirty_water_level_check_failed_);
//    this->motor_error_ = std::move(specialInfo.motor_error_);
//}
//
//SpecialInfo::SpecialInfo(SpecialInfo &&specialInfo) {
//    _move(std::move(specialInfo));
//}
//
//SpecialInfo &SpecialInfo::operator=(SpecialInfo &&specialInfo) {
//    if (this != &specialInfo)
//        _move(std::move(specialInfo));
//    return *this;
//}
//
//SpecialInfo::SpecialInfo(SpecialInfo &specialInfo) {
//    _move(std::move(specialInfo));
//}
//
//SpecialInfo SpecialInfo::operator=(SpecialInfo &specialInfo) {
//    if (this != &specialInfo)
//        _move(std::move(specialInfo));
//    return *this;
//}
//
//void RealPoint::_move(RealPoint &&point) {
//    this->id = std::move(point.id);
//    this->task_id = std::move(point.task_id);
//    this->mode = std::move(point.mode);
//    this->inClean = std::move(point.inClean);
//    this->realPosition = std::move(point.realPosition);
//    this->realOrientation = std::move(point.realOrientation);
//    this->realProgress = std::move(point.realProgress);
//    this->work_status = std::move(point.work_status);
//    this->realError = std::move(point.realError);
//    this->specialInfo = std::move(point.specialInfo);
//}
//
//void RealPoint::_move(RealPoint &point) {
//    this->id = std::move(point.id);
//    this->task_id = std::move(point.task_id);
//    this->mode = std::move(point.mode);
//    this->inClean = std::move(point.inClean);
//    this->realPosition = std::move(point.realPosition);
//    this->realOrientation = std::move(point.realOrientation);
//    this->realProgress = std::move(point.realProgress);
//    this->work_status = std::move(point.work_status);
//    this->realError = std::move(point.realError);
//    this->specialInfo = std::move(point.specialInfo);
//}
//
//RealPoint::RealPoint(RealPoint &&point) {
//    _move(std::move(point));
//}
//
//RealPoint &RealPoint::operator=(RealPoint &&point) {
//    if (this != &point)
//        _move(std::move(point));
//    return *this;
//}
//
//RealPoint::RealPoint(RealPoint &point) {
//    _move(std::move(point));
//}
//
//RealPoint RealPoint::operator=(RealPoint &point) {
//    if (this != &point)
//        _move(std::move(point));
//    return *this;
//}


#endif //APP_COMMUNICATION_REALPOINT_H
