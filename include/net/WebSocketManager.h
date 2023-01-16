//
// Created by lijiang on 2022/1/4.
//

#ifndef APP_COMMUNICATION_WebSocketManager_H
#define APP_COMMUNICATION_WebSocketManager_H

#include "tool/DangerousThreadPool.h"
#include "tool/CThread.h"
#include "sys/socket.h"
#include <sys/types.h>
#include <arpa/inet.h>
#include "WebClientSubscribeLocally.h"
#include "WebClientSubscriber.h"

class WebSocketManager {

    WebSocketManager() = default;

    WebSocketManager(WebSocketManager &) = delete;

    WebSocketManager &operator=(const WebSocketManager &) = delete;

    static WebSocketManager *m_instance_ptr;

public:
    ~WebSocketManager() = default;

    static WebSocketManager *get_instance() {
        if (m_instance_ptr == nullptr) {
            m_instance_ptr = new WebSocketManager;
        }
        return m_instance_ptr;
    }

    void start();

};

void WebSocketManager::start() {
//    WebServer *webServer = new WebServer();
//    webServer->start();
//    webServer->detach();

//    sleep(2);

    WebClientSubscriber *webClientSubscriber = new WebClientSubscriber();
    webClientSubscriber->start();
    webClientSubscriber->detach();

//    WebClientSubscribeLocally *clientSubscribeLocally = new WebClientSubscribeLocally();
//    clientSubscribeLocally->start();
//    clientSubscribeLocally->detach();
}


#endif //APP_COMMUNICATION_WebSocketManager_H
