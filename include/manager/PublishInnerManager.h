//
// Created by mi on 2022/9/10.
//

#ifndef APP_COMMUNICATION_PUBLISHINNERMANAGER_H
#define APP_COMMUNICATION_PUBLISHINNERMANAGER_H

#include "pub/PubInner.h"

class PublishInnerManager {
private:
    PubInner *pubInner = nullptr;
public:
    static auto &instance() {
        static PublishInnerManager obj;
        return obj;
    }

    PubInner *getPubInner() const {
        return pubInner;
    }

    void setPubInner(PubInner *pubInner) {
        PublishInnerManager::pubInner = pubInner;
    }
};

#endif //APP_COMMUNICATION_PUBLISHINNERMANAGER_H
