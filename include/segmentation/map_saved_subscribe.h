//
// Created by Looper on 2023/2/16.
//

#ifndef APP_COMMUNICATION_MAP_SAVED_SUBSCRIBE_H
#define APP_COMMUNICATION_MAP_SAVED_SUBSCRIBE_H

#include <ros/ros.h>
#include <std_msgs/Int32.h>

class MapSavedSubscribe {

private:
    ros::NodeHandle handle;

    ros::Subscriber sub_map_saved_;

    void subscribeCallback(const std_msgs::Int32 &msg);

public:
    MapSavedSubscribe(ros::NodeHandle handle);

    virtual ~MapSavedSubscribe();

};


#endif //APP_COMMUNICATION_MAP_SAVED_SUBSCRIBE_H
