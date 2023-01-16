//
// Created by lijiang on 2021/12/18.
//

#include "pub/PubOut.h"
#include "net/WsServerManager.h"

PubOut::PubOut(ros::NodeHandle handle) : handle(handle) {
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

void PubOut::set_robot_result(string result) const {
    const string robot_result = result;
}

string PubOut::get_robot_result() {
    return robot_result;
}

void PubOut::publishResponse(const std_msgs::String &message) const {
    // pub_response_.publish(message);
}

void PubOut::publishJson(const std::string &message) const {

    WsServerManager::instance().sendData(message);

    std_msgs::String result;
    result.data.append(message);
    pub_response_json_.publish(result);
}

void PubOut::publishStatus(const VersionSubscribe<ShowWorkStatus> &versionSubscribe) const {
    RequestModel<VersionSubscribe<ShowWorkStatus>> requestModel(
            "publish", ROBOT_STATUS, versionSubscribe
    );

    json jsonResult = requestModel;

    WsServerManager::instance().sendRequestData(ROBOT_STATUS, jsonResult.dump());

    std_msgs::String result;
    result.data.append(jsonResult.dump());
    pub_robot_status_.publish(result);
}

void PubOut::publishMaterialStatus(const VersionSubscribe<MaterialStatus> &versionSubscribe) const {
    RequestModel<VersionSubscribe<MaterialStatus>> requestModel(
            "publish", MATERIAL_STATUS, versionSubscribe
    );

    json jsonResult = requestModel;

    WsServerManager::instance().sendRequestData(MATERIAL_STATUS, jsonResult.dump());

    std_msgs::String result;
    result.data.append(jsonResult.dump());
    pub_material_status_.publish(result);
}

void PubOut::publishMap(const nav_msgs::OccupancyGrid &message) const {
    WsServerManager::instance().setMapApp(message);
    pub_map_.publish(message);
}

void PubOut::publishGridMap(const nav_msgs::OccupancyGrid &message) const {
//    WsServerManager::instance().setGridMapApp(message);
//    pub_grid_map_.publish(message);
}

void PubOut::publishScan(const sensor_msgs::LaserScan &message) const {
    //    pub_scan_.publish(message);
}

void PubOut::publishPath(const nav_msgs::Path &message) const {
    //    pub_path_.publish(message);
}

void PubOut::publishMarkerPoint(const visualization_msgs::Marker &message) const {
    //    pub_marker_.publish(message);
}

void PubOut::publishSelfCheck(const VersionSubscribe<SelfCheckStatus> &versionSubscribe) const {
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

void PubOut::publishNotice(const Notice &notice) const {
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

void PubOut::publishAppJson(int version, const std_msgs::String &message) const {
    if (version == APP_JSON_VERSION::V1) {
        acceptAppJsonV1.publish(message);
    } else if (version == APP_JSON_VERSION::V2) {
        acceptAppJsonV2.publish(message);
    }
}

void PubOut::publishAppSchedule(const std_msgs::String &message) const {
    cout << "publishAppSchedule!!!!!!!!!!!!!!! " << endl;
    acceptAppSchedule.publish(message);
}

void PubOut::publishAppError(const std_msgs::String &message) const {
    cout << "publishAppError!!!!!!!!!!!!!!! " << endl;
    acceptAppError.publish(message);
}

void PubOut::publishAppCommunication(const std_msgs::String &message) const {
    acceptAppCommunication.publish(message);
}

void PubOut::publishCloudStatus(const std_msgs::String &message) const {
    pub_cloud_status.publish(message);
}

void PubOut::publishKnob(const VersionSubscribe<KnobStatus> &versionSubscribe) const {
    RequestModel<VersionSubscribe<KnobStatus>> requestModel(
            "publish", KNOB_APP, versionSubscribe
    );

    json jsonResult = requestModel;

    WsServerManager::instance().sendRequestData(KNOB_APP, jsonResult.dump());

    std_msgs::String result;
    result.data.append(jsonResult.dump());
    pub_knob_.publish(result);
}

void PubOut::publishCloudEvent(const clean_msgs::cloud_robot_event &event) const {
    pub_cloud_event.publish(event);
}

void PubOut::publishInternalEvent(const std_msgs::String &message) const{
    pub_internal_event_.publish(message);
}