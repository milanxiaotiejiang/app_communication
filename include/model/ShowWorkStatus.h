//
// Created by lijiang on 2021/12/22.
//

#ifndef APP_COMMUNICATION_SHOWWORKSTATUS_H
#define APP_COMMUNICATION_SHOWWORKSTATUS_H

#include "stdlib.h"
#include "string"
#include "model/WorkStatus.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class ShowWorkStatus {
private:
    int electric;
    int water;
    int sewage;
    WorkStatus work_status;
    std::string work_status_message;
    int m_work_status_code;
    bool m_emergency_stop_status;
    long m_current_execute_time;
    bool m_is_charging;
    int arom_status;


public:

    ShowWorkStatus(int electric, int water, int sewage, const WorkStatus &workStatus,
                   const std::string &workStatusMessage, int mWorkStatusCode, bool mEmergencyStopStatus,
                   long mCurrentExecuteTime, bool mIsCharging, int aromStatus) : electric(electric), water(water),
                                                                                 sewage(sewage),
                                                                                 work_status(workStatus),
                                                                                 work_status_message(workStatusMessage),
                                                                                 m_work_status_code(mWorkStatusCode),
                                                                                 m_emergency_stop_status(
                                                                                         mEmergencyStopStatus),
                                                                                 m_current_execute_time(
                                                                                         mCurrentExecuteTime),
                                                                                 m_is_charging(mIsCharging),
                                                                                 arom_status(aromStatus) {}

    int getElectric() const {
        return electric;
    }

    void setElectric(int electric) {
        ShowWorkStatus::electric = electric;
    }

    int getWater() const {
        return water;
    }

    void setWater(int water) {
        ShowWorkStatus::water = water;
    }

    int getSewage() const {
        return sewage;
    }

    void setSewage(int sewage) {
        ShowWorkStatus::sewage = sewage;
    }

    const WorkStatus &getWorkStatus() const {
        return work_status;
    }

    void setWorkStatus(const WorkStatus &workStatus) {
        work_status = workStatus;
    }

    const std::string &getWorkStatusMessage() const {
        return work_status_message;
    }

    void setWorkStatusMessage(const std::string &workStatusMessage) {
        work_status_message = workStatusMessage;
    }

    int getMWorkStatusCode() const {
        return m_work_status_code;
    }

    void setMWorkStatusCode(int mWorkStatusCode) {
        m_work_status_code = mWorkStatusCode;
    }

    bool isMEmergencyStopStatus() const {
        return m_emergency_stop_status;
    }

    void setMEmergencyStopStatus(bool mEmergencyStopStatus) {
        m_emergency_stop_status = mEmergencyStopStatus;
    }

    long getMCurrentExecuteTime() const {
        return m_current_execute_time;
    }

    void setMCurrentExecuteTime(long mCurrentExecuteTime) {
        m_current_execute_time = mCurrentExecuteTime;
    }

    bool isMIsCharging() const {
        return m_is_charging;
    }

    void setMIsCharging(bool mIsCharging) {
        m_is_charging = mIsCharging;
    }

    int getAromStatus() const {
        return arom_status;
    }

    void setAromStatus(int aromStatus) {
        arom_status = aromStatus;
    }

    friend void to_json(json &j, const ShowWorkStatus &b) {
        j = json{
                {"electric",              b.electric},
                {"water",                 b.water},
                {"sewage",                b.sewage},
                {"work_status",           b.work_status},
                {"work_status_message",   b.work_status_message},
                {"work_status_code",      b.m_work_status_code},
                {"emergency_stop_status", b.m_emergency_stop_status},
                {"current_execute_time",  b.m_current_execute_time},
                {"is_charging",           b.m_is_charging},
                {"aromaDiffuser",         b.arom_status},
        };
    }

    friend void from_json(const json &j, ShowWorkStatus &b) {
        j.at("electric").get_to(b.electric);
        j.at("water").get_to(b.water);
        j.at("sewage").get_to(b.sewage);
        j.at("work_status").get_to(b.work_status);
        j.at("work_status_message").get_to(b.work_status_message);
        j.at("current_execute_time").get_to(b.m_work_status_code);
        j.at("emergency_stop_status").get_to(b.m_emergency_stop_status);
        j.at("current_execute_time").get_to(b.m_current_execute_time);
        j.at("is_charging").get_to(b.m_is_charging);
        j.at("aromaDiffuser").get_to(b.arom_status);
    }
};

#endif//APP_COMMUNICATION_SHOWWORKSTATUS_H
