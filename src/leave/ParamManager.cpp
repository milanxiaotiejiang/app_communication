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
#include "db/path.h"

const std::string app_param_path = path::data_base_config_dir() + "param_app.yaml";

const std::string zoo_param_imu_path = path::zoo_bringup_params_dir() + "base_params_with_imu.yaml";

//#include <catch2/catch.hpp>
//
//TEST_CASE() {
//    ParamManager::instance().setTof(600);
//    REQUIRE(ParamManager::instance().getTof() == 600);
//}
//
//TEST_CASE() {
//    ParamManager::instance().setSilver(false);
//    REQUIRE(ParamManager::instance().getSilver() == false);
//}
//
//TEST_CASE() {
//    ParamManager::instance().setDry(3);
//    REQUIRE(ParamManager::instance().getDry() == 3);
//}

void ParamManager::loadDefaultParam() {
    if (access(app_param_path.c_str(), F_OK)) {
        YAML::Node node;
        node["silver"] = true;
        node["dry"] = 0;
        node["energy"] = false;
        node["txt_upgrade"] = false;
        std::ofstream ofstream(app_param_path);
        ofstream << node;
        ofstream.close();
    }
}

int ParamManager::getTof() {
    if (access(zoo_param_imu_path.c_str(), F_OK) != 0) {
        throw app::exception(make_error_code(error::failed_to_parse_fall_prevention_related_files));
    }
    YAML::Node node = YAML::LoadFile(zoo_param_imu_path);
    return node["tof"].as<int>();
}

void ParamManager::setTof(int tof) {
    if (access(zoo_param_imu_path.c_str(), F_OK) != 0) {
        throw app::exception(make_error_code(error::failed_to_parse_fall_prevention_related_files));
    }
    YAML::Node node = YAML::LoadFile(zoo_param_imu_path);
    node["tof"] = tof;
    std::ofstream ofstream(zoo_param_imu_path);
    ofstream << node;
    ofstream.close();
}

bool ParamManager::getSilver() {
    if (access(app_param_path.c_str(), F_OK) != 0) {
        throw app::exception(make_error_code(error::failed_to_parse_fall_prevention_related_files));
    }
    YAML::Node node = YAML::LoadFile(app_param_path);
    auto childNode = node["silver"];
    if (childNode.IsDefined() && childNode.IsScalar()) {
        return childNode.as<bool>();
    }
    setSilver(false);
    return getSilver();
}

void ParamManager::setSilver(bool silver) {
    if (access(app_param_path.c_str(), F_OK) != 0) {
        throw app::exception(make_error_code(error::failed_to_parse_fall_prevention_related_files));
    }
    YAML::Node node = YAML::LoadFile(app_param_path);
    node["silver"] = silver;
    std::ofstream ofstream(app_param_path);
    ofstream << node;
    ofstream.close();
}

/**
 * 0：智能，默认
 * 1：日常
 * -1：关闭
 */
int ParamManager::getDry() {
    if (access(app_param_path.c_str(), F_OK) != 0) {
        loadDefaultParam();
    }
    YAML::Node node = YAML::LoadFile(app_param_path);
    auto childNode = node["dry"];
    if (childNode.IsDefined() && childNode.IsScalar()) {
        return childNode.as<int>();
    }
    setDry(0);
    return getDry();
}

void ParamManager::setDry(int dry) {
    if (access(app_param_path.c_str(), F_OK) != 0) {
        loadDefaultParam();
    }
    YAML::Node node = YAML::LoadFile(app_param_path);
    node["dry"] = dry;
    std::ofstream ofstream(app_param_path);
    ofstream << node;
    ofstream.close();
}

bool ParamManager::getEnergy() {
    if (access(app_param_path.c_str(), F_OK) != 0) {
        throw app::exception(make_error_code(error::failed_to_parse_fall_prevention_related_files));
    }
    YAML::Node node = YAML::LoadFile(app_param_path);
    auto childNode = node["energy"];
    if (childNode.IsDefined() && childNode.IsScalar()) {
        return childNode.as<bool>();
    }
    setEnergy(false);
    return getEnergy();
}

void ParamManager::setEnergy(bool energy) {
    if (access(app_param_path.c_str(), F_OK) != 0) {
        throw app::exception(make_error_code(error::failed_to_parse_fall_prevention_related_files));
    }
    YAML::Node node = YAML::LoadFile(app_param_path);
    node["energy"] = energy;
    std::ofstream ofstream(app_param_path);
    ofstream << node;
    ofstream.close();
}

bool ParamManager::getTxtUpgrade() {
    if (access(app_param_path.c_str(), F_OK) != 0) {
        throw app::exception(make_error_code(error::failed_to_parse_fall_prevention_related_files));
    }
    YAML::Node node = YAML::LoadFile(app_param_path);
    auto childNode = node["txt_upgrade"];
    if (childNode.IsDefined() && childNode.IsScalar()) {
        return childNode.as<bool>();
    }
    setEnergy(false);
    return getEnergy();
}

void ParamManager::setTxtUpgrade(bool txt_upgrade) {
    if (access(app_param_path.c_str(), F_OK) != 0) {
        throw app::exception(make_error_code(error::failed_to_parse_fall_prevention_related_files));
    }
    YAML::Node node = YAML::LoadFile(app_param_path);
    node["txt_upgrade"] = txt_upgrade;
    std::ofstream ofstream(app_param_path);
    ofstream << node;
    ofstream.close();
}
