//
// Created by io on 23-9-21.
//

#include "task/framework/AsyncGateFramework.h"
#include "future/async_call.h"
#include "task/point_planner.h"
#include "manager/PublishInnerManager.h"
#include <iostream>
#include <string>
#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include "model/nebula_model.h"

using json = nlohmann::json;

AsyncGateImplement::AsyncGateImplement() {
    int err = AsyncGateImplement::make_thread(run, this);
//    LOG_IF(INFO, DEBUG_GATE) << "AsyncGateImplement : " << err;
}

std::string UrlEncode(const std::string &value) {
    CURL *curl = curl_easy_init();
    if (curl) {
        char *output = curl_easy_escape(curl, value.c_str(), static_cast<int>(value.length()));
        std::string result(output);
        curl_free(output);
        curl_easy_cleanup(curl);
        return result;
    }
    return "";
}

std::string ConvertMapToUrlParams(const std::map<std::string, std::string> &data) {
    std::string params;
    for (auto it = data.begin(); it != data.end(); ++it) {
        if (it != data.begin()) {
            params += "&";
        }
        params += UrlEncode(it->first) + "=" + UrlEncode(it->second);
    }
    return params;
}

size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    ((std::string *) userp)->append((char *) contents, size * nmemb);
    return size * nmemb;
}

std::string HttpGet(const std::string &url) {
    CURL *curl;
    CURLcode res;
    std::string readBuffer;

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 5);
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10);
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
    }
    curl_global_cleanup();
    LOG_IF(INFO, DEBUG_GATE) << "HttpGet  " << readBuffer;
    return readBuffer;
}

std::string
HttpPost(const std::string &url, const std::string &token,
         const json &postData/*const std::map<std::string, std::string> &postData*/) {
    // 将Map数据转换为URL编码的字符串
//    std::string urlParams = ConvertMapToUrlParams(postData);

    const std::basic_string<char> &dumpStr = postData.dump();
    const char *bodyStr = dumpStr.c_str();
    LOG_IF(INFO, DEBUG_GATE) << "AsyncGateImplement  " << postData.dump().c_str();
    LOG_IF(INFO, DEBUG_GATE) << "AsyncGateImplement  " << token;

    CURL *curl;
    CURLcode res;
    std::string readBuffer;

    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, ("X-Access-Token: " + token).c_str());
//    headers = curl_slist_append(headers, "Content-Type: application/x-www-form-urlencoded; charset=UTF-8");
    headers = curl_slist_append(headers, "Accept: application/json");
    headers = curl_slist_append(headers, "Content-Type: application/json");
    headers = curl_slist_append(headers, "charsets: utf-8");

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();
    if (curl) {
        //设置请求为post请求
        curl_easy_setopt(curl, CURLOPT_POST, 1);
        //设置超时时间
        curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 5);
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10);
        //设置ssl验证
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, false);
        //设置请求的URL地址
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        //设置post请求的参数
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, bodyStr);
        //设置数据接收和写入函数
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

        //CURLOPT_VERBOSE的值为1时，会显示详细的调试信息
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 0);

        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
    }
    curl_global_cleanup();
    LOG_IF(INFO, DEBUG_GATE) << "HttpPost  " << readBuffer;
    return readBuffer;
}

/*
 * curl -X POST -H "charsets: utf-8" -H "Content-Type: application/json" -H "X-Access-Token: eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJleHAiOjE2OTc3NjkzOTcsInVzZXJuYW1lIjoicm9ib3QifQ.Rv6pR9dRr8c41peGJPPnfJE5mvLXzhMJhO6DdDhSw0A" --data-raw '{"deviceName":"1","factoryId":"01A","uuid":"59a9dbd3c8424bf598ff71ca5bb0be6e"}' http://192.168.2.53:8080/nebula/gate/openGate
//        std::map<std::string, std::string> postData;
//        postData["deviceName"] = "1";
//        postData["uuid"] = "59a9dbd3c8424bf598ff71ca5bb0be6e";
//        postData["factoryId"] = "01A";
 */
bool AsyncGateImplement::openGate(const std::string &gate_factory_id, const std::string &gate_uuid) {
    try {
        LOG_IF(INFO, DEBUG_GATE) << "AsyncGateImplement  发送打开闸机的命令 "
                                 << " gate_factory_id : " << gate_factory_id
                                 << " , gate_uuid : " << gate_uuid << " ... ";
//        PublishInnerManager::instance().pubOpenGate();

        if (!Environment::instance().isRealEnvironment) {
            return true;
        }

        std::string tokenUrl = Environment::instance().nebula_base_url + "/api/v1/auth?account=" +
                               Environment::instance().nebula_account + "&secret=" +
                               Environment::instance().nebula_secret;
        std::string gateUrl = Environment::instance().nebula_base_url + "/gate/openGate";

        std::string tokenResponse = HttpGet(tokenUrl);
        json tokenJson = json::parse(tokenResponse);
        CloudModel<CloudToken> cloudModelToken = tokenJson.get<CloudModel<CloudToken>>();
        if (cloudModelToken.isSuccess()) {
            const CloudToken &cloudToken = cloudModelToken.getResult();
            std::string token = cloudToken.getToken();

            json postData = {
                    {"deviceName", Environment::instance().device_name},
                    {"uuid",       gate_uuid},
                    {"factoryId",  gate_factory_id}
            };
            std::string openGateResponse = HttpPost(gateUrl, token, postData);

            json openGateJson = json::parse(openGateResponse);
            CloudModel<std::string> cloudModelOpenGate = openGateJson.get<CloudModel<std::string>>();
            if (cloudModelOpenGate.isSuccess()) {
                return true;
            } else {
                LOG_IF(INFO, DEBUG_GATE)
                                << "AsyncGateImplement  request open gate fail " << cloudModelOpenGate.getMessage()
                                << "... ";
            }
        } else {
            LOG_IF(INFO, DEBUG_GATE)
                            << "AsyncGateImplement  request token fail " << cloudModelToken.getMessage() << "... ";
        }
    } catch (...) {
        LOG_IF(INFO, DEBUG_GATE)
                        << "AsyncGateImplement  request token catch " << "... ";
    }
    return false;
}

bool AsyncGateImplement::closeGate(const std::string &gate_factory_id, const std::string &gate_uuid) {
    try {
        LOG_IF(INFO, DEBUG_GATE) << "AsyncGateImplement  发送关闭闸机的命令 "
                                 << " gate_factory_id : " << gate_factory_id
                                 << " , gate_uuid : " << gate_uuid << " ... ";
//    PublishInnerManager::instance().pubCloseGate();

        if (!Environment::instance().isRealEnvironment) {
            return true;
        }

        std::string tokenUrl = Environment::instance().nebula_base_url + "/api/v1/auth?account=" +
                               Environment::instance().nebula_account + "&secret=" +
                               Environment::instance().nebula_secret;
        std::string gateUrl = Environment::instance().nebula_base_url + "/gate/closeGate";

        std::string tokenResponse = HttpGet(tokenUrl);
        json tokenJson = json::parse(tokenResponse);
        CloudModel<CloudToken> cloudModelToken = tokenJson.get<CloudModel<CloudToken>>();
        if (cloudModelToken.isSuccess()) {
            const CloudToken &cloudToken = cloudModelToken.getResult();
            std::string token = cloudToken.getToken();

            json postData = {
                    {"deviceName", Environment::instance().device_name},
                    {"uuid",       gate_uuid},
                    {"factoryId",  gate_factory_id}
            };
            std::string openGateResponse = HttpPost(gateUrl, token, postData);

            json openGateJson = json::parse(openGateResponse);
            CloudModel<std::string> cloudModelOpenGate = openGateJson.get<CloudModel<std::string>>();
            if (cloudModelOpenGate.isSuccess()) {
                return true;
            } else {
                LOG_IF(INFO, DEBUG_GATE)
                                << "AsyncGateImplement  request open gate fail " << cloudModelOpenGate.getMessage()
                                << "... ";
            }
        } else {
            LOG_IF(INFO, DEBUG_GATE)
                            << "AsyncGateImplement  request token fail " << cloudModelToken.getMessage() << "... ";
        }
    } catch (...) {
        LOG_IF(INFO, DEBUG_GATE)
                        << "AsyncGateImplement  request token catch " << "... ";
    }
    return false;
}

[[noreturn]] void AsyncGateImplement::execute() {
    for (;;) {
        std::unique_lock<std::mutex> lock(cv_mut);

        cv.wait(lock, [this]() {
            return next;
        });
        next = false;

        LOG_IF(INFO, DEBUG_GATE)
                        << "AsyncGateImplement  totalPointCount : " << totalPointCount << " , progressPointCount : "
                        << progressPointCount;
        if (pointEpollDeque.empty()) {
            implementing = false;
            if (progressPointCount == totalPointCount) {
                //执行完成
                LOG_IF(INFO, DEBUG_GATE) << "AsyncGateImplement  执行完成 ";
                callbackImplementEnd(true, progressPointCount);
            } else {
                callbackImplementEnd(false, progressPointCount);
            }
        } else {
            if (progressPointCount == 0) {
                implementing = true;
                callbackImplementStart();
            }
            RealPoint &front = pointEpollDeque.front();
            if (front.gateControl == GATE_OPEN) {
                LOG_IF(INFO, DEBUG_GATE) << "AsyncGateImplement  准备开启闸机 ... ";

                bool isOpen = false;
                int count = 0;
                bool end_loop = false;
                while (!end_loop) {
                    isOpen = openGate(front.gate_factory_id, front.gate_uuid);
                    if (isOpen) {
                        end_loop = true;
                    } else {
                        LOG_IF(INFO, DEBUG_GATE) << "AsyncGateImplement  第 " << count << " 打开闸机失败，继续重试 ... ";
                        count++;
                        if (count > 5) {
                            end_loop = true;
                        }
                        sleep(1);
                    }
                }
                if (isOpen) {
                    PointPlanner::instance().goToPoint(front);
                } else {
                    LOG_IF(INFO, DEBUG_GATE) << "AsyncGateImplement  开启闸机失败 ... ";
                    pointEpollDeque.clear();
                    implementing = false;
                    callbackImplementEnd(false, progressPointCount);
                }
            } else if (front.gateControl == GATE_CLOSE) {
                LOG_IF(INFO, DEBUG_GATE) << "AsyncGateImplement  准备关闭闸机 ... ";
                closeGate(front.gate_factory_id, front.gate_uuid);
                PointPlanner::instance().goToPoint(front);
            } else {
                PointPlanner::instance().goToPoint(front);
            }

        }

    }
}

void AsyncGateImplement::onImplementStart(const std::vector<RealPoint> &points) {
    {
        std::unique_lock<std::mutex> lock(cv_mut);

        pointEpollDeque.clear();

        progressPointCount = 0;
        for (const auto &point: points) {
            pointEpollDeque.push_back(point);
        }
        totalPointCount = pointEpollDeque.size();

        next = true;
        cv.notify_one();
    }
}

void AsyncGateImplement::cancelImplement() {
    if (!implementing) {
        return;
    }
    {
        std::unique_lock<std::mutex> lock(cv_mut);

        pointEpollDeque.clear();

        progressPointCount = -1;

        next = true;
        cv.notify_one();
    }
}

bool AsyncGateImplement::isImplement() {
    return implementing;
}

void AsyncGateImplement::executeOnPointDone(event::error error) {
    if (!implementing) {
        return;
    }
    {
        std::unique_lock<std::mutex> lock(cv_mut);

        if (error == event::SUCCEEDED) {
            if (!pointEpollDeque.empty()) {
                pointEpollDeque.pop_front();
                progressPointCount++;
            }
        }

        next = true;
        cv.notify_one();
    }
}

void AsyncGateImplement::executeOnPathDone(event::error error) {
    if (!implementing) {
        return;
    }
    {
        std::unique_lock<std::mutex> lock(cv_mut);

        if (error == event::SUCCEEDED) {
            if (!pointEpollDeque.empty()) {
                pointEpollDeque.pop_front();
                progressPointCount++;
            }
        }

        next = true;
        cv.notify_one();
    }
}

void AsyncGateImplement::setCallbackImplementStart(const std::function<void()> &callback) {
    AsyncGateImplement::callbackImplementStart = callback;
}

void AsyncGateImplement::setCallbackImplementEnd(const std::function<void(bool, int)> &callback) {
    AsyncGateImplement::callbackImplementEnd = callback;
}

//*******************************************************************************************************

AsyncGateDistribution::AsyncGateDistribution() : mImplement(std::make_shared<AsyncGateImplement>()) {
    int err = AsyncGateDistribution::make_thread(run, this);
//    LOG_IF(INFO, DEBUG_GATE) << "AsyncGateDistribution : " << err;

    mImplement->setCallbackImplementStart([this]() {
        LOG_IF(INFO, DEBUG_GATE) << "闸机规划点位开始执行 ... ";
        callbackDistributionStart();
    });
    mImplement->setCallbackImplementEnd([this](bool b, int progress) {
        LOG_IF(INFO, DEBUG_GATE) << "通过闸机结果 " << b << " , progress : " << progress;
        callbackDistributionResult(b, progress);
    });
}

[[noreturn]] void AsyncGateDistribution::execute() {
    for (;;) {
        std::unique_lock<std::mutex> lock(cv_mut);
        cv.wait(lock, [this] {
            return !pointList.empty();
        });

        mImplement->onImplementStart(pointList);
        pointList.clear();
    }
}

void AsyncGateDistribution::onDistributionStart(const std::vector<RealPoint> &points) {
    {
        std::unique_lock<std::mutex> lock(cv_mut);

        pointList.clear();
        for (const auto &point: points) {
            pointList.push_back(point);
        }

        cv.notify_one();
    }
}

void AsyncGateDistribution::cancelDistribution() {
    {
        mImplement->cancelImplement();
    }
}

bool AsyncGateDistribution::isImplement() {
    return mImplement->isImplement();
}

void AsyncGateDistribution::executeOnPointDone(event::error error) {
    mImplement->executeOnPointDone(error);
}

void AsyncGateDistribution::executeOnPathDone(event::error error) {
    mImplement->executeOnPathDone(error);
}

void AsyncGateDistribution::setCallbackDistributionStart(const std::function<void()> &callbackDistributionStart) {
    AsyncGateDistribution::callbackDistributionStart = callbackDistributionStart;
}

void AsyncGateDistribution::setCallbackDistribution(const std::function<void(bool, int)> &callback) {
    AsyncGateDistribution::callbackDistributionResult = callback;
}
