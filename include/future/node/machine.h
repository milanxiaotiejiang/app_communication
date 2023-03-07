//
// Created by Looper on 2023/3/7.
//

#ifndef APP_COMMUNICATION_MACHINE_H
#define APP_COMMUNICATION_MACHINE_H

#include <atomic>

namespace node {

    enum class State {
        sleep,
        work,
        map
    };

    enum class WorkState {
        normal,
        way,
        complete,
    };

    enum class MapState {
        normal,
        way,
        complete,
    };

}

#endif //APP_COMMUNICATION_MACHINE_H
