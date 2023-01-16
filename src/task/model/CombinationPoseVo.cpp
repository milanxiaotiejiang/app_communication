//
// Created by Looper on 2022/12/1.
//

#include "task/model/CombinationPoseVo.h"

CombinationPoseVo::CombinationPoseVo() {}

CombinationPoseVo::CombinationPoseVo(float x, float y, float theta, int index) : x(x), y(y), theta(theta),
                                                                                 index(index) {}

float CombinationPoseVo::getX() const {
    return x;
}

void CombinationPoseVo::setX(float x) {
    CombinationPoseVo::x = x;
}

float CombinationPoseVo::getY() const {
    return y;
}

void CombinationPoseVo::setY(float y) {
    CombinationPoseVo::y = y;
}

float CombinationPoseVo::getTheta() const {
    return theta;
}

void CombinationPoseVo::setTheta(float theta) {
    CombinationPoseVo::theta = theta;
}

int CombinationPoseVo::getIndex() const {
    return index;
}

void CombinationPoseVo::setIndex(int index) {
    CombinationPoseVo::index = index;
}
