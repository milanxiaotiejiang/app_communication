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
    MapControl::instance().backupProhibition(SegmentationDataBase::instance().getDbMap().id, true);
    return "";
}

std::string StartMapStrategy::handler(std::string params) {
    if (ParamManager::instance().getRainSnow()) {
        throw app::exception(make_error_code(error::please_exit_the_rain_and_snow_mode_first));
    }
    if (!ZooInnerStatus::instance().getIsCharging()) {
        throw app::exception(make_error_code(error::please_ensure_to_start_end_the_mapping_at_the_base_station));
    }
    if (ManualManager::instance().taskRunning()) {
        throw app::exception(make_error_code(error::current_in_task));
    }
    if (!NodeWorkModeManager::instance().enterWorkMode(0)) {
        throw app::exception(make_error_code(error::mode_switching_is_not_supported));
    }

    HotWindNoteSingleton::instance().closeHotWind();

    std_msgs::Int32 map_start;
    map_start.data = 2;
    PublishInnerManager::instance().publishManualPush(map_start);

    return "";
}

MapScore EndMapStrategy::handler(MapParam params) {
    // 根据电量判断是否在基站，不在基站不处理开始/结束建图
    if (!ZooInnerStatus::instance().getIsCharging()) {
        if (params.isSave()) {
            throw app::exception(make_error_code(error::the_map_needs_to_be_saved_at_the_base_station_location));
        } else {
            throw app::exception(make_error_code(error::quit_map_needs_to_be_saved_at_the_base_station_location));
        }
    }
    // 电机失能
    std_msgs::Int32 map_start;
    map_start.data = 0;
    PublishInnerManager::instance().publishManualPush(map_start);
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
            if (isToSleep) {
                throw app::exception(make_error_code(error::create_map_fail));
            } else {
                throw app::exception(make_error_code(error::create_map_fail_to_sleep));
            }
        }
        // 更新本地内存中数据，单地图其实没必要更新
        SegmentationDataBase::instance().updateMapName(SegmentationDataBase::instance().getDbMap().id, "default");
        //是否重置禁行区、任务等
        if (params.isReset()) {
            // 在此地图下，移除分区、与分区关联的任务
            SegmentationDataBase::instance().removeAllRoom(SegmentationDataBase::instance().getDbMap().id);
            // 在此地图下，移除所有任务，包含定时任务
            TaskDataBase::instance().deleteTaskFoMap(SegmentationDataBase::instance().getDbMap().id);
            // 在此地图下，重置禁行区域，并备份
            MapControl::instance().backupProhibition(SegmentationDataBase::instance().getDbMap().id, true);
            // 删除掉早期过期文件信息
            removeAncientNeeds();
        }
        // 备份地图相关文件，不删除
        MapControl::instance().backupMap(SegmentationDataBase::instance().getDbMap().id, false);
        // 删除多个分区的相关信息
        SegmentationCenter::instance().resetSegmentation();
        // 删除闸机相关信息
        SegmentationCenter::instance().resetGateSegmentation();
        // 重新加载基站信息
        MapAttributeSingleton::instance().loadStation();
        // 使用全覆盖算法快速验证地图质量
        double proportion = tcr::coverageProportion();
        // 设置返回的结果
        mapScore.setId(SegmentationDataBase::instance().getDbMap().id);
        mapScore.setScore(proportion);
        // 发布给 move_base 最新的禁行区域
        PublishInnerManager::instance().publishResetProhibition();
        // 重新规划牛耕田算法的全覆盖
        ExplorationCenter::instance().repaintCoveragePath();
    } else {
        // 本地的文件未变，重新更新地图信息
        MapControl::instance().changeMapServer();
    }

    NodeWorkModeManager::instance().toSleep();

    return mapScore;
}

void EndMapStrategy::removeAncientNeeds() const {
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

std::vector<MapInfo> GetMultiMapsStrategy::handler(std::string params) {
    std::vector<MapInfo> mapInfos;
    const std::vector<MapPo> &allMap = SegmentationDataBase::instance().loadAllMap();
    for (const auto &map: allMap) {
        MapInfo mapInfo(map.id, map.name);
        mapInfos.push_back(mapInfo);
    }
    return mapInfos;
}

std::string ChangeMapStrategy::handler(std::string params) {
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

std::string ModifyMapNameStrategy::handler(MapInfo params) {
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

std::string DeleteMapStrategy::handler(std::string params) {

}

std::string EditMapStrategy::handler(std::vector<std::vector<float>> params) {
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

    MapAttributeSingleton::instance().resetProhibition();
    MapAttributeSingleton::instance().loadVirtualWall();
    MapAttributeSingleton::instance().loadPenaltyZone();
    MapControl::instance().backupProhibition(SegmentationDataBase::instance().getDbMap().id, false);
    ExplorationCenter::instance().repaintCoveragePath();
    return "";
}

std::vector<std::vector<float>> GetEditMapStrategy::handler(std::string params) {

    //操作，打开当前地图对应的编辑文件，并读取编辑信息
    std::vector<std::vector<float>> result;
    if (!get_prohibition(result)) {
        ROS_ERROR("Fail to open file");
    }
    return result;
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
    return "";
}

std::string MapApplyIncreaseArea::handler(std::vector<int> params) {
    MapModification mapModification;
    mapModification.applyIncreaseArea(params);
    MapControl::instance().backupMap(SegmentationDataBase::instance().getDbMap().id, false);
    MapControl::instance().changeMapServer();
    ExplorationCenter::instance().repaintCoveragePath();
    return "";
}
