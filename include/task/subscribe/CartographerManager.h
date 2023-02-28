//
// Created by Looper on 2023/2/28.
//

#ifndef APP_COMMUNICATION_CARTOGRAPHERMANAGER_H
#define APP_COMMUNICATION_CARTOGRAPHERMANAGER_H

#include "task/async_task_call.h"

class CartographerManager {
private:
    AsyncTaskCall *asyncTaskCall;
    ros::Publisher pub_cartographer;
public:
    static auto &instance() {
        static CartographerManager obj;
        return obj;
    }

    void initialize(ros::NodeHandle handle);

    void setAsyncTaskCall(AsyncTaskCall *asyncTaskCall) {
        CartographerManager::asyncTaskCall = asyncTaskCall;
    }

    void pubCover();

    void coverResult();
};


#endif //APP_COMMUNICATION_CARTOGRAPHERMANAGER_H
