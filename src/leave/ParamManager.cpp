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
#include "tool/Variable.h"

const std::string app_param_path = path::data_base_config_dir() + "param_app.yaml";

const std::string zoo_param_imu_path = path::zoo_bringup_params_dir() + "base_params_with_imu.yaml";

const std::string app_communication_param_path = path::app_communication_params_dir() + "robot_basic_params.yaml";

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
//
//TEST_CASE() {
//    ParamManager::instance().setBaseStation(true);
//}

void ParamManager::loadDefaultParam() {
    if (access(app_param_path.c_str(), F_OK)) {
        YAML::Node node;
        //银牛
        node["silver"] = true;
        //热风烘干
        node["dry"] = -1;
        //路径规划
        node["energy"] = false;
        //txt to sql
        node["txt_upgrade"] = false;
        //雨雪天
        node["rain_snow"] = false;
        //二次集尘
        node["collect_dust"] = false;
        //自动喷油
        node["auto_oil"] = false;
        //维护时间
        node["maintenance_start_time"] = 0;
        std::ofstream ofstream(app_param_path);
        ofstream << node;
        ofstream.close();
    }
    setSilver(true);
}

void ParamManager::reset() {
    setSilver(true);
    setDry(-1);
    setEnergy(false);
    setTxtUpgrade(false);
    setRainSnow(false);
    setCollectDust(false);
    setAutoOil(false);
    setMaintenanceStartTime(0);
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

bool ParamManager::getRainSnow() {
    if (access(app_param_path.c_str(), F_OK) != 0) {
        throw app::exception(make_error_code(error::failed_to_parse_fall_prevention_related_files));
    }
    YAML::Node node = YAML::LoadFile(app_param_path);
    auto childNode = node["rain_snow"];
    if (childNode.IsDefined() && childNode.IsScalar()) {
        return childNode.as<bool>();
    }
    setRainSnow(false);
    return getRainSnow();
}

void ParamManager::setRainSnow(bool rain_snow) {
    if (access(app_param_path.c_str(), F_OK) != 0) {
        throw app::exception(make_error_code(error::failed_to_parse_fall_prevention_related_files));
    }
    YAML::Node node = YAML::LoadFile(app_param_path);
    node["rain_snow"] = rain_snow;
    std::ofstream ofstream(app_param_path);
    ofstream << node;
    ofstream.close();
}

bool ParamManager::isBaseStation() {
    return Variable::get_instance()->getBaseExist();
}

void ParamManager::setBaseStation(bool has) {
    if (access(app_communication_param_path.c_str(), F_OK) != 0) {
        throw app::exception(make_error_code(error::failed_to_parse_fall_prevention_related_files));
    }
    YAML::Node node = YAML::LoadFile(app_communication_param_path);
    node["base_exist"] = has;
    std::ofstream ofstream(app_communication_param_path);
    ofstream << node;
    ofstream.close();
}

bool ParamManager::getCollectDust() {
    if (access(app_param_path.c_str(), F_OK) != 0) {
        throw app::exception(make_error_code(error::failed_to_parse_fall_prevention_related_files));
    }
    YAML::Node node = YAML::LoadFile(app_param_path);
    auto childNode = node["collect_dust"];
    if (childNode.IsDefined() && childNode.IsScalar()) {
        return childNode.as<bool>();
    }
    setCollectDust(false);
    return getCollectDust();
}

void ParamManager::setCollectDust(bool collect_dust) {
    if (access(app_param_path.c_str(), F_OK) != 0) {
        throw app::exception(make_error_code(error::failed_to_parse_fall_prevention_related_files));
    }
    YAML::Node node = YAML::LoadFile(app_param_path);
    node["collect_dust"] = collect_dust;
    std::ofstream ofstream(app_param_path);
    ofstream << node;
    ofstream.close();
}

bool ParamManager::getAutoOil() {
    if (access(app_param_path.c_str(), F_OK) != 0) {
        throw app::exception(make_error_code(error::failed_to_parse_fall_prevention_related_files));
    }
    YAML::Node node = YAML::LoadFile(app_param_path);
    auto childNode = node["auto_oil"];
    if (childNode.IsDefined() && childNode.IsScalar()) {
        return childNode.as<bool>();
    }
    setAutoOil(false);
    return getAutoOil();
}

void ParamManager::setAutoOil(bool auto_oil) {
    if (access(app_param_path.c_str(), F_OK) != 0) {
        throw app::exception(make_error_code(error::failed_to_parse_fall_prevention_related_files));
    }
    YAML::Node node = YAML::LoadFile(app_param_path);
    node["auto_oil"] = auto_oil;
    std::ofstream ofstream(app_param_path);
    ofstream << node;
    ofstream.close();
}

long ParamManager::getMaintenanceStartTime() {
    if (access(app_param_path.c_str(), F_OK) != 0) {
        throw app::exception(make_error_code(error::failed_to_parse_fall_prevention_related_files));
    }
    YAML::Node node = YAML::LoadFile(app_param_path);
    auto childNode = node["maintenance_start_time"];
    if (childNode.IsDefined() && childNode.IsScalar()) {
        return childNode.as<long>();
    }
    setMaintenanceStartTime(0);
    return getMaintenanceStartTime();
}

void ParamManager::setMaintenanceStartTime(long maintenance_start_time) {
    if (access(app_param_path.c_str(), F_OK) != 0) {
        throw app::exception(make_error_code(error::failed_to_parse_fall_prevention_related_files));
    }
    YAML::Node node = YAML::LoadFile(app_param_path);
    node["maintenance_start_time"] = maintenance_start_time;
    std::ofstream ofstream(app_param_path);
    ofstream << node;
    ofstream.close();
}
