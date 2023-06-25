//
// Created by lijiang on 2022/5/25.
// 单例新写法
//

#ifndef APP_COMMUNICATION_VERSIONMANAGER_H
#define APP_COMMUNICATION_VERSIONMANAGER_H

#include "string"

class VersionManager {
private:
    std::string air_code_version{"0.0.0.1"};
    std::string ds_hard_version{"0.0.0.1"};
    std::string ds_soft_version{"0.0.0.1"};
    std::string app_pad_version{"0.0.0.1"};
    int machine_version{630};
    int self_check_version{1};
public:
    static auto &instance() {
        static VersionManager obj;
        return obj;
    }

    const std::string &getAirCodeVersion() const {
        return air_code_version;
    }

    void setAirCodeVersion(const std::string &airCodeVersion) {
        air_code_version = airCodeVersion;
    }

    const std::string &getDsHardVersion() const {
        return ds_hard_version;
    }

    void setDsHardVersion(const std::string &dsHardVersion) {
        ds_hard_version = dsHardVersion;
    }

    const std::string &getDsSoftVersion() const {
        return ds_soft_version;
    }

    void setDsSoftVersion(const std::string &dsSoftVersion) {
        ds_soft_version = dsSoftVersion;
    }

    const std::string &getAppPadVersion() const {
        return app_pad_version;
    }

    void setAppPadVersion(const std::string &appPadVersion) {
        app_pad_version = appPadVersion;
    }

    int getMachineVersion() const {
        return machine_version;
    }

    void setMachineVersion(int machineVersion) {
        machine_version = machineVersion;
    }

    int getSelfCheckVersion() const {
        return self_check_version;
    }

    void setSelfCheckVersion(int selfCheckVersion) {
        self_check_version = selfCheckVersion;
    }

};


#endif //APP_COMMUNICATION_VERSIONMANAGER_H
