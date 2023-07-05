//
// Created by Looper on 2023/4/10.
//

#ifndef APP_COMMUNICATION_AISERVERMANAGER_H
#define APP_COMMUNICATION_AISERVERMANAGER_H

class AiServerManager {
private:
    AiServerManager() = default;

    AiServerManager(AiServerManager &) = delete;

    AiServerManager &operator=(const AiServerManager &) = delete;

public:
    ~AiServerManager() = default;

public:
    static auto &instance() {
        static AiServerManager obj;
        return obj;
    }

    static void startWebSocket();

    static void stopWebSocket();

};


#endif //APP_COMMUNICATION_AISERVERMANAGER_H
