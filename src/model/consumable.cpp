//
// Created by Looper on 2023/4/6.
//

#include "model/consumable.h"

ConsumableVo::ConsumableVo(long sweepExpected, long mopExpected, long vacuumExpected, long pushExpected,
                           long aromatherapyExpected, long disinfectExpected, long sweepUse, long mopUse,
                           long vacuumUse, long pushUse, long aromatherapyUse, long disinfectUse) : sweep_expected(
        sweepExpected), mop_expected(mopExpected), vacuum_expected(vacuumExpected), push_expected(pushExpected),
                                                                                                    aromatherapy_expected(
                                                                                                            aromatherapyExpected),
                                                                                                    disinfect_expected(
                                                                                                            disinfectExpected),
                                                                                                    sweep_use(sweepUse),
                                                                                                    mop_use(mopUse),
                                                                                                    vacuum_use(
                                                                                                            vacuumUse),
                                                                                                    push_use(pushUse),
                                                                                                    aromatherapy_use(
                                                                                                            aromatherapyUse),
                                                                                                    disinfect_use(
                                                                                                            disinfectUse) {}

ConsumableVo::ConsumableVo() {}

long ConsumableVo::getSweepExpected() const {
    return sweep_expected;
}

void ConsumableVo::setSweepExpected(long sweepExpected) {
    sweep_expected = sweepExpected;
}

long ConsumableVo::getMopExpected() const {
    return mop_expected;
}

void ConsumableVo::setMopExpected(long mopExpected) {
    mop_expected = mopExpected;
}

long ConsumableVo::getVacuumExpected() const {
    return vacuum_expected;
}

void ConsumableVo::setVacuumExpected(long vacuumExpected) {
    vacuum_expected = vacuumExpected;
}

long ConsumableVo::getPushExpected() const {
    return push_expected;
}

void ConsumableVo::setPushExpected(long pushExpected) {
    push_expected = pushExpected;
}

long ConsumableVo::getAromatherapyExpected() const {
    return aromatherapy_expected;
}

void ConsumableVo::setAromatherapyExpected(long aromatherapyExpected) {
    aromatherapy_expected = aromatherapyExpected;
}

long ConsumableVo::getDisinfectExpected() const {
    return disinfect_expected;
}

void ConsumableVo::setDisinfectExpected(long disinfectExpected) {
    disinfect_expected = disinfectExpected;
}

long ConsumableVo::getSweepUse() const {
    return sweep_use;
}

void ConsumableVo::setSweepUse(long sweepUse) {
    sweep_use = sweepUse;
}

long ConsumableVo::getMopUse() const {
    return mop_use;
}

void ConsumableVo::setMopUse(long mopUse) {
    mop_use = mopUse;
}

long ConsumableVo::getVacuumUse() const {
    return vacuum_use;
}

void ConsumableVo::setVacuumUse(long vacuumUse) {
    vacuum_use = vacuumUse;
}

long ConsumableVo::getPushUse() const {
    return push_use;
}

void ConsumableVo::setPushUse(long pushUse) {
    push_use = pushUse;
}

long ConsumableVo::getAromatherapyUse() const {
    return aromatherapy_use;
}

void ConsumableVo::setAromatherapyUse(long aromatherapyUse) {
    aromatherapy_use = aromatherapyUse;
}

long ConsumableVo::getDisinfectUse() const {
    return disinfect_use;
}

void ConsumableVo::setDisinfectUse(long disinfectUse) {
    disinfect_use = disinfectUse;
}

ResetConsumableVo::ResetConsumableVo() {}

ResetConsumableVo::ResetConsumableVo(bool sweep, bool mop, bool vacuum, bool push, bool aromatherapy, bool disinfect)
        : sweep(sweep), mop(mop), vacuum(vacuum), push(push), aromatherapy(aromatherapy), disinfect(disinfect) {}

bool ResetConsumableVo::isSweep() const {
    return sweep;
}

void ResetConsumableVo::setSweep(bool sweep) {
    ResetConsumableVo::sweep = sweep;
}

bool ResetConsumableVo::isMop() const {
    return mop;
}

void ResetConsumableVo::setMop(bool mop) {
    ResetConsumableVo::mop = mop;
}

bool ResetConsumableVo::isVacuum() const {
    return vacuum;
}

void ResetConsumableVo::setVacuum(bool vacuum) {
    ResetConsumableVo::vacuum = vacuum;
}

bool ResetConsumableVo::isPush() const {
    return push;
}

void ResetConsumableVo::setPush(bool push) {
    ResetConsumableVo::push = push;
}

bool ResetConsumableVo::isAromatherapy() const {
    return aromatherapy;
}

void ResetConsumableVo::setAromatherapy(bool aromatherapy) {
    ResetConsumableVo::aromatherapy = aromatherapy;
}

bool ResetConsumableVo::isDisinfect() const {
    return disinfect;
}

void ResetConsumableVo::setDisinfect(bool disinfect) {
    ResetConsumableVo::disinfect = disinfect;
}
