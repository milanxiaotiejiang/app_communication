//
// Created by Looper on 2023/2/22.
//

#include "leave/ParamManager.h"
#include "ros/ros.h"
#include "ros/package.h"
#include "string"
#include "yaml-cpp/yaml.h"
#include <iostream>
#include <fstream>
#include "BaseThrowable.h"

const std::string data_base_dir = ros::package::getPath("data_base");
const std::string drop_path = data_base_dir + "/config/" + "param_app.yaml";

void ParamManager::loadDefaultParam() {
    if (access(drop_path.c_str(), F_OK)) {
        YAML::Node node;
        node["tof"] = 600;
        node["silver"] = true;
        node["dry"] = 0;
        std::ofstream ofstream(drop_path);
        ofstream << node;
        ofstream.close();
    }
}

int ParamManager::getTof() {
    if (access(drop_path.c_str(), F_OK) != 0) {
        throw app::exception(make_error_code(error::failed_to_parse_fall_prevention_related_files));
    }
    YAML::Node node = YAML::LoadFile(drop_path);
    return node["tof"].as<int>();
}

void ParamManager::setTof(int tof) {
    if (access(drop_path.c_str(), F_OK) != 0) {
        throw app::exception(make_error_code(error::failed_to_parse_fall_prevention_related_files));
    }
    YAML::Node node = YAML::LoadFile(drop_path);
    node["tof"] = tof;
    std::ofstream ofstream(drop_path);
    ofstream << node;
    ofstream.close();
}

bool ParamManager::getSilver() {
    if (access(drop_path.c_str(), F_OK) != 0) {
        throw app::exception(make_error_code(error::failed_to_parse_fall_prevention_related_files));
    }
    YAML::Node node = YAML::LoadFile(drop_path);
    return node["tof"].as<int>();
}

void ParamManager::setSilver(bool silver) {
    if (access(drop_path.c_str(), F_OK) != 0) {
        throw app::exception(make_error_code(error::failed_to_parse_fall_prevention_related_files));
    }
    YAML::Node node = YAML::LoadFile(drop_path);
    node["silver"] = silver;
    std::ofstream ofstream(drop_path);
    ofstream << node;
    ofstream.close();
}

/**
 * 0：智能，默认
 * 1：日常
 * -1：关闭
 */
int ParamManager::getDry() {
    if (access(drop_path.c_str(), F_OK) != 0) {
        loadDefaultParam();
    }
    YAML::Node node = YAML::LoadFile(drop_path);
    return node["dry"].as<int>();
}

void ParamManager::setDry(int dry) {
    if (access(drop_path.c_str(), F_OK) != 0) {
        loadDefaultParam();
    }
    YAML::Node node = YAML::LoadFile(drop_path);
    node["dry"] = dry;
    std::ofstream ofstream(drop_path);
    ofstream << node;
    ofstream.close();
}
