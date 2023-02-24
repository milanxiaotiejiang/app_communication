//
// Created by Looper on 2023/2/24.
//

#include "manager/PublishOutManager.h"
#include "net/WsServerManager.h"

void PublishOutManager::initialize(ros::NodeHandle handle) {
    pub_response_ = handle.advertise<std_msgs::String>(RESPONSE, 1);
    pub_response_json_ = handle.advertise<std_msgs::String>(RESPONSE_JSON, 1);
    pub_robot_status_ = handle.advertise<std_msgs::String>(ROBOT_STATUS, 10);
    pub_material_status_ = handle.advertise<std_msgs::String>(MATERIAL_STATUS, 10);
    pub_map_ = handle.advertise<nav_msgs::OccupancyGrid>(MAP_APP, 10);
    pub_grid_map_ = handle.advertise<nav_msgs::OccupancyGrid>(GRID_MAP_APP, 10);

    pub_marker_ = handle.advertise<visualization_msgs::Marker>(WAYPOINTS_MARKER, 10);
    pub_scan_ = handle.advertise<sensor_msgs::LaserScan>(SCAN_APP, 10);
    pub_path_ = handle.advertise<nav_msgs::Path>(PATH_TEST, 10);
    pub_self_check_ = handle.advertise<std_msgs::String>(CHECK_APP, 1);
    pub_notice_ = handle.advertise<std_msgs::String>(NOTICE_APP, 1);
    pub_cloud_status = handle.advertise<std_msgs::String>("/cloud_robot_status", 10);
    pub_cloud_event = handle.advertise<clean_msgs::cloud_robot_event>("/cloud_robot_event", 10);
    pub_internal_event_ = handle.advertise<std_msgs::String>(INTERNAL_EVENT, 10);

    acceptAppJsonV1 = handle.advertise<std_msgs::String>(APP_JSON, 1);
    acceptAppJsonV2 = handle.advertise<std_msgs::String>(APP_JSON_V2, 1);
    acceptAppSchedule = handle.advertise<std_msgs::String>(APP_SCHEDULE, 1);
    acceptAppError = handle.advertise<std_msgs::String>(APP_ERROR, 1);
    acceptAppCommunication = handle.advertise<std_msgs::String>(APP_COMMUNICATION, 1);

    pub_knob_ = handle.advertise<std_msgs::String>(KNOB_APP, 10);
}

void PublishOutManager::publishResponse(const std_msgs::String &message) const {
    // pub_response_.publish(message);
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

void PublishOutManager::publishMaterialStatus(const VersionSubscribe<MaterialStatus> &versionSubscribe) const {
    RequestModel<VersionSubscribe<MaterialStatus>> requestModel(
            "publish", MATERIAL_STATUS, versionSubscribe
    );

    json jsonResult = requestModel;

    WsServerManager::instance().sendRequestData(MATERIAL_STATUS, jsonResult.dump());

    std_msgs::String result;
    result.data.append(jsonResult.dump());
    pub_material_status_.publish(result);
}

void PublishOutManager::publishMap(const nav_msgs::OccupancyGrid &message) const {
    WsServerManager::instance().setMapApp(message);
    pub_map_.publish(message);
}

void PublishOutManager::publishGridMap(const nav_msgs::OccupancyGrid &message) const {
//    WsServerManager::instance().setGridMapApp(message);
//    pub_grid_map_.publish(message);
}

void PublishOutManager::publishScan(const sensor_msgs::LaserScan &message) const {
    //    pub_scan_.publish(message);
}

void PublishOutManager::publishPath(const nav_msgs::Path &message) const {
    //    pub_path_.publish(message);
}

void PublishOutManager::publishMarkerPoint(const visualization_msgs::Marker &message) const {
    //    pub_marker_.publish(message);
}

void PublishOutManager::publishSelfCheck(const VersionSubscribe<SelfCheckStatus> &versionSubscribe) const {
    RequestModel<VersionSubscribe<SelfCheckStatus>> requestModel;
    requestModel.setOp("publish");
    requestModel.setTopic(CHECK_APP);
    requestModel.setMsg(versionSubscribe);

    json jsonResult = requestModel;

    WsServerManager::instance().sendRequestData(CHECK_APP, jsonResult.dump());

    std_msgs::String result;
    result.data.append(jsonResult.dump());
    pub_self_check_.publish(result);
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

void PublishOutManager::publishAppJson(int version, const std_msgs::String &message) const {
    if (version == APP_JSON_VERSION::V1) {
        acceptAppJsonV1.publish(message);
    } else if (version == APP_JSON_VERSION::V2) {
        acceptAppJsonV2.publish(message);
    }
}

void PublishOutManager::publishAppSchedule(const std_msgs::String &message) const {
    cout << "publishAppSchedule!!!!!!!!!!!!!!! " << endl;
    acceptAppSchedule.publish(message);
}

void PublishOutManager::publishAppError(const std_msgs::String &message) const {
    cout << "publishAppError!!!!!!!!!!!!!!! " << endl;
    acceptAppError.publish(message);
}

void PublishOutManager::publishAppCommunication(const std_msgs::String &message) const {
    acceptAppCommunication.publish(message);
}

void PublishOutManager::publishCloudStatus(const std_msgs::String &message) const {
    pub_cloud_status.publish(message);
}

void PublishOutManager::publishKnob(const VersionSubscribe<KnobStatus> &versionSubscribe) const {
    RequestModel<VersionSubscribe<KnobStatus>> requestModel(
            "publish", KNOB_APP, versionSubscribe
    );

    json jsonResult = requestModel;

    WsServerManager::instance().sendRequestData(KNOB_APP, jsonResult.dump());

    std_msgs::String result;
    result.data.append(jsonResult.dump());
    pub_knob_.publish(result);
}

void PublishOutManager::publishCloudEvent(const clean_msgs::cloud_robot_event &event) const {
    pub_cloud_event.publish(event);
}

void PublishOutManager::publishInternalEvent(const std_msgs::String &message) const {
//    LOG(ERROR) << "publishInternalEvent message : " << message.data;
    pub_internal_event_.publish(message);
}