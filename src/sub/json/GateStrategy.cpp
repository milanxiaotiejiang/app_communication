//
// Created by io on 23-9-19.
//

#include "sub/json/GateStrategy.h"
#include "db/segmentation_data_base.h"
#include "segmentation/SegmentationCenter.h"
#include "segmentation/GateComprehensive.h"

int AddGateStrategy::handler(GateInfo params) {
    auto segmented_map = SegmentationCenter::instance().generateMat();
    auto map_origin = MapAttributeSingleton::instance().getMapOrigin();
    std::vector<Room> rooms;
    SegmentationCenter::instance().gateSegmentation(segmented_map, map_origin, rooms,
                                                    SegmentationDataBase::info2Gate(params));

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
    auto map_origin = MapAttributeSingleton::instance().getMapOrigin();
    std::vector<Room> rooms;
    SegmentationCenter::instance().gateSegmentation(segmented_map, map_origin, rooms,
                                                    SegmentationDataBase::info2Gate(params));

    params.setOMapId(SegmentationDataBase::instance().getDbMap().id);
    return SegmentationDataBase::instance().modifyGateInfo(params);
}

std::vector<GateInfo> ListGateStrategy::handler(std::string params) {
    return SegmentationDataBase::instance().loadGateInfo(SegmentationDataBase::instance().getDbMap().id);
}

int AddGateV2Strategy::handler(GateInfo params) {
    auto segmented_map = SegmentationCenter::instance().generateMat();
    auto map_origin = MapAttributeSingleton::instance().getMapOrigin();
    std::vector<Room> rooms;
    SegmentationCenter::instance().gateSegmentation(segmented_map, map_origin, rooms,
                                                    SegmentationDataBase::info2Gate(params));

    params.setId(-1);
    params.setOMapId(params.getOMapId()/*SegmentationDataBase::instance().getDbMap().id*/);
    return SegmentationDataBase::instance().addGateInfo(params);
}

long ModifyGateV2Strategy::handler(GateInfo params) {
    auto segmented_map = SegmentationCenter::instance().generateMat();
    auto map_origin = MapAttributeSingleton::instance().getMapOrigin();
    std::vector<Room> rooms;
    SegmentationCenter::instance().gateSegmentation(segmented_map, map_origin, rooms,
                                                    SegmentationDataBase::info2Gate(params));

    params.setOMapId(params.getOMapId()/*SegmentationDataBase::instance().getDbMap().id*/);
    return SegmentationDataBase::instance().modifyGateInfo(params);
}

GateInfo QueryIdGateStrategy::handler(long params) {
    return SegmentationDataBase::instance().queryGateForId(params);
}

std::string OpenGateSettingStrategy::handler(std::string params) {
    GateSettingCenter::instance().startInspect();
    return "";
}

std::string CloseGateSettingStrategy::handler(std::string params) {
    GateSettingCenter::instance().stopInspect();
    return "";
}

long MultipleModifyGateStrategy::handler(GateSimpleInfo params) {
    auto mapId = params.getOMapId();

    auto segmented_map = SegmentationCenter::instance().generateMat(mapId);
    MapAttribute mapAttribute;
    mapAttribute.attrPath = path::robot_slam_map_dir() + mapId + path::separator() + path::mymap_yaml;
    if (!MapAttributeSingleton::readAnyMapInfo(mapAttribute))
        throw app::exception(make_error_code(error::map_id_does_not_exist));

    GateInfo originalGate = SegmentationDataBase::instance().queryGateForId(params.getId());
    originalGate.setStartX(params.getStartX());
    originalGate.setStartY(params.getStartY());
    originalGate.setEndX(params.getEndX());
    originalGate.setEndY(params.getEndY());
    originalGate.setLeftGateId(params.getLeftGateId());
    originalGate.setRightGateId(params.getRightGateId());
    originalGate.setFactoryId(params.getFactoryId());

    std::vector<Room> rooms;
    SegmentationCenter::instance()
            .gateSegmentation(segmented_map,
                              {mapAttribute.originPose.position.x, mapAttribute.originPose.position.y},
                              rooms,
                              SegmentationDataBase::info2Gate(originalGate)
            );

    params.setOMapId(mapId);
    return SegmentationDataBase::instance().modifyGateInfo(originalGate);
}

std::vector<GateInfo> MultipleListGateStrategy::handler(std::string params) {
    return SegmentationDataBase::instance().loadGateInfo(params);
}

std::string MultiplePurgeGateStrategy::handler(std::string params) {
    SegmentationDataBase::instance().purgeGate(params);
    return "";
}
