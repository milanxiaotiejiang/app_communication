//
// Created by lijiang on 2021/12/20.
//

#include <thread>
#include <tool/Variable.h>
#include "sub/MapInnerSubscribe.h"
#include "manager/PublishOutManager.h"
#include <ros/time.h>
#include "simulation.h"

MapInnerSubscribe::MapInnerSubscribe(ros::NodeHandle handle) : handle(handle) {
    sub_map_inner_ = handle.subscribe("/map", 10, &MapInnerSubscribe::subscribeCallback, this);
}

MapInnerSubscribe::~MapInnerSubscribe() = default;

void MapInnerSubscribe::subscribeCallback(const nav_msgs::OccupancyGrid &map_raw) {
    if (ros::Time::now().sec - lastTime > 3) {
        lastTime = ros::Time::now().sec;
        PublishOutManager::instance().publishMap(map_raw);
    }
//    LOG(INFO) << "SIZE " << map_raw.info.width << " " << map_raw.info.height;

    Variable::get_instance()->setMapApp(map_raw);//存进Device..里面，方便全局获取
}