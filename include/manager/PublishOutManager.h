//
// Created by mi on 2022/9/10.
//

#ifndef APP_COMMUNICATION_PUBLISHOUTMANAGER_H
#define APP_COMMUNICATION_PUBLISHOUTMANAGER_H

#include <std_msgs/String.h>
#include <nav_msgs/OccupancyGrid.h>
#include <nav_msgs/Path.h>
#include <sensor_msgs/LaserScan.h>
#include <visualization_msgs/Marker.h>
#include "ros/ros.h"
#include "model/ShowWorkStatus.h"
#include "model/Notice.h"
#include "net/base/VersionSubscribe.h"
#include "model/InternalEvent.h"
#include "std_msgs/Int32.h"
#include "net/ros/SensorSelfModel.h"
#include "alignment/rrmap2.h"
#include "model/ElevatorStatus.h"
#include "elevator_control.h"

class PublishOutManager {
private:
    PublishOutManager() = default;

    PublishOutManager(PublishOutManager &) = delete;

    PublishOutManager &operator=(const PublishOutManager &) = delete;

public:
    ~PublishOutManager() = default;

private:
    ros::Publisher pub_response_,//原协议
    pub_response_json_,  //新协议
    pub_robot_status_,   //机器人状态
    pub_map_,            //地图
    pub_notice_,         //notice
    pub_sensor_check_,  //
    pub_internal_event_;//发送给云端
    ros::Publisher acceptAppJsonV1;
    ros::Publisher acceptAppCommunication;
    ros::Publisher pubCarpet;
    ros::Publisher pubPad;
    ros::Publisher pubCloud;
    ros::Publisher pubElevatorManager;
    ros::Publisher pubElevatorStatus;
    ros::Publisher pubElevatorVoice;
    ros::Publisher pubTaskStatus;
public:
    static auto &instance() {
        static PublishOutManager obj;
        return obj;
    }

    void initialize(ros::NodeHandle handle);

    void publishJson(const std::string &message) const;

    void publishStatus(const VersionSubscribe<ShowWorkStatus> &versionSubscribe) const;

    void publishMap(const nav_msgs::OccupancyGrid &message) const;

    void publishNotice(const Notice &notice) const;

    void publishSensorCheck(const SensorSelf &model) const;

    void publishAppJson(int version, const std_msgs::String &message) const;

    void publishAppCommunication(const std_msgs::String &message) const;

    void publishAlarm(const internal_event::AlarmEvent &alarmEvent) const;

    void publishInternalEvent(const std_msgs::String &message) const;

    void publishResourcesUpdateForPad(const std_msgs::Int32 &message) const;

    void publishResourcesUpdateForCloud(const std_msgs::Int32 &message) const;

    void publishElevatorManager() const;

    void publishElevatorStatus(ElevatorModel elevatorModel) const;

    void publishTaskStatus(TaskVo task) const;

    void publishElevatorVoice(Voice voice) const;
};


#endif //APP_COMMUNICATION_PUBLISHOUTMANAGER_H
