//
// Created by lijiang on 2022/6/14.
//

#include <ros/package.h>
#include "sub/DSVersionSubscribe.h"
#include "tool/write_file.hpp"
#include "manager/PublishInnerManager.h"
#include "db/path.h"

DSVersionSubscribe::DSVersionSubscribe(ros::NodeHandle handle) : handle(handle) {
    sub_ds_hw = handle.subscribe("/dasheng/hw", 1, &DSVersionSubscribe::subscribeHWCallback, this);
    sub_ds_sw = handle.subscribe("/dasheng/sw", 1, &DSVersionSubscribe::subscribeSWCallback, this);

    std_msgs::Int32 version;
    version.data = 0;
    PublishInnerManager::instance().publishDSVersion(version);
    sleep(0.5);
    version.data = 1;
    PublishInnerManager::instance().publishDSVersion(version);

    string filePath;
    filePath.append(path::data_base_config_dir());
    filePath.append("pad_version_info.txt");

    if (sh::File::exists(filePath)) {
        unique_ptr<sh::File> uFilePtr(new sh::File(filePath));
        if (uFilePtr->open(std::ios::in)) {
            auto padVersion = uFilePtr->readAll();
            VersionManager::instance().setAppPadVersion(padVersion);
        }
        uFilePtr->close();
    }
}

void DSVersionSubscribe::subscribeHWCallback(const std_msgs::String &msg) {
    VersionManager::instance().setDsHardVersion(msg.data);
}

void DSVersionSubscribe::subscribeSWCallback(const std_msgs::String &msg) {
    VersionManager::instance().setDsSoftVersion(msg.data);
}
