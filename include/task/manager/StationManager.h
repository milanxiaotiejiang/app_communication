//
// Created by admin1 on 22-11-25.
//

#ifndef APP_COMMUNICATION_STATIONMANAGER_H
#define APP_COMMUNICATION_STATIONMANAGER_H

#include "task/async_task_call.h"

class StationManager {
private:
    StationManager() = default;

    StationManager(StationManager &) = delete;

    StationManager &operator=(const StationManager &) = delete;

public:
    ~StationManager() = default;

private:
    std::shared_ptr<AsyncTaskCall> asyncTaskCall;

    ros::Publisher pub_flag_in, pub_flag_out;
public:
    static auto &instance() {
        static StationManager obj;
        return obj;
    }

    void initialize(ros::NodeHandle handle);

    void setAsyncTaskCall(std::shared_ptr<AsyncTaskCall> asyncTaskCallPtr) {
        StationManager::asyncTaskCall = asyncTaskCallPtr;
    }

    void outStation();

    void stationOutResult(int flag);

    void backStation();

    void cancelBackStation();

    void stationInResult(int flag);
};


#endif //APP_COMMUNICATION_STATIONMANAGER_H
