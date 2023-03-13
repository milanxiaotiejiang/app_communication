//
// Created by admin1 on 22-11-25.
//

#ifndef APP_COMMUNICATION_NODEWORKMODEMANAGER_H
#define APP_COMMUNICATION_NODEWORKMODEMANAGER_H

#include <ros/ros.h>

enum WorkMode {
    MAPPING,
    SLEEPING,
    WORKING,
    UNKNOWN
};

class NodeWorkModeManager {
private:
//    ros::NodeHandle nodeHandle;
//    WorkMode workMode;
    ros::Publisher pub_node_;
public:
    static auto &instance() {
        static NodeWorkModeManager obj;
        return obj;
    }

    void initialize(ros::NodeHandle handle);

    WorkMode getWorkMode();

    void setWorkMode(WorkMode mode);

    void enterWorkMode(int enter);
};


#endif //APP_COMMUNICATION_NODEWORKMODEMANAGER_H

