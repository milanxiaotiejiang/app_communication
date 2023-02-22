//
// Created by Looper on 2023/2/22.
//

#include "model/ParamVo.h"

ParamVo::ParamVo() {}

ParamVo::ParamVo(int tof, bool silver, int speed) : tof(tof), silver(silver), speed(speed) {}

int ParamVo::getTof() const {
    return tof;
}

void ParamVo::setTof(int tof) {
    ParamVo::tof = tof;
}

bool ParamVo::isSilver() const {
    return silver;
}

void ParamVo::setSilver(bool silver) {
    ParamVo::silver = silver;
}

int ParamVo::getSpeed() const {
    return speed;
}

void ParamVo::setSpeed(int speed) {
    ParamVo::speed = speed;
}
