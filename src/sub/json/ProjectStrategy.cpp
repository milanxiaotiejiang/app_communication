//
// Created by lijiang on 2022/5/26.
//

#include "sub/json/ProjectStrategy.h"
#include "manager/VersionManager.h"
#include "leave/ParamManager.h"
#include "leave/robot_speed.h"
#include "manager/cloud_robot_control.h"
#include "db/path.h"

string ProjectStrategy::handler(Project params) {
    string filePath;
    filePath.append(path::data_base_config_dir());
    filePath.append("project_info.txt");

    if (!sh::File::exists(filePath)) {
        unique_ptr<sh::File> uFilePtr(new sh::File(filePath));
        if (!uFilePtr->create(filePath)) {
            throw app::exception(make_error_code(error::create_file_fail));
        }
    }

    unique_ptr<sh::File> uFilePtr(new sh::File(filePath));
    if (!uFilePtr->open(std::ios::in)) {
        throw app::exception(make_error_code(error::open_file_fail));
    }
    uFilePtr->close();

    string name = params.getName();

    //直接保存
    if (sh::File::saveTextTo(filePath, name)) {
        return "";
    } else {
        throw app::exception(make_error_code(error::open_file_fail));
    }
}

string getProjectStrategy::handler(string params) {
    bool is_location;

    string filePath;
    filePath.append(path::data_base_config_dir());
    filePath.append("project_info.txt");

    sh::File *pFile1 = new sh::File(filePath);
    string responseP;
    if (pFile1->open(std::ios::in)) {//检查是否存在文件
        responseP = pFile1->readAll();
        if (responseP.length() > 0) {
            is_location = true;
            CloudRobotControl::instance().saveInfo(responseP);
        } else {
            is_location = false;
            CloudRobotControl::instance().saveInfo("there is no project!!!");
        }
    } else {
    }
    LOG(INFO) << "!!!!!!!!!!!!!!!!!!!!file " << responseP;
    // sh::File::readAll(filePath)
    return "";
}

string PadVersionStrategy::handler(string params) {
    string filePath;
    filePath.append(path::data_base_config_dir());
    filePath.append("pad_version_info.txt");

    if (!sh::File::exists(filePath)) {
        unique_ptr<sh::File> uFilePtr(new sh::File(filePath));
        if (!uFilePtr->create(filePath)) {
            throw app::exception(make_error_code(error::create_file_fail));
        }
    }

    unique_ptr<sh::File> uFilePtr(new sh::File(filePath));
    if (!uFilePtr->open(std::ios::in)) {
        LOG(ERROR) << "open file " << filePath << " fail !!!";
        throw app::exception(make_error_code(error::open_file_fail));
    }
    uFilePtr->close();

    VersionManager::instance().setAppPadVersion(params);

    //直接保存
    if (sh::File::saveTextTo(filePath, params)) {
        return "";
    } else {
        throw app::exception(make_error_code(error::failed_to_open_version_file));
    }
}

int MachineModelStrategy::handler(string params) {
    int machineVersion = VersionManager::instance().getMachineVersion();
    return machineVersion;
}

ParamVo GetRobotParamsStrategy::handler(string params) {
    int tof = ParamManager::instance().getTof();
    bool silver = ParamManager::instance().getSilver();
    RobotSpeed robotSpeed;
    float speed = robotSpeed.currentSpeed();
    return ParamVo(tof, silver, speed);
}

ParamVo SetRobotParamsStrategy::handler(ParamVo params) {
    ParamManager::instance().setTof(params.getTof());
    ParamManager::instance().setSilver(params.isSilver());
    RobotSpeed robotSpeed;
    robotSpeed.changeSpeed(params.getSpeed());
    return params;
}
