//
// Created by lijiang on 2021/12/20.
//

#ifndef APP_COMMUNICATION_MAPINNERSUBSCRIBE_H
#define APP_COMMUNICATION_MAPINNERSUBSCRIBE_H

#include <ros/ros.h>
#include <model/WorkStatus.h>
#include "tool/OldFunction.hpp"

class MapInnerSubscribe {

private:
    ros::NodeHandle handle;
    ros::Subscriber sub_map_inner_;

    nav_msgs::OccupancyGrid map_app;

    int lastTime;
public:
    const nav_msgs::OccupancyGrid &getMapApp() const;

    void setMapApp(const nav_msgs::OccupancyGrid &mapApp);

private:

    void subscribeCallback(const nav_msgs::OccupancyGrid &map_raw);

public:
    MapInnerSubscribe(ros::NodeHandle handle);

    virtual ~MapInnerSubscribe();

};


#endif //APP_COMMUNICATION_MAPINNERSUBSCRIBE_H
