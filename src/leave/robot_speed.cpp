//
// Created by Looper on 2023/2/22.
//

#include "leave/robot_speed.h"
#include "ros/ros.h"
#include "ros/package.h"
#include "simulation.h"
#include "yaml-cpp/yaml.h"
#include "BaseThrowable.h"
#include "db/path.h"
#include <iostream>
#include <fstream>

const std::string DWA_LOCAL_PLANNER_PARAMS_ZOO_FILE_NAME = "dwa_local_planner_params_zoo.yaml";

float RobotSpeed::currentSpeed() {
    auto slamPath = path::robot_slam_dir();
    auto absolute_path = slamPath + "/params/planner/" + DWA_LOCAL_PLANNER_PARAMS_ZOO_FILE_NAME;

    if (access(absolute_path.c_str(), F_OK) != 0) {
        throw app::exception(make_error_code(error::failed_to_parse_speed_related_files));
    }
    YAML::Node rootConfig = YAML::LoadFile(absolute_path);
    if (!rootConfig.IsDefined() || !rootConfig.IsMap()) {
        throw app::exception(make_error_code(error::failed_to_parse_speed_related_files));
    }

    auto DWAPlannerROSNode = rootConfig["DWAPlannerROS"];
    if (!DWAPlannerROSNode.IsDefined() || !DWAPlannerROSNode.IsMap()) {
        throw app::exception(make_error_code(error::failed_to_parse_speed_related_files));
    }
    return DWAPlannerROSNode["max_vel_x"].as<float>();
}

void RobotSpeed::changeSpeed(float speed) {
    auto slamPath = ros::package::getPath("robot_slam");
    auto absolute_path = slamPath + "/params/planner/" + DWA_LOCAL_PLANNER_PARAMS_ZOO_FILE_NAME;

    if (access(absolute_path.c_str(), F_OK) != 0) {
        throw app::exception(make_error_code(error::failed_to_parse_speed_related_files));
    }

    YAML::Node rootConfig = YAML::LoadFile(absolute_path);
    if (!rootConfig.IsDefined() || !rootConfig.IsMap()) {
        throw app::exception(make_error_code(error::failed_to_parse_speed_related_files));
    }

    auto DWAPlannerROSNode = rootConfig["DWAPlannerROS"];
    if (!DWAPlannerROSNode.IsDefined() || !DWAPlannerROSNode.IsMap()) {
        throw app::exception(make_error_code(error::failed_to_parse_speed_related_files));
    }

    DWAPlannerROSNode["max_vel_x"] = speed;

    std::ofstream ofstream(absolute_path);

    ofstream << rootConfig;
    ofstream.close();
}