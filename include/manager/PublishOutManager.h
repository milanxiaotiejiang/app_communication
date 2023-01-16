//
// Created by mi on 2022/9/10.
//

#ifndef APP_COMMUNICATION_PUBLISHOUTMANAGER_H
#define APP_COMMUNICATION_PUBLISHOUTMANAGER_H

#include "pub/PubOut.h"

class PublishOutManager {
private:
    PubOut *pubOut = nullptr;
public:
    static auto &instance() {
        static PublishOutManager obj;
        return obj;
    }

    PubOut *getPubOut() const {
        return pubOut;
    }

    void setPubOut(PubOut *pubOut) {
        PublishOutManager::pubOut = pubOut;
    }
};


#endif //APP_COMMUNICATION_PUBLISHOUTMANAGER_H
