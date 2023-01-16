//
// Created by Paul Wang on 2022/09/26.
//

#include "model/OtaInfo.h"

OtaInfo::OtaInfo(){}

OtaInfo::OtaInfo(const std::string &otamodule, const std::string &otaversion, const std::string &otapath,const std::string &otadesc)
:ota_module(otamodule),
ota_version(otaversion),
ota_path(otapath),
ota_desc(otadesc){}

OtaInfo::~OtaInfo(){}

    const std::string &OtaInfo::getota_module() const{
        return ota_module;
    }

    void OtaInfo::setota_module(const std::string &module){
        ota_module = module;
    }

    const std::string &OtaInfo::getota_version() const{
        return ota_version;
    }

    void OtaInfo::setota_version(const std::string &ov){
        ota_version = ov;
    }

    const std::string &OtaInfo::getota_path() const{
        return ota_path;
    }

    void OtaInfo::setota_path(const std::string &op){
        ota_path = op;
    }

    const std::string &OtaInfo::getota_desc() const{
        return ota_desc;
    }

    void OtaInfo::setota_desc(const std::string &od){
        ota_desc = od;
    }