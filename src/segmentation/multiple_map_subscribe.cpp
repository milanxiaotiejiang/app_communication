//
// Created by io on 23-10-31.
//

#include "segmentation/multiple_map_subscribe.h"
#include "BaseThrowable.h"
#include "sub/json/MapStrategy.h"
#include "sub/json/ModeStrategy.h"
#include "sub/json/GateStrategy.h"
#include "segmentation/map_attribute.h"
#include "segmentation/SegmentationCenter.h"
#include "db/segmentation_data_base.h"

const std::string map_0 = "name_map_0";

MultipleMapSubscribe::MultipleMapSubscribe(ros::NodeHandle handle) {
    sub_map_create_ = handle.subscribe("/multiple_map_create", 1,
                                       &MultipleMapSubscribe::multipleMapCreateSubscribeCallback,
                                       this);
    sub_map_switch_ = handle.subscribe("/multiple_map_switch", 1,
                                       &MultipleMapSubscribe::multipleMapSwitchSubscribeCallback,
                                       this);
    build_manager_ = handle.subscribe("/build_manager", 1,
                                      &MultipleMapSubscribe::buildManagerSubscribeCallback,
                                      this);
}

void MultipleMapSubscribe::multipleMapCreateSubscribeCallback(const std_msgs::Int32 &flag) {
    try {//multiple_map_create
        int data = flag.data;
        MapScore mapScore;
        if (data == 0) {
            LOG(ERROR) << "input data must > 0 "
                          "\n 1 Start drawing map."
                          "\n 2 The map needs to be rebuilt and the construction is over."
                          "\n 3 End mapping and replace existing map."
                          "\n 4 End mapping, replace existing maps, and delete relevant resources from existing maps."
                          "\n >4 Create maps separately as name_map_0'data'.";
        } else if (data == 1) {
            StartMapStrategy startMapStrategy;
            startMapStrategy.handler("");
        } else if (data == 2) {
            // 地图创建失败，需要重新创建
            BuildMapParam mapParam(false, false, false, "");
            EndMapStrategy endMapStrategy;
            mapScore = endMapStrategy.handler(mapParam);
        } else if (data == 3) {
            // 需要重新绘制当前地图，并且不删除相关资源
            BuildMapParam mapParam(true, false, false, "");
            EndMapStrategy endMapStrategy;
            mapScore = endMapStrategy.handler(mapParam);
        } else if (data == 4) {
            // 需要重新绘制当前地图，并且删除相关资源
            BuildMapParam mapParam(true, true, false, "");
            EndMapStrategy endMapStrategy;
            mapScore = endMapStrategy.handler(mapParam);
        } else {
            std::string map_name = map_0 + std::to_string(data);
            BuildMapParam mapParam(true, false, true, map_name);
            EndMapStrategy endMapStrategy;
            mapScore = endMapStrategy.handler(mapParam);
        }

        LOG(ERROR) << mapScore;

    } catch (app::exception const &e) {
        LOG(ERROR) << e.what();
    } catch (const std::exception &e) {
        LOG(ERROR) << e.what();
    } catch (...) {
        LOG(ERROR) << "MessageStrategy other start exception";
    }
}

void MultipleMapSubscribe::multipleMapSwitchSubscribeCallback(const std_msgs::String &flag) {
    try {//multiple_map_switch
        std::string data = flag.data;
        if (data.empty()) {
            GetMultiMapsStrategy getMultiMapsStrategy;
            auto maps = getMultiMapsStrategy.handler(data);
            for (const auto &item: maps) {
                LOG(INFO) << item;
            }
            LOG(ERROR) << "Enter the unique id (uuid) of the map to switch maps.";
        } else {
            ChangeMapStrategy changeMapStrategy;
            changeMapStrategy.handler(data);
        }
    } catch (app::exception const &e) {
        LOG(ERROR) << e.what();
    } catch (const std::exception &e) {
        LOG(ERROR) << e.what();
    } catch (...) {
        LOG(ERROR) << "MessageStrategy other start exception";
    }
}

void MultipleMapSubscribe::buildManagerSubscribeCallback(const std_msgs::Int32 &flag) {
    try {//build_manager
        if (flag.data == 0) {
            LOG(ERROR) << "input data "
                          "\n -2、 -1、【1、 18】 Set the floor to data for the current map"
                          "\n 100 Original base station mapping."
                          "\n 101 No base station mapping."
                          "\n 10000 Add elevator points to the current map."
                          "\n 10001 Remove elevator points to the current map."
                          "\n 10010 Set the current map to have no base stations."
                          "\n 10011 Set the current map to have base stations."
                          "\n 100000 Build a building named B6 and associate it with all maps.";
        } else if (flag.data == -2 || flag.data == -1 || flag.data >= 1 && flag.data <= 18) {
            SegmentationDataBase::instance().updateFloor(SegmentationDataBase::instance().getDbMap().id, flag.data);
        } else if (flag.data == 100) {
            Environment::instance().no_station_mapping_mode = false;
            ros::param::set("/no_station_mapping_mode", false);
        } else if (flag.data == 101) {
            Environment::instance().no_station_mapping_mode = true;
            ros::param::set("/no_station_mapping_mode", true);
        } else if (flag.data == 10000) {
            SegmentationDataBase::instance().updateMapElevator(SegmentationDataBase::instance().getDbMap().id);
        } else if (flag.data == 10001) {
            SegmentationDataBase::instance().removeMapElevator(SegmentationDataBase::instance().getDbMap().id);
        } else if (flag.data == 10010) {
            SegmentationDataBase::instance().changeBaseStation(SegmentationDataBase::instance().getDbMap().id,
                                                               false);
        } else if (flag.data == 10011) {
            SegmentationDataBase::instance().changeBaseStation(SegmentationDataBase::instance().getDbMap().id,
                                                               true);
        } else if (flag.data == 100000) {
            SegmentationDataBase::instance().removeBuild();
            long buildId = SegmentationDataBase::instance().saveBuild("B6", 0);
            const std::vector<MapPo> &allMap = SegmentationDataBase::instance().loadAllMap();
            for (const auto &item: allMap) {
                SegmentationDataBase::instance().attachBuildMap(buildId, item.id);
            }
            auto vector = SegmentationDataBase::instance().findBuildMapsForBuild(buildId);
            for (const auto &item: vector) {
                std::cout << "build : " << item.first << " , map : " << item.second << std::endl;
            }
        }
    } catch (app::exception const &e) {
        LOG(ERROR) << e.what();
    } catch (const std::exception &e) {
        LOG(ERROR) << e.what();
    } catch (...) {
        LOG(ERROR) << "MessageStrategy other start exception";
    }

}
