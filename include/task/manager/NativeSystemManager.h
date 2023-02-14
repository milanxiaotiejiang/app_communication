//
// Created by Looper on 2022/11/29.
//

#ifndef APP_COMMUNICATION_NATIVESYSTEMMANAGER_H
#define APP_COMMUNICATION_NATIVESYSTEMMANAGER_H


#include "task/async_task_call.h"

class NativeSystemManager {
private:
    AsyncTaskCall *asyncTaskCall;
public:
    static auto &instance() {
        static NativeSystemManager obj;
        return obj;
    }

    void setAsyncTaskCall(AsyncTaskCall *asyncTaskCall) {
        NativeSystemManager::asyncTaskCall = asyncTaskCall;
    }

    void urgencyStop(bool isUrgencyStop);

    void lowBatteryToBackBase();

    void waterLevelToBackBase(int event);

    void urgencyStopAndCharge();

    void motorErrorEvent(int error_event);

    void hlsErrorEvent(int error_event);

    void laserErrorEvent(std::string error_event);
};

#endif //APP_COMMUNICATION_NATIVESYSTEMMANAGER_H
