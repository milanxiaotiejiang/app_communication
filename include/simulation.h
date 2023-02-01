//
// Created by Looper on 2022/11/26.
//

#ifndef APP_COMMUNICATION_SIMULATION_H
#define APP_COMMUNICATION_SIMULATION_H

const bool isTimely = false;
const bool isTestCrash = false;

class Environment {

public:
    static auto &instance() {
        static Environment obj;
        return obj;
    }

    bool isRealEnvironment = true;
};


#endif //APP_COMMUNICATION_SIMULATION_H
