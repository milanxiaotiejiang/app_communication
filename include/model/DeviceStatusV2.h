//
// Created by mi on 2022/4/27.
//

#ifndef APP_COMMUNICATION_DEVICESTATUSV2_H
#define APP_COMMUNICATION_DEVICESTATUSV2_H

#include "MapInfo.h"
#include "Task.h"
#include "WorkStatus.h"
#include "model/Location.h"
#include "model/UpgradeModel.h"
#include "nlohmann/json.hpp"

using json = nlohmann::json;

class DeviceStatusV2 {
private:
    int m_RSOC;
    long m_clean_area;
    long m_clean_time;
    MapInfo m_current_map;
    bool m_is_urgency_stop;
    bool m_lock_status;
    bool m_dust_box_status;
    bool m_water_box_status;
    WorkStatusUpgrade m_work_status;
    std::string m_work_status_message;
    int m_work_status_code;
    int aromatherapy_status{-1};
    string version;
    bool has_face_recognition;
    bool has_base_station;
    bool has_knob;
    bool has_voice;
    string device_ros_version;
    string ecu_soft_version;
    string ecu_hard_version;
    string pad_version;
public:
    DeviceStatusV2();

    DeviceStatusV2(int mRsoc, long mCleanArea, long mCleanTime, const MapInfo &mCurrentMap,
                   bool mIsUrgencyStop, bool mLockStatus, bool mDustBoxStatus, bool mWaterBoxStatus,
                   const WorkStatusUpgrade &mWorkStatus,
                   const string &mWorkStatusMessage, int mWorkStatusCode, const string &version,
                   bool has_facerecognition,
                   bool has_basestation, bool hasknob, bool hasvoice, const string &device_ros_version,
                   const string &ecu_softversion,
                   const string &ecu_hardversion, const string &pad_version, int aromstatus);

    int getMRsoc() const;

    void setMRsoc(int mRsoc);

    long getMCleanArea() const;

    void setMCleanArea(long mCleanArea);

    long getMCleanTime() const;

    void setMCleanTime(long mCleanTime);

    const MapInfo &getMCurrentMap() const;

    void setMCurrentMap(const MapInfo &mCurrentMap);

    bool isMIsUrgencyStop() const;

    void setMIsUrgencyStop(bool mIsUrgencyStop);

    bool isMLockStatus() const;

    void setMLockStatus(bool mLockStatus);

    bool isMDustBoxStatus() const;

    void setMDustBoxStatus(bool mDustBoxStatus);

    bool isMWaterBoxStatus() const;

    void setMWaterBoxStatus(bool mWaterBoxStatus);

    const WorkStatusUpgrade &getMWorkStatus() const;

    void setMWorkStatus(const WorkStatusUpgrade &mWorkStatus);

    const string &getMWorkStatusMessage() const;

    void setMWorkStatusMessage(const string &mWorkStatusMessage);

    int getMWorkStatusCode() const;

    void setMWorkStatusCode(int mWorkStatusCode);

    const string &getVersion() const;

    void setVersion(const string &version);

    const string &getdevice_ros_version() const;

    void setdevice_ros_version(const string &version);

    const string &getecu_softversion() const;

    void setecu_softversion(const string &version);

    const string &getecu_hardversion() const;

    void setecu_hardversion(const string &version);

    const string &getpad_version() const;

    void setpad_version(const string &version);


    bool ishas_face_recognition() const;

    void sethas_face_recognition(bool face_recognition);

    bool ishas_base_station() const;

    void sethas_base_station(bool base_recognition);

    bool ishas_knob() const;

    void sethas_knob(bool knob);

    bool ishas_voice() const;

    void sethas_voice(bool hvoice);

    int getAromStatus() const;

    void setAromStatus(int code);

    friend void to_json(json &j, const DeviceStatusV2 &b) {
        j = json{
                {"battery",              b.m_RSOC},
                {"clean_area",           b.m_clean_area},
                {"clean_time",           b.m_clean_time},
                {"current_map",          b.m_current_map},
                {"is_urgency_stop",      b.m_is_urgency_stop},
                {"lock_status",          b.m_lock_status},
                {"dust_box_status",      b.m_dust_box_status},
                {"water_box_status",     b.m_water_box_status},
                {"work_status",          b.m_work_status},
                {"work_status_message",  b.m_work_status_message},
                {"work_status_code",     b.m_work_status_code},
                {"has_face_recognition", b.has_face_recognition},
                {"has_base_station",     b.has_base_station},
                {"has_knob",             b.has_knob},
                {"has_voice",            b.has_voice},
                {"device_ros_version",   b.device_ros_version},
                {"ecu_soft_version",     b.ecu_soft_version},
                {"ecu_hard_version",     b.ecu_hard_version},
                {"pad_version",          b.pad_version},
                {"version",              b.version},
                {"aromatherapy_status",  b.aromatherapy_status},


        };
    }

    friend void from_json(const json &j, DeviceStatusV2 &b) {
        j.at("battery").get_to(b.m_RSOC);
        j.at("clean_area").get_to(b.m_clean_area);
        j.at("clean_time").get_to(b.m_clean_time);
        j.at("current_map").get_to(b.m_current_map);
        j.at("is_urgency_stop").get_to(b.m_is_urgency_stop);
        j.at("lock_status").get_to(b.m_lock_status);
        j.at("dust_box_status").get_to(b.m_dust_box_status);
        j.at("water_box_status").get_to(b.m_water_box_status);
        j.at("work_status").get_to(b.m_work_status);
        j.at("work_status_message").get_to(b.m_work_status_message);
        j.at("work_status_code").get_to(b.m_work_status_code);
        j.at("version").get_to(b.version);
        j.at("has_face_recognition").get_to(b.has_face_recognition);
        j.at("has_base_station").get_to(b.has_base_station);
        j.at("has_knob").get_to(b.has_knob);
        j.at("has_voice").get_to(b.has_voice);
        j.at("ecu_soft_version").get_to(b.ecu_soft_version);
        j.at("ecu_hard_version").get_to(b.ecu_hard_version);
        j.at("pad_version").get_to(b.pad_version);
        j.at("device_ros_version").get_to(b.device_ros_version);
        j.at("aromatherapy_status").get_to(b.aromatherapy_status);

    }
};


#endif //APP_COMMUNICATION_DEVICESTATUSV2_H
