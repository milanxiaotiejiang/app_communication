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

MapInfo SaveMapStrategy::handler(MapInfo params) {
    if (!ZooInnerStatus::instance().getIsCharging()) {
        throw app::exception(make_error_code(error::the_map_needs_to_be_saved_at_the_base_station_location));
    }
    // todo 此版本为单地图
    if (MapAttribute::instance().saveMap()) {

        SegmentationDataBase::instance().updateMapName(SegmentationDataBase::instance().getDbMap().id,
                                                       params.getMapName());

        if (params.isReset()) {
            SegmentationDataBase::instance().removeAllRoom(SegmentationDataBase::instance().getDbMap().id);
            TaskDataBase::instance().deleteTaskFoMap(SegmentationDataBase::instance().getDbMap().id);
            MapControl::instance().backupProhibition(SegmentationDataBase::instance().getDbMap().id, true);
            PublishInnerManager::instance().publishResetProhibition();
            cppfs::FileHandle file_timer_info_json = cppfs::fs::open(
                    path::data_base_config_dir() + "timer_info_json.txt");
            file_timer_info_json.remove();
            cppfs::FileHandle file_view_part_principal_json = cppfs::fs::open(
                    path::data_base_config_dir() + "view_part_principal_json.txt");
            file_view_part_principal_json.remove();
            cppfs::FileHandle file_combination_list_principal_json_work = cppfs::fs::open(
                    path::data_base_config_dir() + "combination_list_principal_json_work.txt");
            file_combination_list_principal_json_work.remove();
        }

        MapPo &mapPo = SegmentationDataBase::instance().getDbMap();
        MapInfo param(mapPo.id, mapPo.name);

        ExplorationCenter::instance().repaintCoveragePath(true);

        return param;
    } else {
        throw app::exception(make_error_code(error::create_map_fail));
    }

//    MapPo oldMap = SegmentationDataBase::instance().getDbMap();
//    MapControl::instance().backupAndRetrieve(oldMap.id);
//    if (MapAttribute::instance().saveMap()) {
//        const MapPo &newMap = SegmentationDataBase::instance().installMap(params.getMapName());
//        SegmentationDataBase::instance().loadMainMap();
//        MapControl::instance().backupProhibition(newMap.id, false);
//        MapControl::instance().backupMap(newMap.id, false);
//
//        ExplorationCenter::instance().repaintCoveragePath(true);
//
//        MapInfo param(newMap.id, newMap.name);
//        return param;
//    } else {
//        MapControl::instance().loadInformation(oldMap.id);
//        throw app::exception(make_error_code(error::create_map_fail));
//    }
}

vector<MapInfo> GetMultiMapsStrategy::handler(string params) {
    std::vector<MapInfo> mapInfos;
    const std::vector<MapPo> &allMap = SegmentationDataBase::instance().loadAllMap();
    for (const auto &map: allMap) {
        MapInfo mapInfo(map.id, map.name);
        mapInfos.push_back(mapInfo);
    }
    return mapInfos;
}

string ChangeMapStrategy::handler(string params) {
    MapPo oldMap = SegmentationDataBase::instance().getDbMap();
    if (oldMap.id == params) {
        throw app::exception(make_error_code(error::create_map_fail));
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
    if (!MapControl::instance().checkMapInformation(params)) {
        throw app::exception(make_error_code(error::map_id_does_not_exist));
    }

    MapControl::instance().backupAndRetrieve(oldMap.id);

    MapControl::instance().loadInformation(params);
    MapControl::instance().changeMapServer();
    if (NodeControl::instance().isWork()) {
//        CartographerPublisher::instance().publishStartCartoLocalization();
        CartographerServiceClient::instance().callStartLocalization();
    }
    return "";
}

string ModifyMapNameStrategy::handler(MapInfo params) {
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
    SegmentationDataBase::instance().updateMapName(params.getId(), params.getMapName());
    return "";
}

string DeleteMapStrategy::handler(string params) {

}

string EditMapStrategy::handler(vector<std::vector<float>> params) {
    //操作，将编辑信息写入当前地图对应的编辑文件内
    int prohibition_num = params.size();
    reset_prohibition();

    for (int i = 0; i < prohibition_num; i++) {
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
        if (set_prohibition(point, point_num)) {
//            ROS_INFO("set wall %d successfully", i);
        } else {
            ROS_ERROR("Failed to set wall!");
        }
    }
    PublishInnerManager::instance().publishResetProhibition();

    MapAttribute::instance().resetProhibition();
    MapAttribute::instance().loadVirtualWall();
    MapAttribute::instance().loadPenaltyZone();
    MapControl::instance().backupProhibition(SegmentationDataBase::instance().getDbMap().id, false);
    ExplorationCenter::instance().repaintCoveragePath(false);
    return "";
}

vector<std::vector<float>> GetEditMapStrategy::handler(string params) {

    //操作，打开当前地图对应的编辑文件，并读取编辑信息
    std::vector<std::vector<float>> result;
    if (!get_prohibition(result)) {
        ROS_ERROR("Fail to open file");
    }
    return result;
}

int ManualPushStartStrategy::handler(string params) {
    LOG(INFO) << "MapStrategy manual_push_start ...";

    if (!ZooInnerStatus::instance().getIsCharging()) {
        throw app::exception(make_error_code(error::map_creation_needs_to_start_at_the_base_station));
    }

    HotWindNoteSingleton::instance().closeHotWind();

    std_msgs::Int32 map_start;
    map_start.data = 2;
    PublishInnerManager::instance().publishManualPush(map_start);
    return 5;
}

int ManualPushResetStrategy::handler(string params) {
    LOG(INFO) << "MapStrategy manual_push_reset ...";
    std_msgs::Int32 map_start;
    map_start.data = 0;
    PublishInnerManager::instance().publishManualPush(map_start);
    return 5;
}

string MapObstaclesStrategy::handler(vector<vector<PointVo>> params) {
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
    ExplorationCenter::instance().repaintCoveragePath(false);
    return "";
}

string MapFeasibleZoneStrategy::handler(vector<vector<PointVo>> params) {
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
    ExplorationCenter::instance().repaintCoveragePath(false);
    return "";
}

string MapApplyIncreaseArea::handler(vector<int> params) {
    MapModification mapModification;
    mapModification.applyIncreaseArea(params);
    MapControl::instance().backupMap(SegmentationDataBase::instance().getDbMap().id, false);
    MapControl::instance().changeMapServer();
    ExplorationCenter::instance().repaintCoveragePath(false);
    return "";
}
