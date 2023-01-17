//
// Created by lijiang on 2021/12/18.
//

#include "pub/PubInner.h"
#include <std_msgs/Int8.h>
#include "glog/logging.h"

PubInner::PubInner(ros::NodeHandle handle) : handle(handle) {
    pub_mode_ = handle.advertise<std_msgs::Int32>("/mrrobot/switch_mode", 10);
    pub_flag_ = handle.advertise<std_msgs::Int32>("/flag", 10);
    pub_charge_ = handle.advertise<std_msgs::Int32>("/mr_charge", 10);
    pub_push_mode_ = handle.advertise<std_msgs::Int16>("/mrrobot/push_mode_control", 1);
    pub_self_clean_ = handle.advertise<std_msgs::Int16>("/mrrobot/self_clean_control", 1); //基站自清洁
    pub_vacuum_mode_ = handle.advertise<std_msgs::Int16>("/mrrobot/vacuum_mode_control", 1); //扫吸
    pub_mop_mode_ = handle.advertise<std_msgs::Int16>("/mrrobot/mop_mode_control", 1); //湿拖
    pub_manual_push_ = handle.advertise<std_msgs::Int16>("/mrrobot/manual_push_control", 1); //手推建图
    pub_sweep_mode_ = handle.advertise<std_msgs::Int16>("/mrrobot/sweep_mode_control", 1);
    pub_darwer_cmd_ = handle.advertise<std_msgs::Int16>("/mrrobot/drawer_cmd", 1);
    pub_arom_status_ = handle.advertise<std_msgs::Int16>("/mrrobot/arom_status_control", 1);
    pub_light_mode_ = handle.advertise<std_msgs::Int16>("/mrrobot/light_mode", 1);
    pub_mode_command_ = handle.advertise<std_msgs::String>("/mrrobot/mode_command", 1);
    pub_task_point_ = handle.advertise<std_msgs::String>("/mrrobot/task_point", 10);
    pub_skip_point_ = handle.advertise<std_msgs::String>("/mrrobot/skip_point", 1);
    pub_velocity_ = handle.advertise<geometry_msgs::Twist>("/cmd_vel/app", 1);
    pub_pause_ = handle.advertise<geometry_msgs::Twist>("/cmd_vel/pause", 1);
    pub_passed_path_ = handle.advertise<nav_msgs::Path>("/clean_robot/passed_path", 10);
    pub_initial_postion_ = handle.advertise<geometry_msgs::PoseWithCovarianceStamped>("/initialpose", 1);
    pub_music = handle.advertise<std_msgs::Int32>("/play_music", 1);
    pub_otacore = handle.advertise<std_msgs::String>("/ota_core", 10);
    pub_otaPad = handle.advertise<std_msgs::String>("/ota_pad", 10);
    pub_otalow = handle.advertise<std_msgs::String>("/ota_low", 10);
    pub_robot_status = handle.advertise<std_msgs::Int32>("/pub_robot_status", 10);
    pub_back_to_base = handle.advertise<std_msgs::Int32>("/pub_back_to_base", 10);
    pub_start_plan_ = handle.advertise<std_msgs::Int32MultiArray>("/start_plan", 10);
    pub_ds_version = handle.advertise<std_msgs::Int16>("/dasheng/version", 1);
    pub_knob_available = handle.advertise<std_msgs::Bool>("/knob/available", 1);
    pub_knob_task = handle.advertise<std_msgs::Int8>("/knob/task", 1);
    pub_reboot = handle.advertise<std_msgs::Int32>("/reboot_flag", 1);
    pub_shutdown = handle.advertise<std_msgs::Int32>("/shutdown_flag", 1);
}

void PubInner::publishMode(const std_msgs::Int32 &message) const {
    //mode:0:建图；1修改地图；2工作
    pub_mode_.publish(message);
}

void PubInner::publishCharge(const std_msgs::Int32 &message) const {
    //mode:0:建图；1修改地图；2工作
    pub_charge_.publish(message);
}

void PubInner::publishFlag(const std_msgs::Int32 &message) const {

    pub_flag_.publish(message);
}

void PubInner::pubManualPush(const std_msgs::Int16 &message) const {
    pub_manual_push_.publish(message);
}

void PubInner::publishStartPlan(const std_msgs::Int32MultiArray &message) const {
    pub_start_plan_.publish(message);
}

void PubInner::publishPushMode(const std_msgs::Int16 &message) const {
    pub_push_mode_.publish(message);
}

void PubInner::publishSelfClean(const std_msgs::Int16 &message) const {
    pub_self_clean_.publish(message);
}

void PubInner::publishVacuumMode(const std_msgs::Int16 &message) const {
    pub_vacuum_mode_.publish(message);
}

void PubInner::publishMopMode(const std_msgs::Int16 &message) const {
    pub_mop_mode_.publish(message);
}

void PubInner::publishSweepMode(const std_msgs::Int16 &message) const {
    pub_sweep_mode_.publish(message);
}

void PubInner::publishAromStatus(const std_msgs::Int16 &message) const {
    pub_arom_status_.publish(message);
}

void PubInner::publishDrawerCmd(const std_msgs::Int16 &message) const {
    pub_darwer_cmd_.publish(message);
}

void PubInner::publishLightCmd(const std_msgs::Int16 &message) const {
    pub_light_mode_.publish(message);
}

void PubInner::publishCommand(const std_msgs::String &message) const {
    pub_mode_command_.publish(message);
}

void PubInner::publishVelocity(const geometry_msgs::Twist &message) const {
    pub_velocity_.publish(message);
}

void PubInner::publishTaskPoint(const std_msgs::String &message) const {
    pub_task_point_.publish(message);
}

void PubInner::publishSkipTaskPoint(const std_msgs::String &message) const {
    pub_skip_point_.publish(message);
}


void PubInner::publishPauseVel(const geometry_msgs::Twist &message) const {
    pub_pause_.publish(message);
}

void PubInner::publishPassedPath(const nav_msgs::Path &message) const {
    pub_passed_path_.publish(message);
}

void PubInner::publishInitialposition(const geometry_msgs::PoseWithCovarianceStamped &message) const {
    pub_initial_postion_.publish(message);
}

void PubInner::publishMusic(const std_msgs::Int32 &message) const {
    pub_music.publish(message);
}

void PubInner::publishOtaPad(const std_msgs::String &message) const {
    pub_otaPad.publish(message);
}

void PubInner::publishOtaCore(const std_msgs::String &message) const {
    pub_otacore.publish(message);
}

void PubInner::publishOtaLow(const std_msgs::String &message) const {
    LOG(ERROR) << message.data << "  ";
    pub_otalow.publish(message);
}

void PubInner::publishRobotStatus(const std_msgs::Int32 &message) const {
    pub_robot_status.publish(message);
}

void PubInner::publishBacktoBase(const std_msgs::Int32 &message) const {
    pub_back_to_base.publish(message);
}

void PubInner::publishDSVersion(const std_msgs::Int16 &message) const {
    pub_ds_version.publish(message);
}

void PubInner::publishKnobAvailable(const std_msgs::Bool &message) const {
    pub_knob_available.publish(message);
}

void PubInner::publishKnobTask(const std_msgs::Int8 &message) const {
    pub_knob_task.publish(message);
}

void PubInner::publishShutDown(const std_msgs::Int32 &message) const {
    pub_shutdown.publish(message);
}

void PubInner::publishReboot(const std_msgs::Int32 &message) const {
    pub_reboot.publish(message);
}
