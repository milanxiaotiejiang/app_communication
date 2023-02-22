//
// Created by Looper on 2023/2/22.
//

#include "leave/ParamPo.h"

ParamPo::ParamPo() {}

ParamPo::ParamPo(int tof, bool silver, bool speed) : tof(tof), silver(silver), speed(speed) {}

int ParamPo::getTof() const {
    return tof;
}

void ParamPo::setTof(int tof) {
    ParamPo::tof = tof;
}

bool ParamPo::isSilver() const {
    return silver;
}

void ParamPo::setSilver(bool silver) {
    ParamPo::silver = silver;
}

bool ParamPo::isSpeed() const {
    return speed;
}

void ParamPo::setSpeed(bool speed) {
    ParamPo::speed = speed;
}

std::ostream &operator<<(std::ostream &os, const ParamPo &po) {
    os << "tof: " << po.tof << " silver: " << po.silver << " speed: " << po.speed;
    return os;
}
