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

#include "glog/logging.h"

#include "tool/write_file.hpp"
#include "yaml-cpp/yaml.h"
#include "manager/cloud_robot_control.h"

using json = nlohmann::json;
using namespace std;

namespace MessageSource {
    enum {
        WebSocket,
        Cloud
    };
};

class MessageBaseStrategy {
public:
    MessageBaseStrategy() = default;

    virtual ~MessageBaseStrategy() = default;

    virtual void dateProgressing(int source, json &jdecode) = 0;

};

template<class Model, class Result>
class MessageStrategy : public MessageBaseStrategy {
public:
    MessageStrategy<Model, Result>() = default;

    ~MessageStrategy<Model, Result>() override = default;

    void dateProgressing(int source, json &jdecode) override {
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
        BaseResult<string> error(id, errorCode, errorMessage);
        if (source == MessageSource::WebSocket) {
            RequestModel<BaseResult<string>> requestModel(
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
            if (bJson["params"].dump().length() > 2) {
                CloudRobotControl::instance().saveInfo(bJson.dump());
            }
        } else {
            LOG(ERROR) << "parseError source is : " << source << " , no find";
        }

    }

};

class MessageStringStrategy : public MessageStrategy<string, string> {
public:
    ~MessageStringStrategy() override = default;

    string handler(string params) override {
        handler();
        return "";
    }

    virtual void handler() = 0;
};

#endif //APP_COMMUNICATION_MESSAGESTRATEGY_H
