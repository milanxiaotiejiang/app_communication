//
// Created by Looper on 2023/2/24.
//

#include "manager/PublishInnerManager.h"

void PublishInnerManager::initialize(ros::NodeHandle handle) {
    pub_push_mode_ = handle.advertise<std_msgs::Int16>("/mrrobot/push_mode_control", 1);
    pub_self_clean_ = handle.advertise<std_msgs::Int16>("/mrrobot/self_clean_control", 1); //基站自清洁
    pub_vacuum_mode_ = handle.advertise<std_msgs::Int16>("/mrrobot/vacuum_mode_control", 1); //扫吸
    pub_mop_mode_ = handle.advertise<std_msgs::Int16>("/mrrobot/mop_mode_control", 1); //湿拖
    pub_sweep_mode_ = handle.advertise<std_msgs::Int16>("/mrrobot/sweep_mode_control", 1);
    pub_darwer_cmd_ = handle.advertise<std_msgs::Int16>("/mrrobot/drawer_cmd", 1);
    pub_arom_status_ = handle.advertise<std_msgs::Int16>("/mrrobot/arom_status_control", 1);
    pub_light_mode_ = handle.advertise<std_msgs::Int16>("/mrrobot/light_mode", 1);
    pub_mode_command_ = handle.advertise<std_msgs::String>("/mrrobot/mode_command", 1);
    pub_velocity_ = handle.advertise<geometry_msgs::Twist>("/cmd_vel/app", 1);
    pub_passed_path_ = handle.advertise<nav_msgs::Path>("/clean_robot/passed_path", 10);
    pub_music = handle.advertise<std_msgs::Int32>("/play_music", 1);
    pub_otalow = handle.advertise<std_msgs::String>("/ota_low", 10);
    pub_ds_version = handle.advertise<std_msgs::Int16>("/dasheng/version", 1);
    pub_knob_available = handle.advertise<std_msgs::Bool>("/knob/available", 1);
    pub_knob_task = handle.advertise<std_msgs::Int8>("/knob/task", 1);
    pub_reboot = handle.advertise<std_msgs::Int32>("/reboot_flag", 1);
    pub_shutdown = handle.advertise<std_msgs::Int32>("/shutdown_flag", 1);
    pub_oil = handle.advertise<std_msgs::Int32>("/mrrobot/pop_sweep_oil", 1);
    pub_knife = handle.advertise<std_msgs::Int32>("/mrrobot/control_wind_knife", 1);
}

void PublishInnerManager::publishPushMode(const std_msgs::Int16 &message) const {
    pub_push_mode_.publish(message);
}

void PublishInnerManager::publishSelfClean(const std_msgs::Int16 &message) const {
    pub_self_clean_.publish(message);
}

void PublishInnerManager::publishVacuumMode(const std_msgs::Int16 &message) const {
    pub_vacuum_mode_.publish(message);
}

void PublishInnerManager::publishMopMode(const std_msgs::Int16 &message) const {
    pub_mop_mode_.publish(message);
}

void PublishInnerManager::publishSweepMode(const std_msgs::Int16 &message) const {
    pub_sweep_mode_.publish(message);
}

void PublishInnerManager::publishAromStatus(const std_msgs::Int16 &message) const {
    pub_arom_status_.publish(message);
}

void PublishInnerManager::publishDrawerCmd(const std_msgs::Int16 &message) const {
    pub_darwer_cmd_.publish(message);
}

void PublishInnerManager::publishLightCmd(const std_msgs::Int16 &message) const {
    pub_light_mode_.publish(message);
}

void PublishInnerManager::publishCommand(const std_msgs::String &message) const {
    pub_mode_command_.publish(message);
}

void PublishInnerManager::publishVelocity(const geometry_msgs::Twist &message) const {
    pub_velocity_.publish(message);
}

void PublishInnerManager::publishPassedPath(const nav_msgs::Path &message) const {
    pub_passed_path_.publish(message);
}

void PublishInnerManager::publishMusic(const std_msgs::Int32 &message) const {
    pub_music.publish(message);
}

void PublishInnerManager::publishOtaLow(const std_msgs::String &message) const {
    pub_otalow.publish(message);
}

void PublishInnerManager::publishDSVersion(const std_msgs::Int16 &message) const {
    pub_ds_version.publish(message);
}

void PublishInnerManager::publishKnobAvailable(const std_msgs::Bool &message) const {
    pub_knob_available.publish(message);
}

void PublishInnerManager::publishKnobTask(const std_msgs::Int8 &message) const {
    pub_knob_task.publish(message);
}

void PublishInnerManager::publishShutDown(const std_msgs::Int32 &message) const {
    pub_shutdown.publish(message);
}

void PublishInnerManager::publishReboot(const std_msgs::Int32 &message) const {
    pub_reboot.publish(message);
}

void PublishInnerManager::publishOil(const std_msgs::Int32 &message) const {
    pub_oil.publish(message);
}

void PublishInnerManager::pubKnife(const std_msgs::Int32 &message) const {
    pub_knife.publish(message);
}
