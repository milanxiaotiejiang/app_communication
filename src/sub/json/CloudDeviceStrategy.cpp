//
// Created by mi on 2022/8/12.
//

#include "sub/json/CloudDeviceStrategy.h"

string GetDeviceSecretStrategy::handler(string method) {
    string path;
    path.append("/opt/robot/robot_hw_info.yaml");
    if (sh::File::exists(path)) {
        YAML::Node config = YAML::LoadFile(path);
        YAML::Node deviceSecretNode = config["device_secret"];
        YAML::Node deviceNameNode = config["device_name"];
        if (!deviceNameNode.IsNull()) {
            std::string deviceSecret = deviceNameNode.as<string>();
            return deviceSecret;
        } else {
            throw app::exception(make_error_code(error::robot_hw_info_parsing_failed));
        }
    } else {
        throw app::exception(make_error_code(error::robot_hw_info_not_exist));
    }
}
