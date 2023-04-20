//
// Created by Looper on 2023/3/6.
//

#ifndef APP_COMMUNICATION_MAP_CONTROL_H
#define APP_COMMUNICATION_MAP_CONTROL_H

#include "ros/ros.h"
#include "string"

class MapControl {
private:
    ros::ServiceClient change_map_service_client;
public:
    static auto &instance() {
        static MapControl obj;
        return obj;
    }

    bool initialize(ros::NodeHandle handle);

    bool loadInformation(const std::string &map_id);

    bool backupAndRetrieve(const std::string &map_id);

    bool backupProhibition(const std::string &map_id, bool retrieve);

    bool backupMap(const std::string &map_id, bool retrieve);

    bool changeMapServer();

};


#endif //APP_COMMUNICATION_MAP_CONTROL_H
