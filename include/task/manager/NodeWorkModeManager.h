//
// Created by admin1 on 22-11-25.
//

#ifndef APP_COMMUNICATION_NODEWORKMODEMANAGER_H
#define APP_COMMUNICATION_NODEWORKMODEMANAGER_H

#include <ros/ros.h>
#include <condition_variable>

enum WorkMode {
    UNKNOWN,
    SLEEPING,
    WORKING,
    MAPPING
};

const int MAXIMUM_TIME_LIMIT_FOR_QUICK_EXIT = 20;
const long MAXIMUM_LIMIT_TIME_OF_TIMEOUT = 30;
const int MAXIMUM_NUMBER_OF_FORCED_ENTRY = 3;

class NodeWorkModeManager {
private:
    ros::Publisher pub_node_;

    std::condition_variable cv;
    std::mutex cv_mut;

    WorkMode nowWorkMode();

    bool asyncWorkMode(WorkMode mode);

public:
    static auto &instance() {
        static NodeWorkModeManager obj;
        return obj;
    }

    void initialize(ros::NodeHandle handle);

    bool tryToWork();

    bool tryToMap();

    void forceToWork();

    void forceToMap();

    bool tryToSleep();

    void toSleep();

    bool enterWorkMode(int enter);

    bool forceEnterWorkMode(int enter);
};


#endif //APP_COMMUNICATION_NODEWORKMODEMANAGER_H

