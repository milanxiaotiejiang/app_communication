//
// Created by lijiang on 2021/12/17.
//

#ifndef APP_COMMUNICATION_DEVICESTATUS_H
#define APP_COMMUNICATION_DEVICESTATUS_H

#include "MapInfo.h"
#include "Task.h"
#include "WorkStatus.h"

#include "nlohmann/json.hpp"

using json = nlohmann::json;

class DeviceStatus {
private:
    int m_RSOC;
    long m_clean_area;
    long m_clean_time;
    MapInfo m_current_map;
    bool m_is_urgency_stop;
    bool m_lock_status;
    bool m_dust_box_status;
    bool m_water_box_status;
    WorkStatus m_work_status;
    std::string m_work_status_message;
    int m_work_status_code;


public:
    DeviceStatus();

    DeviceStatus(int RSOC, long clean_area, long clean_time, const MapInfo &current_map, bool is_urgency_stop,
                 bool lock_status, bool dust_box_status, bool water_box_status,
                 const WorkStatus &work_status, const std::string &work_status_message, const int &work_status_code);

    virtual ~DeviceStatus();


    int getRSOC() const;

    void setRSOC(int battery);

    long getCleanArea() const;

    void setCleanArea(long cleanArea);

    long getCleanTime() const;

    void setCleanTime(long cleanTime);

    const MapInfo &getCurrentMap() const;

    void setCurrentMap(const MapInfo &currentMap);

    bool getLockStatus() const;

    void setLockStatus(bool lockStatus);

    bool getDustBoxStatus() const;

    void setDustBoxStatus(bool dustBoxStatus);

    bool getWaterBoxStatus() const;

    void setWaterBoxStatus(bool waterBoxStatus);

    const WorkStatus &getWorkStatus() const;

    void setWorkStatus(const WorkStatus &workStatus);

    const std::string &getWorkStatusMessage() const;

    void setWorkStatusMessage(const std::string &workStatusMessage);

    int getWorkStatusCode() const;

    void setWorkStatusCode(const int &work_status_code);

    friend void to_json(json &j, const DeviceStatus &b) {
        j = json{
                {"battery",             b.m_RSOC},
                {"clean_area",          b.m_clean_area},
                {"clean_time",          b.m_clean_time},
                {"current_map",         b.m_current_map},
                {"is_urgency_stop",     b.m_is_urgency_stop},
                {"lock_status",         b.m_lock_status},
                {"dust_box_status",     b.m_dust_box_status},
                {"water_box_status",    b.m_water_box_status},
                {"work_status",         b.m_work_status},
                {"work_status_message", b.m_work_status_message},
                {"work_status_code",    b.m_work_status_code},
        };
    }

    friend void from_json(const json &j, DeviceStatus &b) {
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
    }
};


#endif//APP_COMMUNICATION_DEVICESTATUS_H
