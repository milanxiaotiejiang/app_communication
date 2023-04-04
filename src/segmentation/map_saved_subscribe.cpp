//
// Created by Looper on 2023/2/16.
//

#include "segmentation/map_saved_subscribe.h"
#include "segmentation/map_attribute.h"
#include "segmentation/SegmentationCenter.h"
#include "exploration/ExplorationCenter.h"

MapSavedSubscribe::MapSavedSubscribe(ros::NodeHandle handle) : handle(handle) {
    sub_map_saved_ = handle.subscribe("/build_map_finish", 1, &MapSavedSubscribe::subscribeCallback, this);
}

void MapSavedSubscribe::subscribeCallback(const std_msgs::Int32 &msg) {
    MapAttribute::instance().notifySaveMap();
}

MapSavedSubscribe::~MapSavedSubscribe() = default;