//
// Created by lijiang on 2022/1/4.
//

#ifndef APP_COMMUNICATION_UDPMANAGER_H
#define APP_COMMUNICATION_UDPMANAGER_H

#include "tool/DangerousThreadPool.h"
#include "tool/CThread.h"
#include "sys/socket.h"
#include <sys/types.h>
#include <arpa/inet.h>

#include "glog/logging.h"

#define SERV_PORT 9099
#define INET_ADDR "255.255.255.255"

class UdpThread : public CThread {
public:
    void *run() override {

        LOG(INFO) << "UdpThread Start";

        int udpFd;
        int sendSize;
        int optval = 1;
        struct sockaddr_in sockaddrIn;
        if ((udpFd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
            LOG(WARNING) << "UDP Socket Fail ";
            return NULL;
        }

        if (setsockopt(udpFd, SOL_SOCKET, SO_BROADCAST | SO_REUSEADDR, &optval, sizeof(int)) < 0) {
            LOG(WARNING) << "UDP setsockopt Failed!";
        }
        memset(&sockaddrIn, 0, sizeof(struct sockaddr_in));
        sockaddrIn.sin_family = AF_INET;
        sockaddrIn.sin_addr.s_addr = inet_addr(INET_ADDR);
        sockaddrIn.sin_port = htons(SERV_PORT);

        while (true) {

            if ((sendSize = sendto(udpFd, nullptr, 0, 0, (struct sockaddr *) &sockaddrIn,
                                   sizeof(struct sockaddr))) == -1) {
                printf("sendto fail, errno=%d\n", errno);
                return NULL;
            }
            // LOG(INFO) << "UDP send size " << sendSize;
            sleep(5);
        }

        LOG(INFO) << "UDP end .. ";
    }
};

class UdpManager {

    UdpManager() = default;

    UdpManager(UdpManager &) = delete;

    UdpManager &operator=(const UdpManager &) = delete;

    static UdpManager *m_instance_ptr;

    UdpThread *udpThread;
public:
    ~UdpManager() = default;

    static UdpManager *get_instance() {
        if (m_instance_ptr == nullptr) {
            m_instance_ptr = new UdpManager;
        }
        return m_instance_ptr;
    }

    void start();

    void stop();

};

void UdpManager::start() {
    udpThread = new UdpThread();
    udpThread->start();
    udpThread->detach();
}

void UdpManager::stop() {
    udpThread->stop();
}


#endif //APP_COMMUNICATION_UDPMANAGER_H
