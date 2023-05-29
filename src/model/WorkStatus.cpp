//
// Created by milan on 2021/12/15.
//

#include "model/WorkStatus.h"

WorkStatus::WorkStatus() {}

WorkStatus::~WorkStatus() = default;

WorkStatus::WorkStatus(int sweepStatus, int mopStatus, int vacuumStatus, int pushStatus, int aromatherapyStatus,
                       int disinfectStatus) : sweep_status(sweepStatus), mop_status(mopStatus),
                                              vacuum_status(vacuumStatus), push_status(pushStatus),
                                              aromatherapy_status(aromatherapyStatus),
                                              disinfect_status(disinfectStatus) {}

int WorkStatus::getSweepStatus() const {
    return sweep_status;
}

void WorkStatus::setSweepStatus(int sweepStatus) {
    sweep_status = sweepStatus;
}

int WorkStatus::getMopStatus() const {
    return mop_status;
}

void WorkStatus::setMopStatus(int mopStatus) {
    mop_status = mopStatus;
}

int WorkStatus::getVacuumStatus() const {
    return vacuum_status;
}

void WorkStatus::setVacuumStatus(int vacuumStatus) {
    vacuum_status = vacuumStatus;
}

int WorkStatus::getPushStatus() const {
    return push_status;
}

void WorkStatus::setPushStatus(int pushStatus) {
    push_status = pushStatus;
}

int WorkStatus::getAromatherapyStatus() const {
    return aromatherapy_status;
}

void WorkStatus::setAromatherapyStatus(int aromatherapyStatus) {
    aromatherapy_status = aromatherapyStatus;
}

int WorkStatus::getDisinfectStatus() const {
    return disinfect_status;
}

void WorkStatus::setDisinfectStatus(int disinfectStatus) {
    disinfect_status = disinfectStatus;
}

std::ostream &operator<<(std::ostream &os, const WorkStatus &status) {
    os << "sweep: " << status.sweep_status << " mop: " << status.mop_status << " vacuum: "
       << status.vacuum_status << " push: " << status.push_status << " aromatherapy: "
       << status.aromatherapy_status << " disinfect: " << status.disinfect_status;
    return os;
}
