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

MapInfo SaveMapStrategy::handler(MapInfo params) {
    // todo 此版本为单地图
//    MapPo oldMap = SegmentationDataBase::instance().getDbMap();
//    MapControl::instance().backupAndRetrieve(oldMap.id);
    if (MapAttribute::instance().saveMap()) {
        // todo 此版本为单地图
//        const MapPo &newMap = SegmentationDataBase::instance().installMap(params.getMapName());
//        SegmentationDataBase::instance().loadMainMap();

        ExplorationCenter::instance().repaintCoveragePath(true);

        MapInfo param(1, params.getMapName());
        return param;
    } else {
        throw app::exception(make_error_code(error::create_map_fail));
    }
}

vector<MapInfo> GetMultiMapsStrategy::handler(string params) {
    return std::vector<MapInfo>();
}

int ChangeMapStrategy::handler(string params) {
    return 5;
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
    //更新costmap
    std::string local_costmap =
            "rosparam load " + path::prohibition_areas_path() + " /move_base/local_costmap/costmap_prohibition_layer";
    std::string global_costmap =
            "rosparam load " + path::prohibition_areas_path() + " /move_base/global_costmap/costmap_prohibition_layer";
    std::system(local_costmap.data());
    std::system(global_costmap.data());
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
    std_msgs::Int8 map_start;
    map_start.data = 2;
    PublishInnerManager::instance().publishKnobTask(map_start);
    return 5;
}

int ManualPushResetStrategy::handler(string params) {
    LOG(INFO) << "MapStrategy manual_push_reset ...";
    std_msgs::Int8 map_start;
    map_start.data = 0;
    PublishInnerManager::instance().publishKnobTask(map_start);
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
    return "";
}

string MapApplyIncreaseArea::handler(vector<int> params) {
    MapModification mapModification;
    mapModification.applyIncreaseArea(params);
    return "";
}
