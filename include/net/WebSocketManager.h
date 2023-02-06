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
public:
    static auto &instance() {
        static WebSocketManager obj;
        return obj;
    }

    void start();

};

void WebSocketManager::start() {

    WebClientSubscriber *webClientSubscriber = new WebClientSubscriber();
    webClientSubscriber->start();
    webClientSubscriber->detach();

//    WebClientSubscribeLocally *clientSubscribeLocally = new WebClientSubscribeLocally();
//    clientSubscribeLocally->start();
//    clientSubscribeLocally->detach();
}


#endif //APP_COMMUNICATION_WebSocketManager_H
