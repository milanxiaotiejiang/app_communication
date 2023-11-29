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
    gate_manager_ = handle.subscribe("/gate_manager", 1,
                                     &MultipleMapSubscribe::gateManagerSubscribeCallback,
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

void MultipleMapSubscribe::gateManagerSubscribeCallback(const std_msgs::String &flag) {
    const cv::Mat &map = SegmentationCenter::instance().generateMat();
    MapPo &po = SegmentationDataBase::instance().getDbMap();

    std::cout << "-------------- --------------" << std::endl;
    try {
        std::string data = flag.data;

        std::cout << data << std::endl;

        if (data == "add_gate") {

            int height = 250;
            auto pl = MapAttributeSingleton::instance().mapPoint2RosPoint(map.rows, map.cols,
                                                                          cv::Point(180, height - 15));
            auto pr = MapAttributeSingleton::instance().mapPoint2RosPoint(map.rows, map.cols,
                                                                          cv::Point(180, height + 15));


            GateInfo gateInfo(-1, po.id, 15, height, 280, height,
                              pl.getX(), pl.getY(), 0, 0, 0, 0, 0,
                              pr.getX(), pr.getY(), 0, 0, 0, 0, 0,
                              "59a9dbd3c8424bf598ff71ca5bb0be6e", "9b40dce9ebcf440f8290112b36e69f6f", "01A");


            std::cout << "params : --------------" << std::endl;
            json inJson = gateInfo;
            auto inStr = inJson.dump();
            std::cout << inStr << std::endl;

            AddGateStrategy addGateStrategy;
            int handler = addGateStrategy.handler(gateInfo);

            std::cout << "result : --------------" << std::endl;
            json outJson = handler;
            auto outStr = outJson.dump();
            std::cout << outStr << std::endl;

        } else if (data == "add_gate_1") {
            int height = 110;
            auto pl = MapAttributeSingleton::instance().mapPoint2RosPoint(map.rows, map.cols,
                                                                          cv::Point(180, height - 15));
            auto pr = MapAttributeSingleton::instance().mapPoint2RosPoint(map.rows, map.cols,
                                                                          cv::Point(180, height + 15));


            GateInfo gateInfo(-1, po.id, 15, height, 280, height,
                              pl.getX(), pl.getY(), 0, 0, 0, 0, 0,
                              pr.getX(), pr.getY(), 0, 0, 0, 0, 0,
                              "59a9dbd3c8424bf598ff71ca5bb0be6e", "9b40dce9ebcf440f8290112b36e69f6f", "01A");


            std::cout << "params : --------------" << std::endl;
            json inJson = gateInfo;
            auto inStr = inJson.dump();
            std::cout << inStr << std::endl;

            AddGateStrategy addGateStrategy;
            int handler = addGateStrategy.handler(gateInfo);

            std::cout << "result : --------------" << std::endl;
            json outJson = handler;
            auto outStr = outJson.dump();
            std::cout << outStr << std::endl;
        } else if (data == "delete_gate") {
            auto gateList = SegmentationDataBase::instance().loadGate(po.id);
            long id = gateList[0].id;

            std::cout << "params : --------------" << std::endl;
            json inJson = id;
            auto inStr = inJson.dump();
            std::cout << inStr << std::endl;

            DeleteGateStrategy deleteGateStrategy;
            const std::string &handler = deleteGateStrategy.handler(id);

            std::cout << "result : --------------" << std::endl;
            json outJson = handler;
            auto outStr = outJson.dump();
            std::cout << outStr << std::endl;

        } else if (data == "purge_gate") {
            PurgeGateStrategy purgeGateStrategy;
            purgeGateStrategy.handler();
        } else if (data == "modify_gate") {
            int height = 250;
            auto pl = MapAttributeSingleton::instance().mapPoint2RosPoint(map.rows, map.cols,
                                                                          cv::Point(180, height - 15));
            auto pr = MapAttributeSingleton::instance().mapPoint2RosPoint(map.rows, map.cols,
                                                                          cv::Point(180, height + 15));


            GateInfo gateInfo(-1, po.id, 15, height, 280, height,
                              pl.getX(), pl.getY(), 0, 0, 0, 0, 0,
                              pr.getX(), pr.getY(), 0, 0, 0, 0, 0,
                              "59a9dbd3c8424bf598ff71ca5bb0be6e", "9b40dce9ebcf440f8290112b36e69f6f", "01A");

            std::cout << "params : --------------" << std::endl;
            json inJson = gateInfo;
            auto inStr = inJson.dump();
            std::cout << inStr << std::endl;

            ModifyGateStrategy modifyGateStrategy;
            long handler = modifyGateStrategy.handler(gateInfo);

            std::cout << "result : --------------" << std::endl;
            json outJson = handler;
            auto outStr = outJson.dump();
            std::cout << outStr << std::endl;

        } else if (data == "list_gate") {
            std::cout << "params : --------------" << std::endl;
            json inJson = "";
            auto inStr = inJson.dump();
            std::cout << inStr << std::endl;

            ListGateStrategy listGateStrategy;
            const std::vector<GateInfo> &handler = listGateStrategy.handler("");

            std::cout << "result : --------------" << std::endl;
            json outJson = handler;
            auto outStr = outJson.dump();
            std::cout << outStr << std::endl;

        } else if (data == "query_id_gate") {
            //
        } else if (data == "multiple_modify_gate") {
            auto gateList = SegmentationDataBase::instance().loadGate(po.id);
            long id = gateList[0].id;
            int height = 110;
            GateSimpleInfo gateSimpleInfo(id, po.id, 15, height, 280, height,
                                          "59a9dbd3c8424bf598ff71ca5bb0be6e", "9b40dce9ebcf440f8290112b36e69f6f",
                                          "01A");

            std::cout << "params : --------------" << std::endl;
            json inJson = gateSimpleInfo;
            auto inStr = inJson.dump();
            std::cout << inStr << std::endl;

            MultipleModifyGateStrategy multipleModifyGateStrategy;
            long handler = multipleModifyGateStrategy.handler(gateSimpleInfo);

            std::cout << "result : --------------" << std::endl;
            json outJson = handler;
            auto outStr = outJson.dump();
            std::cout << outStr << std::endl;
        } else if (data == "multiple_list_gate") {
            std::cout << "params : --------------" << std::endl;
            json inJson = po.id;
            auto inStr = inJson.dump();
            std::cout << inStr << std::endl;

            MultipleListGateStrategy multipleListGateStrategy;
            const std::vector<GateInfo> &handler = multipleListGateStrategy.handler(po.id);

            std::cout << "result : --------------" << std::endl;
            json outJson = handler;
            auto outStr = outJson.dump();
            std::cout << outStr << std::endl;

        } else if (data == "multiple_purge_gate") {
            std::cout << "params : --------------" << std::endl;
            json inJson = po.id;
            auto inStr = inJson.dump();
            std::cout << inStr << std::endl;

            MultiplePurgeGateStrategy multiplePurgeGateStrategy;
            const std::string &handler = multiplePurgeGateStrategy.handler(po.id);

            std::cout << "result : --------------" << std::endl;
            json outJson = handler;
            auto outStr = outJson.dump();
            std::cout << outStr << std::endl;

        }
    } catch (app::exception const &e) {
        LOG(ERROR) << e.what();
    } catch (const std::exception &e) {
        LOG(ERROR) << e.what();
    } catch (...) {
        LOG(ERROR) << "MessageStrategy other start exception";
    }

    std::cout << "-------------- --------------" << std::endl;
}
