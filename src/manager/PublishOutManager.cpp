//
// Created by Looper on 2023/2/24.
//

#include "manager/PublishOutManager.h"
#include "net/WsServerManager.h"
#include "net/base/RequestModel.h"
#include "simulation.h"

void PublishOutManager::initialize(ros::NodeHandle handle) {
    pub_response_ = handle.advertise<std_msgs::String>(RESPONSE, 1);
    pub_response_json_ = handle.advertise<std_msgs::String>(RESPONSE_JSON, 1);
    pub_robot_status_ = handle.advertise<std_msgs::String>(ROBOT_STATUS, 10);
    pub_map_ = handle.advertise<nav_msgs::OccupancyGrid>(MAP_APP, 10);

    pub_notice_ = handle.advertise<std_msgs::String>(NOTICE_APP, 1);
    pub_sensor_check_ = handle.advertise<std_msgs::String>(SENSOR_CHECK, 1);
    pub_internal_event_ = handle.advertise<std_msgs::String>(INTERNAL_EVENT, 10);

    acceptAppJsonV1 = handle.advertise<std_msgs::String>(APP_JSON, 1);

    acceptAppCommunication = handle.advertise<std_msgs::String>(APP_COMMUNICATION, 1);
}

void PublishOutManager::publishJson(const std::string &message) const {

    WsServerManager::instance().sendData(message);

    std_msgs::String result;
    result.data.append(message);
    pub_response_json_.publish(result);
}

void PublishOutManager::publishStatus(const VersionSubscribe<ShowWorkStatus> &versionSubscribe) const {
    RequestModel<VersionSubscribe<ShowWorkStatus>> requestModel(
            "publish", ROBOT_STATUS, versionSubscribe
    );

    json jsonResult = requestModel;

    WsServerManager::instance().sendRequestData(ROBOT_STATUS, jsonResult.dump());

    std_msgs::String result;
    result.data.append(jsonResult.dump());
    pub_robot_status_.publish(result);
}

void PublishOutManager::publishMap(const nav_msgs::OccupancyGrid &message) const {
    if (Environment::instance().gzip_map) {
        WsServerManager::instance().setMapApp2(message);
    } else {
        WsServerManager::instance().setMapApp(message);
    }
//    LOG(INFO) << "publishMap  width : " << message.info.width <<
//              " height : " << message.info.height <<
//              " x : " << message.info.origin.position.x <<
//              " y : " << message.info.origin.position.y <<
//              " z : " << message.info.origin.position.z;
    pub_map_.publish(message);
}

void PublishOutManager::publishNotice(const Notice &notice) const {
    RequestModel<Notice> requestModel;
    requestModel.setOp("publish");
    requestModel.setTopic(NOTICE_APP);
    requestModel.setMsg(notice);

    json jsonResult = requestModel;

    WsServerManager::instance().sendRequestData(NOTICE_APP, jsonResult.dump());

    std_msgs::String result;
    result.data.append(jsonResult.dump());
    pub_notice_.publish(result);
}

void PublishOutManager::publishSensorCheck(const SensorSelf &model) const {
    RequestModel<SensorSelf> requestModel;
    requestModel.setOp("publish");
    requestModel.setTopic(SENSOR_CHECK);
    requestModel.setMsg(model);

    json jsonResult = requestModel;

    WsServerManager::instance().sendRequestData(SENSOR_CHECK, jsonResult.dump());

    std_msgs::String result;
    result.data.append(jsonResult.dump());
    pub_sensor_check_.publish(result);
}

void PublishOutManager::publishAppJson(int version, const std_msgs::String &message) const {
    if (version == APP_JSON_VERSION::V1) {
        acceptAppJsonV1.publish(message);
    }
}

void PublishOutManager::publishAlarm(const internal_event::AlarmEvent &alarmEvent) const {
    VersionSubscribe<internal_event::AlarmEvent> versionSubscribe(1, alarmEvent);

    RequestModel<VersionSubscribe<internal_event::AlarmEvent>> requestModel(
            "publish", ALARM_EVENT, versionSubscribe
    );
    json jsonResult = requestModel;
    WsServerManager::instance().sendRequestData(ALARM_EVENT, jsonResult.dump());
}

void PublishOutManager::publishAppCommunication(const std_msgs::String &message) const {
    acceptAppCommunication.publish(message);
}

void PublishOutManager::publishInternalEvent(const std_msgs::String &message) const {
    pub_internal_event_.publish(message);
}
