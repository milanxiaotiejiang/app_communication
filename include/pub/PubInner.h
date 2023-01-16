//
// Created by lijiang on 2021/12/18.
//

#ifndef APP_COMMUNICATION_PUBINNER_H
#define APP_COMMUNICATION_PUBINNER_H

#include "geometry_msgs/PoseWithCovarianceStamped.h"
#include "nav_msgs/Odometry.h"
#include "nav_msgs/Path.h"
#include <geometry_msgs/Pose.h>
#include <geometry_msgs/Twist.h>
#include <nav_msgs/OccupancyGrid.h>
#include <ros/ros.h>
#include <std_msgs/Int16.h>
#include <std_msgs/Int32.h>
#include <std_msgs/Int8.h>
#include <std_msgs/String.h>
#include <std_msgs/Int32MultiArray.h>
#include "std_msgs/Bool.h"

class PubInner {

private:
    ros::NodeHandle handle;
    ros::Publisher pub_mode_,//发布工作模式
    pub_flag_,//基站控制flag
    pub_charge_,
    pub_manual_push_,//手推建图
    pub_push_mode_, //尘推
    pub_self_clean_, //基站自清洁
    pub_vacuum_mode_, //吸
    pub_sweep_mode_, //sao
    pub_mop_mode_,//tuo
    pub_darwer_cmd_, //开抽屉
    pub_arom_status_, //香薰机
    pub_light_mode_,  //
    pub_mode_command_,   //内部指令
    pub_velocity_,       //速度
    pub_task_point_,     //内部指令
    pub_skip_point_,     //内部指令
    pub_pause_,          //暂停速度指令
    pub_passed_path_,    //走过的路径
    pub_initial_postion_,//估计位姿
    pub_music,           //播放音乐
    pub_otacore,         //core ota
    pub_otalow,           //下位机ota
    pub_otaPad,           //pad ota
    pub_robot_status,  //
    pub_start_plan_,      //开始全局规划
    pub_back_to_base,   //回充
    pub_ds_version,     //下位机版本
    pub_knob_available,  //
    pub_knob_task,
    pub_shutdown,
    pub_reboot;


public:
    PubInner(ros::NodeHandle handle);

    void publishMode(const std_msgs::Int32 &message) const;
    void publishCharge(const std_msgs::Int32 &message) const;

    void publishFlag(const std_msgs::Int32 &message) const;

    void pubManualPush(const std_msgs::Int16 &message) const;

    void publishStartPlan(const std_msgs::Int32MultiArray &message) const;

    void publishPushMode(const std_msgs::Int16 &message) const;

    void publishSelfClean(const std_msgs::Int16 &message) const;

    void publishVacuumMode(const std_msgs::Int16 &message) const;

    void publishMopMode(const std_msgs::Int16 &message) const;

    void publishSweepMode(const std_msgs::Int16 &message) const;

    void publishDrawerCmd(const std_msgs::Int16 &message) const;

    void publishLightCmd(const std_msgs::Int16 &message) const;

    void publishAromStatus(const std_msgs::Int16 &message) const;

    void publishCommand(const std_msgs::String &message) const;

    void publishVelocity(const geometry_msgs::Twist &message) const;

    void publishTaskPoint(const std_msgs::String &message) const;

    void publishSkipTaskPoint(const std_msgs::String &message) const;

    void publishPauseVel(const geometry_msgs::Twist &message) const;

    void publishPassedPath(const nav_msgs::Path &message) const;

    void publishInitialposition(const geometry_msgs::PoseWithCovarianceStamped &message) const;

    void publishMusic(const std_msgs::Int32 &message) const;
    void publishOtaCore(const std_msgs::String &message) const;
    void publishOtaLow(const std_msgs::String &message) const;
    void publishOtaPad(const std_msgs::String &message) const;
    void publishRobotStatus(const std_msgs::Int32 &message) const;

    void publishBacktoBase(const std_msgs::Int32 &message) const;

    void publishDSVersion(const std_msgs::Int16 &message) const;

    void publishKnobAvailable(const std_msgs::Bool &message) const;

    void publishKnobTask(const std_msgs::Int8 &message) const;

    void publishShutDown(const std_msgs::Int32 &message) const;

    void publishReboot(const std_msgs::Int32 &message) const;
};


#endif//APP_COMMUNICATION_PUBINNER_H
