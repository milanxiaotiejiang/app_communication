//
// Created by lijiang on 2021/12/20.
//

#include <thread>
#include <tool/Variable.h>
#include "sub/MapInnerSubscribe.h"
#include "manager/PublishOutManager.h"
#include <ros/time.h>

MapInnerSubscribe::MapInnerSubscribe(ros::NodeHandle handle) : handle(handle) {
    sub_map_inner_ = handle.subscribe("/map", 10, &MapInnerSubscribe::subscribeCallback, this);
}

MapInnerSubscribe::~MapInnerSubscribe() {

}

void MapInnerSubscribe::subscribeCallback(const nav_msgs::OccupancyGrid &map_raw) {
    if (ros::Time::now().sec - lastTime > 3) {
        lastTime = ros::Time::now().sec;
        PublishOutManager::instance().publishMap(map_raw);
    }

    Variable::get_instance()->setMapApp(map_raw);//存进Device..里面，方便全局获取
}

const nav_msgs::OccupancyGrid &MapInnerSubscribe::getMapApp() const {
    return map_app;
}

void MapInnerSubscribe::setMapApp(const nav_msgs::OccupancyGrid &mapApp) {
    map_app = mapApp;
}