//
// Created by mi on 2022/8/23.
//

#include "model/KnobStatus.h"

KnobControl::KnobControl() {}

KnobStatus::KnobStatus() {}

const KnobControl &KnobStatus::getControl() const {
    return control;
}

void KnobStatus::setControl(const KnobControl &control) {
    KnobStatus::control = control;
}

bool KnobStatus::isAvailable1() const {
    return isAvailable;
}

void KnobStatus::setIsAvailable(bool isAvailable) {
    KnobStatus::isAvailable = isAvailable;
}
