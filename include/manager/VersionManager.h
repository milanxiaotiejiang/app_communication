//
// Created by lijiang on 2022/5/25.
// 单例新写法
//

#ifndef APP_COMMUNICATION_VERSIONMANAGER_H
#define APP_COMMUNICATION_VERSIONMANAGER_H

#include "string"

using namespace std;

class VersionManager {
private:
    string air_code_version{"0.0.0.1"};
    string ds_hard_version{"0.0.0.1"};
    string ds_soft_version{"0.0.0.1"};
    string app_pad_version{"0.0.0.1"};
    int machine_version{630};
    int self_check_version{1};
public:
    static auto &instance() {
        static VersionManager obj;
        return obj;
    }

    const string &getAirCodeVersion() const {
        return air_code_version;
    }

    void setAirCodeVersion(const string &airCodeVersion) {
        air_code_version = airCodeVersion;
    }

    const string &getDsHardVersion() const {
        return ds_hard_version;
    }

    void setDsHardVersion(const string &dsHardVersion) {
        ds_hard_version = dsHardVersion;
    }

    const string &getDsSoftVersion() const {
        return ds_soft_version;
    }

    void setDsSoftVersion(const string &dsSoftVersion) {
        ds_soft_version = dsSoftVersion;
    }

    const string &getAppPadVersion() const {
        return app_pad_version;
    }

    void setAppPadVersion(const string &appPadVersion) {
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
