//
// Created by mi on 2022/9/10.
//

#ifndef APP_COMMUNICATION_PUBLISHINNERMANAGER_H
#define APP_COMMUNICATION_PUBLISHINNERMANAGER_H

#include <ros/ros.h>
#include <std_msgs/Int32.h>
#include <std_msgs/Int16.h>
#include <std_msgs/Int32MultiArray.h>
#include <geometry_msgs/Twist.h>
#include <std_msgs/String.h>
#include <nav_msgs/Path.h>
#include <geometry_msgs/PoseWithCovarianceStamped.h>
#include <std_msgs/Bool.h>
#include <std_msgs/Int8.h>

class PublishInnerManager {
private:
    ros::Publisher//
    pub_push_mode_, //尘推
    pub_self_clean_, //基站自清洁
    pub_vacuum_mode_, //吸
    pub_sweep_mode_, //sao
    pub_mop_mode_,//tuo
    pub_darwer_cmd_, //开抽屉
    pub_arom_status_, //香薰机
    pub_light_mode_,  //
    pub_velocity_,       //速度
    pub_passed_path_,    //走过的路径
    pub_music,           //播放音乐
    pub_otalow,           //下位机ota
    pub_ds_version,     //下位机版本
    pub_knob_available,  //
    pub_knob_task,//
    pub_shutdown,//
    pub_reboot,//
    pub_oil,//
    pub_knife,//
    acceptAppSchedule;
public:
    static auto &instance() {
        static PublishInnerManager obj;
        return obj;
    }

    void initialize(ros::NodeHandle handle);

    void publishPushMode(const std_msgs::Int16 &message) const;

    void publishSelfClean(const std_msgs::Int16 &message) const;

    void publishVacuumMode(const std_msgs::Int16 &message) const;

    void publishMopMode(const std_msgs::Int16 &message) const;

    void publishSweepMode(const std_msgs::Int16 &message) const;

    void publishDrawerCmd(const std_msgs::Int16 &message) const;

    void publishLightCmd(const std_msgs::Int16 &message) const;

    void publishAromStatus(const std_msgs::Int16 &message) const;

    void publishVelocity(const geometry_msgs::Twist &message) const;

    void publishPassedPath(const nav_msgs::Path &message) const;

    void publishMusic(const std_msgs::Int32 &message) const;

    void publishOtaLow(const std_msgs::String &message) const;

    void publishDSVersion(const std_msgs::Int16 &message) const;

    void publishKnobAvailable(const std_msgs::Bool &message) const;

    void publishKnobTask(const std_msgs::Int8 &message) const;

    void publishShutDown(const std_msgs::Int32 &message) const;

    void publishReboot(const std_msgs::Int32 &message) const;

    void publishOil(const std_msgs::Int32 &message) const;

    void pubKnife(const std_msgs::Int32 &message) const;

    void publishAppSchedule(const std_msgs::String &message) const;
};

#endif //APP_COMMUNICATION_PUBLISHINNERMANAGER_H
