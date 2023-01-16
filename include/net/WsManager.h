//
// Created by zt on 2022/3/8.
//

#ifndef C_T_WSMANAGER_H
#define C_T_WSMANAGER_H

#include "model/RequestModel.h"
#include "model/RegisterOperation.h"
#include "nlohmann/json.hpp"
#include "sys/syscall.h"
#include "atomic"
#include <websocketpp/logger/levels.hpp>
#include <websocketpp/config/asio_no_tls_client.hpp>
#include <websocketpp/client.hpp>
#include "tool/CThread.h"
#include "BlockingCollection.h"
#include "pub/PubInner.h"
#include "model/Entrance.h"
#include "model/RequestData.h"

class WsManager {

    WsManager() = default;

    WsManager(WsManager &) = delete;

    WsManager &operator=(const WsManager &) = delete;

    static WsManager *m_instance_ptr;

public:
    ~WsManager() = default;

    static WsManager *get_instance() {
        if (m_instance_ptr == nullptr) {
            m_instance_ptr = new WsManager;
        }
        return m_instance_ptr;
    }

    void startWebSocket(const PubInner &inner);

    void sendData(const string &data);
};


#endif //C_T_WSMANAGER_H
