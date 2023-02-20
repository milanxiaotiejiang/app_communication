//
// Created by Looper on 2023/2/16.
//

#include "segmentation/map_saved_subscribe.h"
#include "segmentation/map_attribute.h"
#include "segmentation/SegmentationCenter.h"

MapSavedSubscribe::MapSavedSubscribe(ros::NodeHandle handle) : handle(handle) {
    sub_map_saved_ = handle.subscribe("/map_saved", 1, &MapSavedSubscribe::subscribeCallback, this);
}

void MapSavedSubscribe::subscribeCallback(const std_msgs::Int16 &msg) {
    SegmentationCenter::instance().resetSegmentation();
    MapAttribute::instance().loadStation();
    MapAttribute::instance().setCreatingMap(false);
}

MapSavedSubscribe::~MapSavedSubscribe() = default;