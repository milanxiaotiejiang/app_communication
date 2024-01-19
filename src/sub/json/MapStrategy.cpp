//
// Created by lijiang on 2021/12/18.
//

#include "sub/json/MapStrategy.h"
#include "segmentation/SegmentationCenter.h"
#include "segmentation/map_attribute.h"
#include "exploration/ExplorationCenter.h"
#include "segmentation/map_modification.h"
#include "db/segmentation_data_base.h"
#include "leave/map_control.h"
#include "leave/cartographer_node.h"
#include "future/node/node_control.h"
#include "task/manager/MechanismManager.h"
#include "leave/HotWindNote.h"
#include "db/task_data_base.h"
#include <cppfs/fs.h>
#include <cppfs/FileHandle.h>
#include "exploration/tcr.h"
#include "task/manager/manual.h"
#include "task/manager/NodeWorkModeManager.h"
#include "tool/Variable.h"
#include "leave/ParamManager.h"
#include "db/property_data_base.h"
#include "tool/param_check.h"
#include "schedule/schedule_manager_singleton.h"

std::string FactoryResetStrategy::handler(std::string params) {
    PropertyDataBase::instance().resetConsumable(true, true, true, true, true, true);
    ParamManager::instance().reset();
    // 清除历史记录
    clean_history_db::CleanHistoryCenter::instance().removeCleanHistory();
    // 在此地图下，移除分区、与分区关联的任务
    SegmentationDataBase::instance().removeAllRoom();
    // 在此地图下，移除所有任务，包含定时任务
    TaskDataBase::instance().deleteOwnTask();
    // 在此地图下，重置禁行区域，并备份
    MapControl::instance().backupProhibition(SegmentationDataBase::instance().getDbMap().id, false, true);
    // 删除其他地图
    const std::vector<MapPo> &allMap = SegmentationDataBase::instance().loadAllMap();
    for (const auto &map: allMap) {
        if (!map.main) {
            // plan_param
            SegmentationDataBase::instance().removePlanParam(map.id);
            // segmentation
            SegmentationDataBase::instance().removeAllRoom(map.id);
            // gate
            SegmentationDataBase::instance().purgeGate(map.id);
            // 地图id
            SegmentationDataBase::instance().removeMap(map.id);
            // task
            TaskDataBase::instance().deleteTaskFoMap(map.id);

            MapControl::instance().removeInformation(map.id);
        }
    }
    return "";
}

std::string StartMapStrategy::handler(std::string params) {
    if (ParamManager::instance().getRainSnow()) {
        throw app::exception(make_error_code(error::please_exit_the_rain_and_snow_mode_first));
    }
    if (!Environment::instance().no_station_mapping_mode) {
        if (!ZooInnerStatus::instance().getIsCharging()) {
            throw app::exception(make_error_code(error::please_ensure_to_start_end_the_mapping_at_the_base_station));
        }
    }

    if (ManualManager::instance().taskRunning()) {
        throw app::exception(make_error_code(error::current_in_task));
    }
    if (!NodeWorkModeManager::instance().enterWorkMode(0)) {
        throw app::exception(make_error_code(error::mode_switching_is_not_supported));
    }

    HotWindNoteSingleton::instance().closeHotWind();

    // 电机失能
    std_msgs::Int32 map_start;
    map_start.data = 2;
    PublishInnerManager::instance().publishManualPush(map_start);

    return "";
}

#define multiple true

MapScore EndMapStrategy::handler(BuildMapParam params) {
    if (multiple)
        checkName(params.getMapName());
    if (params.isNewMap()) {
        params.setReset(false);
    }

    if (!Environment::instance().no_station_mapping_mode) {
        // 根据电量判断是否在基站，不在基站不处理开始/结束建图
        if (!ZooInnerStatus::instance().getIsCharging()) {
            if (params.isSave()) {
                throw app::exception(make_error_code(error::the_map_needs_to_be_saved_at_the_base_station_location));
            } else {
                throw app::exception(make_error_code(error::quit_map_needs_to_be_saved_at_the_base_station_location));
            }
        }
    }
    // 最终结果，包含建图地图评分
    MapScore mapScore;
    if (params.isSave()) {
        if (Variable::get_instance()->getMapApp().info.width *
            Variable::get_instance()->getMapApp().info.height < 6000) {//41*118
            throw app::exception(make_error_code(error::area_too_small));
        }
        //关键 保存地图
        if (!MapAttributeSingleton::instance().saveMap()) {
            bool isToSleep = NodeWorkModeManager::instance().tryToSleep();
            MapControl::instance().loadInformation(SegmentationDataBase::instance().getDbMap().id);
            MapControl::instance().changeMapServer();

            // 电机使能
            std_msgs::Int32 map_start;
            map_start.data = 0;
            PublishInnerManager::instance().publishManualPush(map_start);

            if (isToSleep) {
                throw app::exception(make_error_code(error::create_map_fail));
            } else {
                throw app::exception(make_error_code(error::create_map_fail_to_sleep));
            }
        }

        if (params.isNewMap()) {
            // 插入新地图信息
            SegmentationDataBase::instance().installMap(params.getBuildId(), params.getMapName(), params.getFloor(),
                                                        params.isBaseStation());
            SegmentationDataBase::instance().loadMainMap();
        } else {
            // 只修改地图名称
            MapPo &mapPo = SegmentationDataBase::instance().getDbMap();
            SegmentationDataBase::instance().updateMapName(mapPo.id, params.getMapName());
            SegmentationDataBase::instance().updateFloor(mapPo.id, params.getFloor());
            SegmentationDataBase::instance().changeBaseStation(mapPo.id, params.isBaseStation());
        }

//        // 更新本地内存中数据，单地图其实没必要更新
//        SegmentationDataBase::instance().updateMapName(SegmentationDataBase::instance().getDbMap().id, "default");
        //是否重置禁行区、任务等
        if (params.isReset()) {
            // 在此地图下，移除分区、与分区关联的任务
            SegmentationDataBase::instance().removeAllRoom(SegmentationDataBase::instance().getDbMap().id);
            // 在此地图下，移除所有任务，包含定时任务
            TaskDataBase::instance().deleteTaskFoMap(SegmentationDataBase::instance().getDbMap().id);
            // 在此地图下，重置禁行区域，并备份
            MapControl::instance().backupProhibition(SegmentationDataBase::instance().getDbMap().id, false, true);
            // 删除多个分区的相关信息
            SegmentationCenter::instance().resetSegmentation();
            // 删除闸机相关信息
            SegmentationCenter::instance().resetGateSegmentation();
        }
        if (params.isNewMap()) {
            // 新创建地图需要重置禁行区域
            MapControl::instance().backupProhibition(SegmentationDataBase::instance().getDbMap().id, false, true);
        }
        // 备份地图相关文件
        MapControl::instance().backupMap(SegmentationDataBase::instance().getDbMap().id, false);
        // 重新加载基站信息
        MapAttributeSingleton::instance().loadStation();
        // 更新内存中禁区
        MapAttributeSingleton::instance().resetProhibition();
        MapAttributeSingleton::instance().loadVirtualWall();
        MapAttributeSingleton::instance().loadPenaltyZone();
        // 使用全覆盖算法快速验证地图质量
        double proportion = tcr::coverageProportion();
        // 设置返回的结果
        mapScore.setId(SegmentationDataBase::instance().getDbMap().id);
        mapScore.setScore(proportion);
        // 更新内存中定时任务
        ScheduleManagerSingleton::instance().trigger_task_update();
        // 发布给 move_base 最新的禁行区域
        PublishInnerManager::instance().publishResetProhibition();
        // 重新规划牛耕田算法的全覆盖
        ExplorationCenter::instance().repaintCoveragePath();
    } else {
        // 本地的文件未变，重新更新地图信息
        MapControl::instance().changeMapServer();
    }

    // 电机使能
    std_msgs::Int32 map_start;
    map_start.data = 0;
    PublishInnerManager::instance().publishManualPush(map_start);

    NodeWorkModeManager::instance().toSleep();

    return mapScore;
}

std::vector<MultiMapInfo> GetMultiMapsStrategy::handler(std::string params) {
    std::vector<MultiMapInfo> mapInfos;
    const std::vector<MapPo> &allMap = SegmentationDataBase::instance().loadAllMap();
    for (const auto &map: allMap) {
        MultiMapInfo multiMapInfo(map.id, map.name, map.main, map.path);
        mapInfos.push_back(multiMapInfo);
    }
    return mapInfos;
}

std::string ChangeMapStrategy::handler(std::string params) {
    if (!Environment::instance().no_station_mapping_mode) {
        if (!ZooInnerStatus::instance().getIsCharging()) {
            throw app::exception(make_error_code(error::the_base_station_is_no_longer_able_to_switch_maps));
        }
    }
    if (!Environment::instance().no_station_mapping_mode) {
        if (!NodeControl::instance().isSleep()) {
//        CartographerPublisher::instance().publishStartCartoLocalization();
//        CartographerServiceClient::instance().callStartLocalization();
            throw app::exception(make_error_code(error::cannot_switch_maps_in_non_sleep_mode));
        }
    }

    MapPo oldMap = SegmentationDataBase::instance().getDbMap();
    if (oldMap.id == params) {
        throw app::exception(make_error_code(error::cannot_switch_to_the_current_map));
    }

    const std::vector<MapPo> &allMap = SegmentationDataBase::instance().loadAllMap();
    bool isFind = false;
    for (const auto &item: allMap) {
        if (item.id == params) {
            isFind = true;
            break;
        }
    }
    if (!isFind) {
        throw app::exception(make_error_code(error::map_id_does_not_exist));
    }
    // 确保文件存在
    if (!MapControl::instance().checkMapInformation(params)) {
        throw app::exception(make_error_code(error::map_id_does_not_exist));
    }
    // 备份之前的地图
    MapControl::instance().backupAndRetrieve(oldMap.id);
    // 改变为新地图信息
    SegmentationDataBase::instance().changeMap(params);
    SegmentationDataBase::instance().loadMainMap();
    // 加载新资源
    MapControl::instance().loadInformation(params);
    // 重新加载基站信息
    MapAttributeSingleton::instance().loadStation();
    // 更新内存中禁区
    MapAttributeSingleton::instance().resetProhibition();
    MapAttributeSingleton::instance().loadVirtualWall();
    MapAttributeSingleton::instance().loadPenaltyZone();
    // 更新内存中定时任务
    ScheduleManagerSingleton::instance().trigger_task_update();
    // 发布给 move_base 最新的禁行区域
    PublishInnerManager::instance().publishResetProhibition();
    // 重新规划牛耕田算法的全覆盖
    ExplorationCenter::instance().repaintCoveragePath();

    MapControl::instance().changeMapServer();

    return "";
}

std::string ModifyMapNameStrategy::handler(ModifyMapName params) {
    checkName(params.getName());
    const std::vector<MapPo> &allMap = SegmentationDataBase::instance().loadAllMap();
    bool isFind = false;
    for (const auto &item: allMap) {
        if (item.id == params.getId()) {
            isFind = true;
            break;
        }
    }
    if (!isFind) {
        throw app::exception(make_error_code(error::map_id_does_not_exist));
    }
    SegmentationDataBase::instance().updateMapName(params.getId(), params.getName());
    return "";
}

std::string DeleteMapStrategy::handler(std::string params) {
    MapPo oldMap = SegmentationDataBase::instance().getDbMap();
    if (oldMap.id == params) {
        throw app::exception(make_error_code(error::cannot_switch_to_the_current_map));
    }
    // plan_param
    SegmentationDataBase::instance().removePlanParam(params);
    // segmentation
    SegmentationDataBase::instance().removeAllRoom(params);
    // gate
    SegmentationDataBase::instance().purgeGate(params);
    // 地图id
    SegmentationDataBase::instance().removeMap(params);
    // task
    TaskDataBase::instance().deleteTaskFoMap(params);

    MapControl::instance().removeInformation(params);
    return "";
}

std::string EditMapStrategy::handler(std::vector<std::vector<float>> params) {
    reset_prohibition(path::prohibition_areas_path());

    for (int i = 0; i < params.size(); i++) {
        int type = params[i][0];//是区域还是线
        int point_num = 0;
        if (type == 1) {
            point_num = 8;//线的话4个点
        }
        if (type == 2) {
            point_num = 4;//线的话4个点
        }
        float *point = new float[point_num];
        for (int j = 1; j < point_num + 1; j++) {
            point[j - 1] = params[i][j];//点位信息
        }
        if (set_prohibition(path::prohibition_areas_path(), point, point_num)) {
//            ROS_INFO("set wall %d successfully", i);
        } else {
            ROS_ERROR("Failed to set wall!");
        }
    }
    PublishInnerManager::instance().publishResetProhibition();

    MapAttributeSingleton::instance().resetProhibition();
    MapAttributeSingleton::instance().loadVirtualWall();
    MapAttributeSingleton::instance().loadPenaltyZone();
    MapControl::instance().backupProhibition(SegmentationDataBase::instance().getDbMap().id, true, false);
    ExplorationCenter::instance().repaintCoveragePath();

    NoticeManager::instance().sendNotice(source_, M_MAP_PROHIBITION);
    return "";
}

std::vector<std::vector<float>> GetEditMapStrategy::handler(std::string params) {

    //操作，打开当前地图对应的编辑文件，并读取编辑信息
    std::vector<std::vector<float>> result;
    if (!get_prohibition(path::prohibition_areas_path(), result)) {
        ROS_ERROR("Fail to open file");
    }
    return result;
}

std::string MultipleEditMapStrategy::handler(CompositeFloatList params) {
    std::string mapId = params.getMapId();

    auto prohibitionAreasPath = path::robot_slam_map_dir() + mapId + path::separator() + path::prohibition_areas_yaml;

    std::vector<std::vector<float>> floats = params.getFloats();

    if (mapId == SegmentationDataBase::instance().getDbMap().id) {
        reset_prohibition(path::prohibition_areas_path());
        for (int i = 0; i < floats.size(); i++) {
            int type = floats[i][0];
            int point_num = 0;
            if (type == 1) point_num = 8;
            if (type == 2) point_num = 4;
            float *point = new float[point_num];
            for (int j = 1; j < point_num + 1; j++) {
                point[j - 1] = floats[i][j];
            }
            if (set_prohibition(path::prohibition_areas_path(), point, point_num)) {
                ROS_INFO("set wall %d successfully", i);
            } else {
                ROS_ERROR("Failed to set wall!");
            }
        }
        PublishInnerManager::instance().publishResetProhibition();

        MapAttributeSingleton::instance().resetProhibition();
        MapAttributeSingleton::instance().loadVirtualWall();
        MapAttributeSingleton::instance().loadPenaltyZone();
        MapControl::instance().backupProhibition(SegmentationDataBase::instance().getDbMap().id, true, false);
        ExplorationCenter::instance().repaintCoveragePath();

    } else {
        reset_prohibition(prohibitionAreasPath);
        for (int i = 0; i < floats.size(); i++) {
            int type = floats[i][0];
            int point_num = 0;
            if (type == 1) point_num = 8;
            if (type == 2) point_num = 4;
            float *point = new float[point_num];
            for (int j = 1; j < point_num + 1; j++) {
                point[j - 1] = floats[i][j];
            }
            if (set_prohibition(prohibitionAreasPath, point, point_num)) {
                ROS_INFO("set wall %d successfully", i);
            } else {
                ROS_ERROR("Failed to set wall!");
            }
        }
    }

    NoticeManager::instance().sendNotice(source_, M_MAP_PROHIBITION);
    return "";
}

CompositeFloatList MultipleGetEditMapStrategy::handler(std::string params) {
    const std::string &mapId = params;
    auto prohibitionAreasPath = path::robot_slam_map_dir() + mapId + path::separator() + path::prohibition_areas_yaml;

    CompositeFloatList compositeFloatList;
    compositeFloatList.setMapId(mapId);
    std::vector<std::vector<float>> result;
    if (mapId == SegmentationDataBase::instance().getDbMap().id) {
        if (!get_prohibition(path::prohibition_areas_path(), result)) {
            ROS_ERROR("Fail to open file");
        }
    } else {

        if (!get_prohibition(prohibitionAreasPath, result)) {
            ROS_ERROR("Fail to open file");
        }
    }
    compositeFloatList.setFloats(result);
    return compositeFloatList;
}

int ManualPushStartStrategy::handler(std::string params) {
    LOG_IF(INFO, DEBUG_REQUEST) << "MapStrategy manual_push_start ...";

    if (!ZooInnerStatus::instance().getIsCharging()) {
        throw app::exception(make_error_code(error::map_creation_needs_to_start_at_the_base_station));
    }

    HotWindNoteSingleton::instance().closeHotWind();

    std_msgs::Int32 map_start;
    map_start.data = 2;
    PublishInnerManager::instance().publishManualPush(map_start);
    return 5;
}

int ManualPushResetStrategy::handler(std::string params) {
    LOG_IF(INFO, DEBUG_REQUEST) << "MapStrategy manual_push_reset ...";

    std_msgs::Int32 map_start;
    map_start.data = 0;
    PublishInnerManager::instance().publishManualPush(map_start);
    return 5;
}

std::string MapObstaclesStrategy::handler(std::vector<std::vector<PointVo>> params) {
    std::vector<std::vector<cv::Point>> points;

    for (const auto &vector: params) {
        std::vector<cv::Point> cvs;
        for (const auto &pointVo: vector) {
            cv::Point point(pointVo.getX(), pointVo.getY());
            cvs.push_back(point);
        }
        points.push_back(cvs);
    }

    MapModification mapModification;
    mapModification.addObstacles(points);
    MapControl::instance().backupMap(SegmentationDataBase::instance().getDbMap().id, false);
    MapControl::instance().changeMapServer();
    ExplorationCenter::instance().repaintCoveragePath();
    NoticeManager::instance().sendNotice(source_, M_MAP_RESOURCE);
    return "";
}

std::string MapFeasibleZoneStrategy::handler(std::vector<std::vector<PointVo>> params) {
    std::vector<std::vector<cv::Point>> points;

    for (const auto &vector: params) {
        std::vector<cv::Point> cvs;
        for (const auto &pointVo: vector) {
            cv::Point point(pointVo.getX(), pointVo.getY());
            cvs.push_back(point);
        }
        points.push_back(cvs);
    }

    MapModification mapModification;
    mapModification.addFeasibleZone(points);
    MapControl::instance().backupMap(SegmentationDataBase::instance().getDbMap().id, false);
    MapControl::instance().changeMapServer();
    ExplorationCenter::instance().repaintCoveragePath();
    NoticeManager::instance().sendNotice(source_, M_MAP_RESOURCE);
    return "";
}

std::string MapApplyIncreaseArea::handler(std::vector<int> params) {
    MapModification mapModification;
    mapModification.applyIncreaseArea(params);
    MapControl::instance().backupMap(SegmentationDataBase::instance().getDbMap().id, false);
    MapControl::instance().changeMapServer();
    ExplorationCenter::instance().repaintCoveragePath();
    NoticeManager::instance().sendNotice(source_, M_MAP_RESOURCE);
    return "";
}

std::string MultipleMapObstaclesStrategy::handler(CompositePointList params) {
    std::string mapId = params.getMapId();

    std::vector<std::vector<cv::Point>> points;
    for (const auto &vector: params.getPoints()) {
        std::vector<cv::Point> cvs;
        for (const auto &pointVo: vector) {
            cv::Point point(pointVo.getX(), pointVo.getY());
            cvs.push_back(point);
        }
        points.push_back(cvs);
    }

    MapModification mapModification;

    if (mapId == SegmentationDataBase::instance().getDbMap().id) {
        mapModification.addObstacles(points);
        MapControl::instance().backupMap(SegmentationDataBase::instance().getDbMap().id, false);
        MapControl::instance().changeMapServer();
        ExplorationCenter::instance().repaintCoveragePath();
    } else {
        mapModification.addObstacles(mapId, points);
    }

    NoticeManager::instance().sendNotice(source_, M_MAP_RESOURCE);
    return "";
}

std::string MultipleMapFeasibleZoneStrategy::handler(CompositePointList params) {
    const std::string &mapId = params.getMapId();

    std::vector<std::vector<cv::Point>> points;
    for (const auto &vector: params.getPoints()) {
        std::vector<cv::Point> cvs;
        for (const auto &pointVo: vector) {
            cv::Point point(pointVo.getX(), pointVo.getY());
            cvs.push_back(point);
        }
        points.push_back(cvs);
    }

    MapModification mapModification;

    if (mapId == SegmentationDataBase::instance().getDbMap().id) {
        mapModification.addFeasibleZone(points);
        MapControl::instance().backupMap(SegmentationDataBase::instance().getDbMap().id, false);
        MapControl::instance().changeMapServer();
        ExplorationCenter::instance().repaintCoveragePath();
    } else {
        mapModification.addFeasibleZone(mapId, points);
    }

    NoticeManager::instance().sendNotice(source_, M_MAP_RESOURCE);
    return "";
}

long AddBuildStrategy::handler(BuildVo params) {
    checkName(params.getName());
    return SegmentationDataBase::instance().saveBuild(params.getName());
}

std::string DeleteBuildStrategy::handler(long params) {
    SegmentationDataBase::instance().removeBuild(params);
    return "";
}

std::string ModifyBuildNameStrategy::handler(BuildVo params) {
    checkName(params.getName());
    SegmentationDataBase::instance().modifyBuild(params.getId(), params.getName());
    return "";
}

std::vector<BuildVo> ListBuildStrategy::handler(std::string params) {

    auto buildMaps = SegmentationDataBase::instance().findBuildMapsForMap(
            SegmentationDataBase::instance().getDbMap().id);
    if (buildMaps.empty()) {
        throw app::exception(make_error_code(error::no_multi_map_buildings_have_been_set_up));
    } else if (buildMaps.size() == 1) {
        auto buildMap = buildMaps[0];
        auto currentBuild = buildMap.first;

        std::vector<BuildVo> buildResults;
        auto buildList = SegmentationDataBase::instance().loadAllBuild();
        for (const auto &item: buildList) {
            BuildVo build(item.id, item.name);
            build.setMain(item.id == currentBuild.id);
            buildResults.push_back(build);
        }
        return buildResults;
    } else {
        throw app::exception(make_error_code(error::multiple_map_building_data_error));
    }
}

std::string ModifyMapBaseStationStrategy::handler(MapBaseStation params) {
    SegmentationDataBase::instance().changeBaseStation(params.map_id, params.base_station);
    return "";
}

std::string ModifyMapFloorStrategy::handler(MapFloor params) {
    if (params.floor == 0)
        SegmentationDataBase::instance().removeFloor(params.map_id);
    else
        SegmentationDataBase::instance().updateFloor(params.map_id, params.floor);
    return "";
}

std::string ModifyMapElevatorStrategy::handler(MapElevator params) {
    if (params.elevator)
        SegmentationDataBase::instance().updateMapElevator(params.map_id);
    else
        SegmentationDataBase::instance().removeMapElevator(params.map_id);
    return "";
}

std::vector<MultiMapInfo> ListMapForBuildStrategy::handler(long params) {
    std::vector<MultiMapInfo> mapResults;
    auto floorBuildMaps = SegmentationDataBase::instance().findBuildMapsForBuild(params);
    for (const auto &buildMap: floorBuildMaps) {
        auto build = buildMap.first;
        auto map = buildMap.second;

        MapAttribute mapAttribute;
        mapAttribute.attrPath = path::robot_slam_map_dir() + map.id + path::separator() + path::mymap_yaml;
        if (!MapAttributeSingleton::readAnyMapInfo(mapAttribute))
            throw app::exception(make_error_code(error::map_id_does_not_exist));

        mapResults.emplace_back(map.id, map.name, map.main, map.path, map.elevator, map.elevator_position_x,
                                map.elevator_position_y, map.elevator_position_z, map.elevator_orientation_x,
                                map.elevator_orientation_y, map.elevator_orientation_z, map.elevator_orientation_w,
                                map.floor, map.base_station, mapAttribute.originPoint.x, mapAttribute.originPoint.y,
                                mapAttribute.originPose.position.x, mapAttribute.originPose.position.y,
                                mapAttribute.originPoint.y,
                                mapAttribute.mapCols, mapAttribute.mapRows,
                                build.id, build.name);
    }
    return mapResults;
}

std::string AttachBuildMapStrategy::handler(AttachBuildMap params) {
    auto buildMaps = SegmentationDataBase::instance().findBuildMapsForMap(params.mapId);
    if (buildMaps.empty()) {
    } else if (buildMaps.size() == 1) {
        std::pair<BuildPo, MapPo> buildMap = buildMaps[0];
        BuildPo &buildPo = buildMap.first;
        if (buildPo.id != params.buildId) {
            SegmentationDataBase::instance().detachBuildMap(buildPo.id, params.mapId);

            SegmentationDataBase::instance().attachBuildMap(params.buildId, params.mapId);
        }

    } else {
        throw app::exception(make_error_code(error::multiple_map_building_data_error));
    }
    return "";
}

MultiMapInfo MapForIdStrategy::handler(std::string params) {
    auto map = SegmentationDataBase::instance().loadMapForId(params);

    auto buildMaps = SegmentationDataBase::instance().findBuildMapsForMap(params);

    MapAttribute mapAttribute;
    mapAttribute.attrPath = path::robot_slam_map_dir() + map.id + path::separator() + path::mymap_yaml;
    if (!MapAttributeSingleton::readAnyMapInfo(mapAttribute))
        throw app::exception(make_error_code(error::map_id_does_not_exist));

    long buildId = -1;
    std::string buildName = "";

    if (buildMaps.empty()) {
    } else if (buildMaps.size() == 1) {

        buildId = buildMaps[0].first.id;
        buildName = buildMaps[0].first.name;
    } else {
        throw app::exception(make_error_code(error::multiple_map_building_data_error));
    }

    return MultiMapInfo(map.id, map.name, map.main, map.path, map.elevator, map.elevator_position_x,
                        map.elevator_position_y, map.elevator_position_z, map.elevator_orientation_x,
                        map.elevator_orientation_y, map.elevator_orientation_z, map.elevator_orientation_w,
                        map.floor, map.base_station, mapAttribute.originPoint.x, mapAttribute.originPoint.y,
                        mapAttribute.originPose.position.x, mapAttribute.originPose.position.y,
                        mapAttribute.originPoint.y,
                        mapAttribute.mapCols, mapAttribute.mapRows,
                        buildId, buildName);
}
