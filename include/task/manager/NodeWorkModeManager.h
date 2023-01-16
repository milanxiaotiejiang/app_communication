//
// Created by admin1 on 22-11-25.
//

#ifndef APP_COMMUNICATION_NODEWORKMODEMANAGER_H
#define APP_COMMUNICATION_NODEWORKMODEMANAGER_H

#include <ros/ros.h>

enum WorkMode {
    MAPPING,
    SLEEPING,
    WORKING
};

class NodeWorkModeManager {
private:
    ros::NodeHandle nodeHandle;
    WorkMode workMode;
public:
    static auto &instance() {
        static NodeWorkModeManager obj;
        return obj;
    }

    void initialize(ros::NodeHandle handle);

    WorkMode getWorkMode();

    void setWorkMode(int);
};


#endif //APP_COMMUNICATION_NODEWORKMODEMANAGER_H

