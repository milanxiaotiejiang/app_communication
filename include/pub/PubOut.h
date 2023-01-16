//
// Created by lijiang on 2021/12/18.
//

#ifndef APP_COMMUNICATION_PUBOUT_H
#define APP_COMMUNICATION_PUBOUT_H

#include<iostream>

#include <geometry_msgs/Pose.h>
#include <geometry_msgs/Twist.h>
#include <nav_msgs/OccupancyGrid.h>
#include <nav_msgs/Path.h>
#include <ros/ros.h>
#include <sensor_msgs/LaserScan.h>
#include <std_msgs/Int16.h>
#include <std_msgs/String.h>
#include <visualization_msgs/Marker.h>
#include <clean_msgs/cloud_robot_event.h>
#include "model/BaseResult.h"
#include "model/TaskPoint.h"
#include "model/VersionSubscribe.h"
#include "model/Notice.h"
#include "model/ShowWorkStatus.h"
#include "model/MaterialStatus.h"
#include "model/KnobStatus.h"
#include "model/SelfCheckStatus.h"

using namespace std;

class PubOut {
private:
    ros::NodeHandle handle;
    ros::Publisher pub_response_,//原协议
    pub_response_json_,  //新协议
    pub_robot_status_,   //机器人状态
    pub_material_status_,//
    pub_map_,            //地图
    pub_grid_map_,       //grid地图
    pub_scan_,           //雷达数据
    pub_path_,           //路径测试
    pub_marker_,         //
    pub_self_check_,     //目标点
    pub_cloud_status,    //云端
    pub_cloud_event,     //
    pub_notice_,         //notice
    pub_knob_,           //knob
    pub_internal_event_;//发送给云端
    ros::Publisher acceptAppJsonV1;
    ros::Publisher acceptAppJsonV2;
    ros::Publisher acceptAppCommunication;
    ros::Publisher acceptAppSchedule; //定时文件内容更新
    ros::Publisher acceptAppError;

public:
    PubOut(ros::NodeHandle handle);

    mutable string robot_result;

    void set_robot_result(string result) const;

    string get_robot_result();

    void publishResponse(const std_msgs::String &message) const;

    void publishJson(const std::string &message) const;

    void publishStatus(const VersionSubscribe<ShowWorkStatus> &versionSubscribe) const;

    void publishMaterialStatus(const VersionSubscribe<MaterialStatus> &versionSubscribe) const;

    void publishMap(const nav_msgs::OccupancyGrid &message) const;

    void publishGridMap(const nav_msgs::OccupancyGrid &message) const;

    void publishScan(const sensor_msgs::LaserScan &message) const;

    void publishMarkerPoint(const visualization_msgs::Marker &message) const;

    void publishPath(const nav_msgs::Path &message) const;

    void publishSelfCheck(const VersionSubscribe<SelfCheckStatus> &versionSubscribe) const;

    void publishNotice(const Notice &notice) const;

    void publishAppJson(const int version, const std_msgs::String &message) const;

    void publishAppCommunication(const std_msgs::String &message) const;

    void publishAppSchedule(const std_msgs::String &message) const;

    void publishAppError(const std_msgs::String &message) const;

    void publishCloudStatus(const std_msgs::String &message) const;

    void publishKnob(const VersionSubscribe<KnobStatus> &versionSubscribe) const;

    void publishCloudEvent(const clean_msgs::cloud_robot_event &event) const;

    void publishInternalEvent(const std_msgs::String &message) const;
};


#endif//APP_COMMUNICATION_PUBOUT_H
