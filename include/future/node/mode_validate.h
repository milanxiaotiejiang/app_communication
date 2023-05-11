//
// Created by Looper on 2023/4/11.
//

#ifndef APP_COMMUNICATION_MODE_VALIDATE_H
#define APP_COMMUNICATION_MODE_VALIDATE_H


#include "machine.h"

class ModeValidate {
private:
    static int getMoveBaseMode();

public:
    static bool validateCartographer(node::State state);

    static bool validateMoveBase(int open);

    static bool validateMoveBaseAvailable();
};


#endif //APP_COMMUNICATION_MODE_VALIDATE_H
