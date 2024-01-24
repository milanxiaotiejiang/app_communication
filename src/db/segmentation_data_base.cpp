//
// Created by Looper on 2022/9/16.
//

#include "db/segmentation_data_base.h"

#include <utility>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

#include "segmentation/Room.h"
#include "BaseThrowable.h"
#include "db/task_data_base.h"
#include "segmentation/map_attribute.h"

void SegmentationDataBase::resetMap(const MapPo &map) {
    mapPo.id = map.id;
    mapPo.name = map.name;
    mapPo.path = map.path;
    mapPo.main = map.main;
    mapPo.elevator = map.elevator;
    mapPo.elevator_position_x = map.elevator_position_x;
    mapPo.elevator_position_y = map.elevator_position_y;
    mapPo.elevator_position_z = map.elevator_position_z;
    mapPo.elevator_orientation_x = map.elevator_orientation_x;
    mapPo.elevator_orientation_y = map.elevator_orientation_y;
    mapPo.elevator_orientation_z = map.elevator_orientation_z;
    mapPo.elevator_orientation_w = map.elevator_orientation_w;
    mapPo.floor = map.floor;
    mapPo.base_station = map.base_station;
}

GateInfo SegmentationDataBase::gate2Info(const Gate &gate) {
    return GateInfo(
            gate.id, gate.o_map_id, gate.start_x, gate.start_y, gate.end_x, gate.end_y,
            gate.left_position_x, gate.left_position_y, gate.left_position_z,
            gate.left_orientation_x, gate.left_orientation_y, gate.left_orientation_z, gate.left_orientation_w,
            gate.right_position_x, gate.right_position_y, gate.right_position_z,
            gate.right_orientation_x, gate.right_orientation_y, gate.right_orientation_z, gate.right_orientation_w,
            gate.left_gate_ID, gate.right_gate_ID, gate.factory_ID
    );
}

Gate SegmentationDataBase::info2Gate(const GateInfo &gateInfo) {
    return Gate(
            gateInfo.getId(), gateInfo.getOMapId(), gateInfo.getStartX(), gateInfo.getStartY(), gateInfo.getEndX(),
            gateInfo.getEndY(),
            gateInfo.getLeftPositionX(), gateInfo.getLeftPositionY(), gateInfo.getLeftPositionZ(),
            gateInfo.getLeftOrientationX(), gateInfo.getLeftOrientationY(), gateInfo.getLeftOrientationZ(),
            gateInfo.getLeftOrientationW(),
            gateInfo.getRightPositionX(), gateInfo.getRightPositionY(), gateInfo.getRightPositionZ(),
            gateInfo.getRightOrientationX(), gateInfo.getRightOrientationY(), gateInfo.getRightOrientationZ(),
            gateInfo.getRightOrientationW(),
            gateInfo.getLeftGateId(), gateInfo.getRightGateId(),
            gateInfo.getFactoryId(), CURRENT_GATE_VERSION
    );
}

void SegmentationDataBase::sync_schema() {
    segmentationStorage.sync_schema();
}

bool SegmentationDataBase::loadMainMap() {
    try {
        std::vector<MapPo> mainMaps = segmentationStorage.get_all<MapPo>(where(c(&MapPo::main) == true));
        if (mainMaps.empty()) {
            const MapPo &defaultMap = installDefaultMap();
            mainMaps.push_back(defaultMap);
        }

        for (const auto &map: mainMaps) {
            if (map.main) {
                resetMap(map);
                break;
            }
        }
        return true;
    } catch (const std::system_error &e) {
        LOG(ERROR) << e.what();
        return false;
    }
}

bool SegmentationDataBase::existMap(const std::string &map_id) {
    try {
        segmentationStorage.get<MapPo>(map_id);
        return true;
    } catch (...) {
        return false;
    }
}

MapPo &SegmentationDataBase::getDbMap() {
    return mapPo;
}

MapPo SegmentationDataBase::installMap(long buildId, std::string name, int floor, bool base_station) {
    auto build = segmentationStorage.get<BuildPo>(buildId);

    segmentationStorage.update_all(sqlite_orm::set(c(&MapPo::main) = false));

    MapPo map;
    map.id = boost::uuids::to_string(boost::uuids::random_generator()());
    map.name = std::move(name);
    map.path = "";
    map.main = true;
    map.elevator = false;
    map.floor = floor;
    map.base_station = base_station;
    segmentationStorage.replace(map);

    attachBuildMap(buildId, map.id);
    return map;
}

void SegmentationDataBase::changeMap(const std::string &map_id) {
    segmentationStorage.update_all(sqlite_orm::set(c(&MapPo::main) = false));
    MapPo map = segmentationStorage.get<MapPo>(map_id);
    map.main = true;
    segmentationStorage.update(map);
}

MapPo SegmentationDataBase::installDefaultMap() {
    auto builds = SegmentationDataBase::instance().loadAllBuild();
    if (builds.empty()) {
        throw std::runtime_error("builds is empty ...");
    }
    SegmentationDataBase::instance().removeBuild();

    long buildId = SegmentationDataBase::instance().saveBuild("default_build", 0);

    auto mapList = segmentationStorage.get_all<MapPo>();
    for (const auto &item: mapList) {
        removeAllRoom(item.id);
        segmentationStorage.remove_all<PlanPo>(where(c(&PlanPo::map_id) == item.id));
    }
    segmentationStorage.remove_all<MapPo>();

    MapPo map;
    map.id = "default_map_uuid_0123456789";
    map.name = "default";
    map.path = "";
    map.main = true;
    map.elevator = false;
    map.floor = 0;
    map.base_station = true;
    segmentationStorage.replace(map);

    SegmentationDataBase::instance().attachBuildMap(buildId, map.id);
    return map;
}

std::vector<MapPo> SegmentationDataBase::loadAllMap() {
    return segmentationStorage.get_all<MapPo>();
}

MapPo SegmentationDataBase::loadMapForId(std::string mapId) {
    return segmentationStorage.get<MapPo>(mapId);
}

void SegmentationDataBase::updateMapName(const std::string &map_id, const std::string &map_name) {
    MapPo map = segmentationStorage.get<MapPo>(map_id);
    map.name = map_name;
    segmentationStorage.update(map);

    if (map.id == mapPo.id) {
        resetMap(selectMapById(map_id));
    }
}

MapPo SegmentationDataBase::updateMapElevator(const std::string &map_id) {
    auto pose = MapAttributeSingleton::instance().getRobotCompletePosition();
    MapPo map = segmentationStorage.get<MapPo>(map_id);
    map.elevator = true;
    map.elevator_position_x = pose.position.x;
    map.elevator_position_y = pose.position.y;
    map.elevator_position_z = pose.position.z;
    map.elevator_orientation_x = pose.orientation.x;
    map.elevator_orientation_y = pose.orientation.y;
    map.elevator_orientation_z = pose.orientation.z;
    map.elevator_orientation_w = pose.orientation.w;
    segmentationStorage.update(map);

    if (map.id == mapPo.id) {
        resetMap(selectMapById(map_id));
    }
    return map;
}

MapPo SegmentationDataBase::removeMapElevator(const std::string &map_id) {
    MapPo map = segmentationStorage.get<MapPo>(map_id);
    map.elevator = false;
    segmentationStorage.update(map);

    if (map.id == mapPo.id) {
        resetMap(selectMapById(map_id));
    }
    return map;
}

MapPo SegmentationDataBase::updateFloor(const std::string &map_id, int floor) {
    // todo 写到前面
    if (floor <= -2 || floor > 100 || floor == 0)
        throw app::exception(make_error_code(error::beyond_the_floor_range));

    auto buildMaps = SegmentationDataBase::instance().findBuildMapsForMap(map_id);
    if (buildMaps.empty())
        throw app::exception(make_error_code(error::unassociated_building_cannot_be_set_up));
    if (buildMaps.size() != 1)
        throw app::exception(make_error_code(error::multiple_map_building_data_error));

    auto buildMap = buildMaps[0];
    auto build = buildMap.first;
    auto buildId = build.id;

    auto floorBuildMaps = SegmentationDataBase::instance().findBuildMapsForBuild(buildId);
    bool exist = false;
    for (const auto &floorBuildMap: floorBuildMaps) {
        if (floorBuildMap.second.floor == floor) {
            if (floorBuildMap.second.id != map_id) {
                exist = true;
            }
        }
    }
    if (exist)
        throw app::exception(make_error_code(error::current_floor_exists));

    MapPo map = segmentationStorage.get<MapPo>(map_id);
    map.floor = floor;
    segmentationStorage.update(map);

    if (map.id == mapPo.id) {
        resetMap(selectMapById(map_id));
    }
    return map;
}

MapPo SegmentationDataBase::removeFloor(const std::string &map_id) {
    MapPo map = segmentationStorage.get<MapPo>(map_id);
    map.floor = 0;
    segmentationStorage.update(map);

    if (map.id == mapPo.id) {
        resetMap(selectMapById(map_id));
    }
    return map;
}

MapPo SegmentationDataBase::changeBaseStation(const std::string &map_id, bool hasBaseStation) {
    MapPo map = segmentationStorage.get<MapPo>(map_id);
    map.base_station = hasBaseStation;
    segmentationStorage.update(map);

    if (map.id == mapPo.id) {
        resetMap(selectMapById(map_id));
    }
    return map;
}

MapPo SegmentationDataBase::selectMapById(const std::string &map_id) {
    return segmentationStorage.get<MapPo>(map_id);
}

void SegmentationDataBase::removeMap(const std::string &map_id) {
    segmentationStorage.remove<MapPo>(map_id);
    segmentationStorage.remove_all<BuildMapMapping>(
            where(c(&BuildMapMapping::o_map_id) == map_id)
    );
}

RoomPo SegmentationDataBase::selectRoomById(long roomId) {
    return segmentationStorage.get<RoomPo>(roomId);
}

std::vector<RoomPo> SegmentationDataBase::selectRoomByMapId(const std::string &mapId) {
    return segmentationStorage.get_all<RoomPo>(where(c(&RoomPo::o_map_id) == mapId));
}

void SegmentationDataBase::removeAllRoom(const std::string &mapId) {
    segmentationStorage.remove_all<RoomPo>(where(c(&RoomPo::o_map_id) == mapId));
    TaskDataBase::instance().deleteTaskFoMode(mapId, TaskMode::Subregion, true);
}

void SegmentationDataBase::removeAllRoom() {
    segmentationStorage.remove_all<RoomPo>();
}

void SegmentationDataBase::memory2Storage(cv::Mat &mat, std::vector<Room> &rooms) {
//    cv::imshow("memory2Storage", mat);
//    cv::waitKey();
    segmentationStorage.transaction([&] {

        removeAllRoom(mapPo.id);

        for (auto &item: rooms) {
            auto id = item.getID();
            auto name = item.getName();
            auto area = item.getArea();
            auto center = item.getCenter();

            auto neighborIds = item.getNeighborIDs();
            auto perimeter = item.getPerimeter();

            std::string neighbor_room_ids;
            for (int i = 0; i < neighborIds.size(); i++) {
                auto neighborId = neighborIds[i];
                if (i == neighborIds.size() - 1) {
                    neighbor_room_ids.append(std::to_string(neighborId));
                } else {
                    neighbor_room_ids.append(std::to_string(neighborId)).append(path::split_str);
                }
            }

            RoomPo dbRoom(mapPo.id, name, id, area, center.x, center.y, perimeter, neighbor_room_ids);
            segmentationStorage.insert(dbRoom);
        }

//        auto uuid = boost::uuids::to_string(boost::uuids::random_generator()());
//        auto randomPngPath = mapPo.path + SEGMENTATION_MB + uuid + ".png";
//        auto depth = mat.clone();
//        CvUtils::savePng(randomPngPath, depth);

        return CvUtils::write(path::map_segmentation_path(), mat);
    });
}

void
SegmentationDataBase::storage2Memory(cv::Mat &mat, std::vector<Room> &rooms, double map_resolution_from_subscription) {
    try {
        mat = CvUtils::read(path::map_segmentation_path());

        auto result = segmentationStorage.get_all<RoomPo>(where(c(&RoomPo::o_map_id) == mapPo.id));

        for (const auto &item: result) {

            std::vector<cv::Point> members;
            for (int y = 0; y < mat.rows; y++) {
                for (int x = 0; x < mat.cols; x++) {
                    auto value = mat.at<int>(y, x);
                    auto point = cv::Point(x, y);
                    if (item.value == value) {
                        members.push_back(point);
                    }
                }
            }

            std::vector<int> neighbor_room_ids;
            std::string str = item.neighbor_room_ids + path::split_str;
            size_t pos = str.find(path::split_str);
            int step = path::split_str.size();
            while (pos != str.npos) {
                std::string temp = str.substr(0, pos);
                neighbor_room_ids.push_back(atoi(temp.c_str()));
                //去掉已分割的字符串,在剩下的字符串中进行分割
                str = str.substr(pos + step, str.size());
                pos = str.find(path::split_str);
            }

            Room room(item.value);
            room.setName(item.name);
            room.setArea(item.area);
            room.setPerimeter(item.perimeter);
            room.directInsertMemberPoints(members, map_resolution_from_subscription);
            for (const auto &neighborRoomId: neighbor_room_ids) {
                room.addNeighborID(neighborRoomId);
            }
            room.setDbId(item.id);
            rooms.push_back(room);
        }
//        cv::imshow("storage2Memory", mat);
//        cv::waitKey();
    } catch (app::exception const &e) {
        removeAllRoom(mapPo.id);
        throw app::exception(make_error_code(error::room_mb_file_open_fail));
    }
}

void SegmentationDataBase::reRoomName(int targetId, const std::string &name) {
    RoomPo roomPo = SegmentationDataBase::instance().selectRoomById(targetId);
    roomPo.name = name;
    segmentationStorage.update(roomPo);
//    auto results = segmentationStorage.get_all<RoomPo>(
//            where(c(&RoomPo::o_map_id) == mapPo.id and c(&RoomPo::value) == targetId)
//    );
//    segmentationStorage.transaction([results, name, this] {
//        for (auto item: results) {
//            item.name = name;
//            segmentationStorage.update(item);
//        }
//        return true;
//    });
}

PlanPo SegmentationDataBase::loadDefaultPlanParam(const std::string &mapId) {
    return SegmentationDataBase::instance().setPlanParam(mapId,
                                                         robot_radius_,
                                                         map_correction_closing_neighborhood_size_,
                                                         grid_obstacle_offset_,
                                                         path_eps_,
                                                         min_cell_area_,
                                                         max_deviation_from_track_,
                                                         range_near_base_station_,
                                                         room_area_factor_lower_limit_,
                                                         room_area_factor_upper_limit_,
                                                         neighborhood_index_,
                                                         max_iterations_,
                                                         min_critical_point_distance_factor_,
                                                         max_area_for_merging_,
                                                         distance_from_obstacles_,
                                                         number_extension_,
                                                         multiple_contour_spacing_,
                                                         random_number_generation_ratio_,
                                                         boundary_min_area_);
}

PlanPo SegmentationDataBase::setPlanParam(const std::string &mapId, double robotRadius,
                                          int mapCorrectionClosingNeighborhoodSize,
                                          double gridObstacleOffset, double pathEps, double minCellArea,
                                          double maxDeviationFromTrack,
                                          int rangeNearBaseStation, double roomAreaFactorLowerLimit,
                                          double roomAreaFactorUpperLimit,
                                          int neighborhoodIndex, int maxIterations,
                                          double minCriticalPointDistanceFactor,
                                          double maxAreaForMerging, int distanceFromObstacles, int numberExtension,
                                          int multipleContourSpacing, int random_number_generation_ratio,
                                          int boundary_min_area, int version) {
    PlanPo planPo(mapId, robotRadius, mapCorrectionClosingNeighborhoodSize,
                  gridObstacleOffset, pathEps, minCellArea, maxDeviationFromTrack,
                  rangeNearBaseStation, roomAreaFactorLowerLimit, roomAreaFactorUpperLimit,
                  neighborhoodIndex, maxIterations, minCriticalPointDistanceFactor, maxAreaForMerging,
                  distanceFromObstacles, numberExtension, multipleContourSpacing,
                  random_number_generation_ratio, boundary_min_area, version);
    segmentationStorage.replace(planPo);
    return planPo;
}

void SegmentationDataBase::removePlanParam(const std::string &mapId) {
    segmentationStorage.remove_all<PlanPo>(
            where(c(&PlanPo::map_id) == std::move(mapId))
    );
}

PlanPo SegmentationDataBase::getDbPlan(const std::string &mapId) {
    auto vectorPlan = segmentationStorage.get_all<PlanPo>(
            where(c(&PlanPo::map_id) == mapId)
    );
    if (!vectorPlan.empty())
        return vectorPlan.front();
    else {
        return SegmentationDataBase::instance().loadDefaultPlanParam(mapId);
    };
}

void SegmentationDataBase::saveGate(const Gate &gate) {
    segmentationStorage.insert(gate);
}

std::vector<Gate> SegmentationDataBase::loadGate(const std::string &mapId) {
    return segmentationStorage.get_all<Gate>(where(c(&Gate::o_map_id) == mapId));
}

void SegmentationDataBase::purgeGate(const std::string &mapId) {
    segmentationStorage.remove_all<Gate>(where(c(&Gate::o_map_id) == mapId));
}

int SegmentationDataBase::addGateInfo(const GateInfo &gateInfo) {
    return segmentationStorage.insert(info2Gate(gateInfo));
}

void SegmentationDataBase::deleteGateForId(long id) {
    segmentationStorage.remove<Gate>(id);
}

long SegmentationDataBase::modifyGateInfo(const GateInfo &gateInfo) {
    segmentationStorage.update(info2Gate(gateInfo));
    return gateInfo.getId();
}

void SegmentationDataBase::modifyGateLine(long id, double start_x, double start_y, double end_x, double end_y) {
    auto gate = segmentationStorage.get<Gate>(id);
    gate.start_x = start_x;
    gate.start_y = start_y;
    gate.end_x = end_x;
    gate.end_y = end_y;
    segmentationStorage.update(gate);
}

std::vector<GateInfo> SegmentationDataBase::loadGateInfo(const std::string &mapId) {
    auto gates = segmentationStorage.get_all<Gate>(where(c(&Gate::o_map_id) == mapId));
    std::vector<GateInfo> gateInfos;
    for (const auto &gate: gates) {
        gateInfos.push_back(gate2Info(gate));
    }
    return gateInfos;
}

GateInfo SegmentationDataBase::queryGateForId(long id) {
    Gate originalGate = segmentationStorage.get<Gate>(id);
    return gate2Info(originalGate);
}

long SegmentationDataBase::saveBuild(const std::string &name, int elevator_address) {
    BuildPo buildPo;
    buildPo.name = std::move(name);
    buildPo.elevator_address = elevator_address;
    return segmentationStorage.insert(buildPo);
}

void SegmentationDataBase::removeBuild(long id) {
    segmentationStorage.remove<BuildPo>(id);
    segmentationStorage.remove_all<BuildMapMapping>(
            where(c(&BuildMapMapping::o_build_id) == id)
    );
}

void SegmentationDataBase::removeBuild() {
    segmentationStorage.remove_all<BuildPo>();
    segmentationStorage.remove_all<BuildMapMapping>();
}

std::vector<BuildPo> SegmentationDataBase::loadAllBuild() {
    return segmentationStorage.get_all<BuildPo>();
}

BuildPo SegmentationDataBase::queryBuildForId(long id) {
    return segmentationStorage.get<BuildPo>(id);
}

void SegmentationDataBase::modifyBuildName(long id, const std::string &name) {
    auto buildPo = segmentationStorage.get<BuildPo>(id);
    buildPo.name = name;
    segmentationStorage.update(buildPo);
}

void SegmentationDataBase::modifyBuildElevatorAddress(long id, int elevator_address) {
    auto buildPo = segmentationStorage.get<BuildPo>(id);
    buildPo.elevator_address = elevator_address;
    segmentationStorage.update(buildPo);
}

void SegmentationDataBase::attachBuildMap(long buildId, const std::string &mapId) {
    auto list = segmentationStorage.get_all<BuildMapMapping>(
            where(
                    c(&BuildMapMapping::o_build_id) == buildId
                    and
                    c(&BuildMapMapping::o_map_id) == mapId
            )
    );
    if (list.empty()) {
        BuildMapMapping mapping(buildId, mapId);
        segmentationStorage.insert(mapping);
    }
}

void SegmentationDataBase::detachBuildMap(long buildId, const std::string &mapId) {
    segmentationStorage.remove_all<BuildMapMapping>(
            where(
                    c(&BuildMapMapping::o_build_id) == buildId
                    and
                    c(&BuildMapMapping::o_map_id) == mapId
            )
    );
}

std::vector<std::pair<BuildPo, MapPo>> SegmentationDataBase::findBuildMapsForBuild(long buildId) {
    auto results = segmentationStorage.select(
            distinct(columns(
                    &MapPo::id,
                    &MapPo::name,
                    &MapPo::path,
                    &MapPo::main,
                    &BuildPo::id,
                    &BuildPo::name,
                    &BuildPo::elevator_address,
                    &MapPo::elevator,
                    &MapPo::elevator_position_x,
                    &MapPo::elevator_position_y,
                    &MapPo::elevator_position_z,
                    &MapPo::elevator_orientation_x,
                    &MapPo::elevator_orientation_y,
                    &MapPo::elevator_orientation_z,
                    &MapPo::elevator_orientation_w,
                    &MapPo::floor,
                    &MapPo::base_station
            )),
            inner_join<MapPo>(on(c(&MapPo::id) == &BuildMapMapping::o_map_id)),
            inner_join<BuildPo>(on(c(&BuildPo::id) == &BuildMapMapping::o_build_id)),
            where(c(&BuildMapMapping::o_build_id) == buildId)
    );

    std::vector<std::pair<BuildPo, MapPo>> vos;
    for (const auto &row: results) {
        BuildPo b(
                std::get<4>(row),//id
                std::get<5>(row),//name
                std::get<6>(row)//elevator_address
        );
        MapPo m(
                std::get<0>(row),//id
                std::get<1>(row),//name
                std::get<2>(row),//path
                std::get<3>(row),//main
                std::get<7>(row),//elevator
                std::get<8>(row),//elevator_position_x
                std::get<9>(row),//elevator_position_y
                std::get<10>(row),//elevator_position_z
                std::get<11>(row),//elevator_orientation_x
                std::get<12>(row),//elevator_orientation_y
                std::get<13>(row),//elevator_orientation_z
                std::get<14>(row),//elevator_orientation_w
                std::get<15>(row),//floor
                std::get<16>(row)//base_station
        );
        auto pair = std::make_pair(b, m);
        vos.push_back(pair);
    }

    return vos;
}

std::vector<std::pair<BuildPo, MapPo>> SegmentationDataBase::findBuildMapsForMap(const std::string &mapId) {
    auto results = segmentationStorage.select(
            distinct(columns(
                    &MapPo::id,
                    &MapPo::name,
                    &MapPo::path,
                    &MapPo::main,
                    &BuildPo::id,
                    &BuildPo::name,
                    &BuildPo::elevator_address,
                    &MapPo::elevator,
                    &MapPo::elevator_position_x,
                    &MapPo::elevator_position_y,
                    &MapPo::elevator_position_z,
                    &MapPo::elevator_orientation_x,
                    &MapPo::elevator_orientation_y,
                    &MapPo::elevator_orientation_z,
                    &MapPo::elevator_orientation_w,
                    &MapPo::floor,
                    &MapPo::base_station
            )),
            inner_join<MapPo>(on(c(&MapPo::id) == &BuildMapMapping::o_map_id)),
            inner_join<BuildPo>(on(c(&BuildPo::id) == &BuildMapMapping::o_build_id)),
            where(c(&BuildMapMapping::o_map_id) == mapId)
    );

    std::vector<std::pair<BuildPo, MapPo>> vos;
    for (const auto &row: results) {
        BuildPo b(
                std::get<4>(row),//id
                std::get<5>(row),//name
                std::get<6>(row)//elevator_address
        );
        MapPo m(
                std::get<0>(row),//id
                std::get<1>(row),//name
                std::get<2>(row),//path
                std::get<3>(row),//main
                std::get<7>(row),//elevator
                std::get<8>(row),//elevator_position_x
                std::get<9>(row),//elevator_position_y
                std::get<10>(row),//elevator_position_z
                std::get<11>(row),//elevator_orientation_x
                std::get<12>(row),//elevator_orientation_y
                std::get<13>(row),//elevator_orientation_z
                std::get<14>(row),//elevator_orientation_w
                std::get<15>(row),//floor
                std::get<16>(row)//base_station
        );
        auto pair = std::make_pair(b, m);
        vos.push_back(pair);
    }

    return vos;
}
