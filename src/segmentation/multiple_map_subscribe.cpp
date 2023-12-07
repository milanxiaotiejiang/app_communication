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
    try {
        int data = flag.data;
        MapScore mapScore;
        if (data == 0) {
            LOG(ERROR) << "input data must > 0 "
                          "\n 1 try_to_enter."
                          "\n 2 Start drawing map."
                          "\n 3 The map needs to be rebuilt and the construction is over."
                          "\n 4 End mapping and replace existing map."
                          "\n 5 End mapping, replace existing maps, and delete relevant resources from existing maps."
                          "\n >5 Create maps separately as name_map_0'data'.";
        } else if (data == 1) {
            RobotTryEnterModeStrategy robotTryEnterModeStrategy;
            robotTryEnterModeStrategy.handler(0);
        } else if (data == 2) {
            StartMapStrategy startMapStrategy;
            startMapStrategy.handler("");
        } else if (data == 3) {
            // 地图创建失败，需要重新创建
            BuildMapParam mapParam(false, false, false, "");
            EndMapStrategy endMapStrategy;
            mapScore = endMapStrategy.handler(mapParam);
        } else if (data == 4) {
            // 需要重新绘制当前地图，并且不删除相关资源
            BuildMapParam mapParam(true, false, false, "");
            EndMapStrategy endMapStrategy;
            mapScore = endMapStrategy.handler(mapParam);
        } else if (data == 5) {
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
    try {
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
    try {
        if (flag.data == 1) {
            SegmentationDataBase::instance().removeBuild();
            long buildId = SegmentationDataBase::instance().saveBuild("B6");
            const std::vector<MapPo> &allMap = SegmentationDataBase::instance().loadAllMap();
            for (const auto &item: allMap) {
                SegmentationDataBase::instance().attachBuildMap(buildId, item.id);
            }
            auto vector = SegmentationDataBase::instance().findBuildMaps(buildId);
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
