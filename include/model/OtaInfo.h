//
// Created by Paul Wang on 2022/09/26.
//

#ifndef APP_COMMUNICATION_OTAINFO_H
#define APP_COMMUNICATION_OTAINFO_H

#include <string>

#include "nlohmann/json.hpp"
#include <iostream>

using json = nlohmann::json;



class OtaInfo {
private:
    std::string ota_module; //取值：“core", "pad", "ecu"，"nebula"
    std::string ota_version; //
    std::string ota_path; //
    std::string ota_desc; //
    

public:
    OtaInfo();

    virtual ~OtaInfo();

    
    OtaInfo(const std::string &ota_module, const std::string &ota_version, const std::string &ota_path,const std::string &ota_desc);

    const std::string &getota_module() const;

    void setota_module(const std::string &module);

    const std::string &getota_version() const;

    void setota_version(const std::string &ov);

    const std::string &getota_path() const;

    void setota_path(const std::string &op);

    const std::string &getota_desc() const;

    void setota_desc(const std::string &od);

    

    friend void to_json(json &j, const OtaInfo &b) {
        j = json{
                {"ota_module",   b.ota_module},
                {"ota_version", b.ota_version},
                {"ota_path",    b.ota_path},
                {"ota_desc", b.ota_desc}

        };
    }

    friend void from_json(const json &j, OtaInfo &b) {

        j.at("ota_module").get_to(b.ota_module);
        j.at("ota_version").get_to(b.ota_version);
        j.at("ota_path").get_to(b.ota_path);
        j.at("ota_desc").get_to(b.ota_desc);
       
    }
};


#endif //APP_COMMUNICATION_OTAINFO_H
