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

//    MessageBusManager() = default;
    MessageBusManager() {
        messageBus = new MessageBus();
    };

    MessageBusManager(MessageBusManager &) = delete;

    MessageBusManager &operator=(const MessageBusManager &) = delete;

    static MessageBusManager *m_instance_ptr;

private:
    MessageBus *messageBus = nullptr;

public:
    ~MessageBusManager() = default;

    static MessageBusManager *get_instance() {
        if (m_instance_ptr == nullptr) {
            m_instance_ptr = new MessageBusManager;
        }
        return m_instance_ptr;
    }

    MessageBus *getMessageBus() const;
};


#endif //APP_COMMUNICATION_MESSAGEBUSMANAGER_H
