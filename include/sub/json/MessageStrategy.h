//
// Created by lijiang on 2021/12/18.
//

#ifndef APP_COMMUNICATION_MESSAGESTRATEGY_H
#define APP_COMMUNICATION_MESSAGESTRATEGY_H

#include "ros/ros.h"
#include "ros/package.h"
#include <std_msgs/String.h>

#include "string"
#include <iostream>

#include "net/base/BaseMethod.h"
#include "net/base/BaseResult.h"
#include "net/base/RequestModel.h"
#include "BaseThrowable.h"

#include "manager/PublishInnerManager.h"
#include "manager/PublishOutManager.h"

#include <nlohmann/json.hpp>

#include "simulation.h"

#include "tool/write_file.hpp"
#include "yaml-cpp/yaml.h"
#include "manager/cloud_robot_control.h"
#include "manager/NoticeManager.h"
#include "model/MapInfo.h"

using json = nlohmann::json;

namespace MessageSource {
    enum {
        WebSocket,
        Cloud
    };
}

class MessageBaseStrategy {
public:
    MessageBaseStrategy() = default;

    virtual ~MessageBaseStrategy() = default;

    virtual void dateProgressing(int source, json &jdecode) = 0;
};

template<class Model, class Result>
class MessageStrategy : public MessageBaseStrategy {
protected:
    int source_ = -1;
public:
    MessageStrategy<Model, Result>() = default;

    ~MessageStrategy<Model, Result>() override = default;

    void dateProgressing(int source, json &jdecode) override {
        source_ = source;
        RequestModel<BaseMethod<Model>> requestModel = jdecode.get<RequestModel<BaseMethod<Model>>>();
        auto message = requestModel.getMsg();
        auto id = message.getId();
        auto params = message.getParams();

        try {
            Result result = handler(params);
            BaseResult<Result> baseResult(id, result);
            parseAfter(source, baseResult);
        } catch (app::exception const &e) {
            parseError(source, id, e.code().value(), e.what());
        } catch (const std::exception &e) {
            LOG(ERROR) << e.what();
            parseError(source, id, error::general, e.what());
        } catch (...) {
            LOG(ERROR) << "MessageStrategy other start exception";
        }

    }

    virtual Result handler(Model params) = 0;

    void parseError(int source, int id, int errorCode, const std::string &errorMessage) {
        BaseResult<std::string> error(id, errorCode, errorMessage);
        if (source == MessageSource::WebSocket) {
            RequestModel<BaseResult<std::string>> requestModel(
                    "publish", "/response_json", error
            );
            json jsonResult = requestModel;
            PublishOutManager::instance().publishJson(jsonResult.dump());
        } else if (source == MessageSource::Cloud) {
            json bJson = error;
            CloudRobotControl::instance().saveInfo(bJson.dump());
        } else {
            LOG(ERROR) << "parseError source is : " << source << " , no find";
        }

    }

    void parseAfter(int source, BaseResult<Result> &baseResult) {
        if (source == MessageSource::WebSocket) {
            RequestModel<BaseResult<Result>> requestModel(
                    "publish", "/response_json", baseResult
            );
            json jsonResult = requestModel;
            PublishOutManager::instance().publishJson(jsonResult.dump());
        } else if (source == MessageSource::Cloud) {
            json bJson = baseResult;
            CloudRobotControl::instance().saveInfo(bJson.dump());
        } else {
            LOG(ERROR) << "parseError source is : " << source << " , no find";
        }

    }

    MultiMapInfo loadMapForId(const std::string &map_id) {
        auto map = SegmentationDataBase::instance().loadMapForId(map_id);

        auto buildMaps = SegmentationDataBase::instance().findBuildMapsForMap(map_id);

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

        return MultiMapInfo(map.id, map.name, map.main, map.path,

                            map.elevator,

                            map.elevator_position_x,
                            map.elevator_position_y,
                            map.elevator_position_z,
                            map.elevator_orientation_x,
                            map.elevator_orientation_y,
                            map.elevator_orientation_z,
                            map.elevator_orientation_w,

                            map.elevator_inside_position_x,
                            map.elevator_inside_position_y,
                            map.elevator_inside_position_z,
                            map.elevator_inside_orientation_x,
                            map.elevator_inside_orientation_y,
                            map.elevator_inside_orientation_z,
                            map.elevator_inside_orientation_w,

                            map.p1x,
                            map.p1y,
                            map.p2x,
                            map.p2y,
                            map.p3x,
                            map.p3y,
                            map.p4x,
                            map.p4y,

                            map.floor,
                            map.base_station,

                            mapAttribute.originPoint.x,
                            mapAttribute.originPoint.y,
                            mapAttribute.originPose.position.x,
                            mapAttribute.originPose.position.y,
                            mapAttribute.originPoint.y,

                            mapAttribute.mapCols,
                            mapAttribute.mapRows,

                            buildId, buildName);
    }
};

class MessageStringStrategy : public MessageStrategy<std::string, std::string> {
public:
    ~MessageStringStrategy() override = default;

    std::string handler(std::string params) override {
        handler();
        return "";
    }

    virtual void handler() = 0;
};

#endif //APP_COMMUNICATION_MESSAGESTRATEGY_H
