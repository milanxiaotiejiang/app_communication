//
// Created by lijiang on 2022/5/25.
//

#include "sub/json/LocationStrategy.h"
#include "db/path.h"

string LocationStrategy::handler(Location params) {
    string filePath;
    filePath.append(path::data_base_config_path());
    filePath.append("location_info.txt");

    if (!sh::File::exists(filePath)) {
        unique_ptr<sh::File> uFilePtr(new sh::File(filePath));
        if (!uFilePtr->create(filePath)) {
            throw app::exception(make_error_code(error::acquisition_file_failed));
        }
    }

    unique_ptr<sh::File> uFilePtr(new sh::File(filePath));
    if (!uFilePtr->open(std::ios::in)) {
        throw app::exception(make_error_code(error::open_file_fail));
    }
    uFilePtr->close();

    json jsonLocation = params;
    string strLocation = jsonLocation.dump();
    // pubOut.robot_result = strLocation;
    // LOG(INFO) << "location info  to cloud----------- " << pubOut.robot_result;

    //直接保存
    if (sh::File::saveTextTo(filePath, strLocation)) {
        return "";
    } else {
        throw app::exception(make_error_code(error::ave_location_info_file_fail));
    }
}

void getLocationStrategy::handler() {
    bool is_location;

    // string filePath;
    // filePath.append(path::data_base_config_path());
    // filePath.append("project_info.txt");

    // // sh::File *pFile1 = new sh::File(filePath);
    // std::shared_ptr<sh::File> pFile1 = make_shared<sh::File>(filePath);


    // if (pFile1->open(std::ios::in)) {//检查是否存在文件
    //     responseP = pFile1->readAll();
    //     if (responseP.length() > 0) {
    //         is_location = true;
    //         jsonProject = json::parse(responseP);
    //     } else {
    //         is_location = false;
    //         PublishOutManager::instance().getPubOut()->robot_result = "{\"err_msg\":\"this no project\"}";
    //     }
    // } else {
    // }
    // json jsonProject;
    string responseP;
    string filePath2;
    filePath2.append(path::data_base_config_path());
    filePath2.append("location_info.txt");

    // *pFile2 = new sh::File(filePath2);
    std::shared_ptr<sh::File> pFile2 = make_shared<sh::File>(filePath2);
    string strLocationInfo;
    json jLocation;
    if (pFile2->open(std::ios::in)) {//检查是否存在矩形文件
        strLocationInfo = pFile2->readAll();
        if (strLocationInfo.length() > 0) {
            is_location = true;
            jLocation = json::parse(strLocationInfo);
            // jLocation["project"] = jsonProject["name"];
            strLocationInfo = jLocation.dump();
            CloudRobotControl::instance().saveInfo(strLocationInfo);
        } else {
            is_location = false;
            CloudRobotControl::instance().saveInfo("{\"err_msg\":\"this no locaiton\"}");
        }
    } else {
    }
    // return strLocationInfo;
}
