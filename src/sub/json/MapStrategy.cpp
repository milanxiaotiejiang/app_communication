//
// Created by lijiang on 2021/12/18.
//

#include "sub/json/MapStrategy.h"
#include "segmentation/SegmentationCenter.h"
#include "segmentation/map_attribute.h"
#include "exploration/ExplorationCenter.h"

MapInfo SaveMapStrategy::handler(MapInfo params) {

    MapAttribute::instance().setCreatingMap(true);

    std_msgs::String map_save;
    map_save.data.append("save_map");
    for (int i = 0; i < 5; i++) {
        PublishInnerManager::instance().getPubInner()->publishCommand(map_save);
        ros::Duration(1).sleep();
    }

    //回复，带参数，包括分配的id
    MapInfo param(1, params.getMapName());

    return param;
}

vector<MapInfo> GetMultiMapsStrategy::handler(string params) {
    string fileName;
    string sss;
    fileName.append(ros::package::getPath("app_communication"));
    fileName.append("/config/map_info.txt");
    // fileName.append("/home/admin1/test_ws/src/app_communication/config/map_info.txt");
    //sh::File *fff = new sh::File(fileName);
    std::shared_ptr<sh::File> fff = make_shared<sh::File>(fileName);
    string base64;
    if (fff->open(std::ios::in)) {
        sss = fff->readAll();
    } else {
        cout << "fail to open file" << endl;
    }
    std_msgs::String result;
    if (sss.length() > 0)//不为空
    {
        json jdecode = json::parse(sss);
        std::vector<MapInfo> map_info = jdecode.get<std::vector<MapInfo>>();//数据内容，结构体格式

        return map_info;
    } else {
        return std::vector<MapInfo>();
    }

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
    MapAttribute::instance().resetProhibition();
    MapAttribute::instance().loadVirtualWall();
    MapAttribute::instance().loadPenaltyZone();
    ExplorationCenter::instance().repaintCoveragePath();
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

    std_msgs::Int8 map_start;
    map_start.data = 2;
    for (int i = 0; i < 5; i++) {
        PublishInnerManager::instance().getPubInner()->publishKnobTask(map_start);
        ros::Duration(1).sleep();
    }

    return 5;
}

int ManualPushResetStrategy::handler(string params) {
    std_msgs::Int8 map_start;
    map_start.data = 0;
    for (int i = 0; i < 5; i++) {
        PublishInnerManager::instance().getPubInner()->publishKnobTask(map_start);
        ros::Duration(1).sleep();
    }
    return 5;
}

MapInfo ManualPushSaveStrategy::handler(MapInfo params) {
    std_msgs::String map_save;
    map_save.data.append("save_map");
    for (int i = 0; i < 5; i++) {
        PublishInnerManager::instance().getPubInner()->publishCommand(map_save);
        ros::Duration(1).sleep();
    }
    MapInfo param(1, params.getMapName());
    std_msgs::Int8 map_start;
    map_start.data = 0;
    for (int i = 0; i < 5; i++) {
        PublishInnerManager::instance().getPubInner()->publishKnobTask(map_start);
        ros::Duration(1).sleep();
    }

    return param;
}

