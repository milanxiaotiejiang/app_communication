//
// Created by Looper on 2023/4/6.
//

#ifndef APP_COMMUNICATION_CONSUMABLE_MODEL_H
#define APP_COMMUNICATION_CONSUMABLE_MODEL_H

#include "string"

const long SWEEP_EXPECTED_DURATION = 35 * 60 * 60;
const long MOP_EXPECTED_DURATION = 35 * 60 * 60;
const long VACUUM_EXPECTED_DURATION = 35 * 60 * 60;
const long PUSH_EXPECTED_DURATION = 50 * 60 * 60;
const long AROMATHERAPY_EXPECTED_DURATION = 35 * 60 * 60;
const long DISINFECT_EXPECTED_DURATION = 35 * 60 * 60;

const int CURRENT_CONSUMABLE_VERSION = 2;

struct Consumable {
    long id;

    long sweep_expected;
    long mop_expected;
    long vacuum_expected;
    long push_expected;
    long aromatherapy_expected;
    long disinfect_expected;

    long sweep_use;
    long mop_use;
    long vacuum_use;
    long push_use;
    long aromatherapy_use;
    long disinfect_use;

    int version;

    Consumable() {}

    Consumable(long sweepExpected, long mopExpected, long vacuumExpected, long pushExpected, long aromatherapyExpected,
               long disinfectExpected, long sweepUse, long mopUse, long vacuumUse, long pushUse, long aromatherapyUse,
               long disinfectUse, int version) : sweep_expected(sweepExpected), mop_expected(mopExpected),
                                                 vacuum_expected(vacuumExpected), push_expected(pushExpected),
                                                 aromatherapy_expected(aromatherapyExpected),
                                                 disinfect_expected(disinfectExpected),
                                                 sweep_use(sweepUse), mop_use(mopUse), vacuum_use(vacuumUse),
                                                 push_use(pushUse),
                                                 aromatherapy_use(aromatherapyUse), disinfect_use(disinfectUse),
                                                 version(version) {}
};


#endif //APP_COMMUNICATION_CONSUMABLE_MODEL_H
