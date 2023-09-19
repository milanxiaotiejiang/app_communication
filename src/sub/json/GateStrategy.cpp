//
// Created by io on 23-9-19.
//

#include "sub/json/GateStrategy.h"
#include "db/segmentation_data_base.h"
#include "segmentation/SegmentationCenter.h"

long AddGateStrategy::handler(GateInfo params) {
    auto segmented_map = SegmentationCenter::instance().generateMat();
    std::vector<Room> rooms;
    SegmentationCenter::instance().gateSegmentation(segmented_map, rooms, SegmentationDataBase::info2Gate(params));

    params.setId(-1);
    params.setOMapId(SegmentationDataBase::instance().getDbMap().id);
    return SegmentationDataBase::instance().addGateInfo(params);
}

std::string DeleteGateStrategy::handler(long params) {
    SegmentationDataBase::instance().deleteGateForId(params);
    return "";
}

void PurgeGateStrategy::handler() {
    SegmentationDataBase::instance().purgeGate(SegmentationDataBase::instance().getDbMap().id);
}

long ModifyGateStrategy::handler(GateInfo params) {
    auto segmented_map = SegmentationCenter::instance().generateMat();
    std::vector<Room> rooms;
    SegmentationCenter::instance().gateSegmentation(segmented_map, rooms, SegmentationDataBase::info2Gate(params));

    params.setOMapId(SegmentationDataBase::instance().getDbMap().id);
    return SegmentationDataBase::instance().modifyGateInfo(params);
}

std::vector<GateInfo> ListGateStrategy::handler(std::string params) {
    return SegmentationDataBase::instance().loadGateInfo(SegmentationDataBase::instance().getDbMap().id);
}

GateInfo QueryIdGateStrategy::handler(long params) {
    return SegmentationDataBase::instance().queryGateForId(params);
}
