//
// Created by lijiang on 2022/5/25.
//

#ifndef APP_COMMUNICATION_VERSIONINFO_H
#define APP_COMMUNICATION_VERSIONINFO_H

#include <utility>

#include "string"

#include "nlohmann/json.hpp"

using namespace std;
using json = nlohmann::json;

class VersionInfo {
private:
    string air_code_version;
    string ds_hard_version;
    string ds_soft_version;
    string app_pad_version;
public:

    VersionInfo(string airCodeVersion, string dsHardVersion, string dsSoftVersion,
                string appPadVersion) : air_code_version(std::move(airCodeVersion)),
                                        ds_hard_version(std::move(dsHardVersion)),
                                        ds_soft_version(std::move(dsSoftVersion)),
                                        app_pad_version(std::move(appPadVersion)) {}

    friend void to_json(json &j, const VersionInfo &info) {
        j = json{
                {"air_code_version", info.air_code_version},
                {"ds_hard_version",  info.ds_hard_version},
                {"ds_soft_version",  info.ds_soft_version},
                {"app_pad_version",  info.app_pad_version}
        };
    }

    friend void from_json(const json &j, VersionInfo &info) {
        j.at("air_code_version").get_to(info.air_code_version);
        j.at("ds_hard_version").get_to(info.ds_hard_version);
        j.at("ds_soft_version").get_to(info.ds_soft_version);
        j.at("app_pad_version").get_to(info.app_pad_version);
    }
};

#endif //APP_COMMUNICATION_VERSIONINFO_H
