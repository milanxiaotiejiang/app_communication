//
// Created by lijiang on 2022/1/4.
//

#ifndef APP_COMMUNICATION_MESSAGEBUSMANAGER_H
#define APP_COMMUNICATION_MESSAGEBUSMANAGER_H

#include "tool/DangerousThreadPool.h"
#include "tool/CThread.h"
#include "sys/socket.h"
#include <sys/types.h>
#include <arpa/inet.h>

#include "glog/logging.h"

#include "net/MessageBus.h"

class MessageBusManager {

private:
    MessageBusManager() {
        messageBus = new MessageBus();
    };

    MessageBus *messageBus = nullptr;

public:
    static auto &instance() {
        static MessageBusManager obj;
        return obj;
    }

    MessageBus *getMessageBus() const;
};


#endif //APP_COMMUNICATION_MESSAGEBUSMANAGER_H
