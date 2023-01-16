//
// Created by lijiang on 2021/12/22.
//

#ifndef APP_COMMUNICATION_ShowCloudStatus_H
#define APP_COMMUNICATION_ShowCloudStatus_H

#include "stdlib.h"
#include "string"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class ShowCloudStatus {
private:
    int electric;
    int mop;
    int sweep;
    int water;
    std::string work_status_message;
    int m_work_status_code;
    bool m_emergency_stop_status;
    long m_current_execute_time;
    bool m_is_charging;
    int arom_status;
    int m_absorb_status;
    int m_push_status;
    bool m_lock_status;
    bool m_dust_box_status;
    std::string m_map_name;
    int m_map_id;
    long m_clean_area;
    int m_clean_count;


public:
    ShowCloudStatus(int electric, int mop, int sweep, int water, const std::string &workStatusMessage,
                    int work_status_code, bool emergency_stop_status,
                    long current_execute_time,
                    bool is_charging,
                    int arom,
                    int absorb_status,
                    int push_status,
                    bool lock_status,
                    bool dust_box_status,
                    std::string map_name,
                    int map_id,
                    long clean_area,
                    int clean_count);

    virtual ~ShowCloudStatus();

    int getElectric() const;

    void setElectric(int electric);

    int getMop() const;

    void setMop(int mop);

    int getSweep() const;

    void setSweep(int sweep);

    int getWater() const;

    void setWater(int water);

    int getArom() const;

    void setArom(int arom);

    const std::string &getWorkStatusMessage() const;

    void setWorkStatusMessage(const std::string &workStatusMessage);

    int getWorkStatusCode() const {
        return m_work_status_code;
    }

    void setWorkStatusCode(int work_status_code) {
        m_work_status_code = work_status_code;
    }

    int getabsorb_status() const { return m_absorb_status; }

    int getpush_status() const { return m_push_status; }

    bool getlock_status() const { return m_push_status; }

    bool getdust_box_status() const { return m_dust_box_status; }

    std::string getmap_name() const { return m_map_name; }

    int getmap_id() const { return m_map_id; }

    long getclean_area() const { return m_clean_area; }

    int getclean_count() const { return m_clean_count; }

    int setabsorb_status(int n) { m_absorb_status = n; }

    int setpush_status(int n) { m_push_status = n; }

    bool setlock_status(bool n) { m_push_status = n; }

    bool setdust_box_status(bool n) { m_dust_box_status = n; }

    std::string setmap_name(std::string n) { m_map_name = n; }

    int setmap_id(int n) { return m_map_id = n; }

    long setclean_area(long n) { m_clean_area = n; }

    int setclean_count(int n) { m_clean_count = n; }

    friend void to_json(json &j, const ShowCloudStatus &b) {
        j = json{
                {"battery",             b.electric},
                {"drag_status",          b.mop},
                {"sweep_status",        b.sweep},
                {"water_box_status",    b.water},
                {"work_status_message", b.work_status_message},
                {"work_status_code",    b.m_work_status_code},
                {"is_urgency_stop",     b.m_emergency_stop_status},
                {"clean_time",          b.m_current_execute_time},
                {"is_charging",         b.m_is_charging},
                {"aromaDiffuser",       b.arom_status},
                {"absorb_status",       b.m_absorb_status},
                {"push_status",         b.m_push_status},
                {"lock_status",         b.m_lock_status},
                {"dust_box_status",     b.m_dust_box_status},
                {"map_name",            b.m_map_name},
                {"map_id",              b.m_map_id},
                {"clean_area",          b.m_clean_area},
                {"clean_count",         b.m_clean_count},

        };
    }

    friend void from_json(const json &j, ShowCloudStatus &b) {
        j.at("battery").get_to(b.electric);
        j.at("drag_status").get_to(b.mop);
        j.at("sweep_status").get_to(b.sweep);
        j.at("water_box_status").get_to(b.water);
        j.at("work_status_message").get_to(b.work_status_message);
        j.at("work_status_code").get_to(b.m_work_status_code);
        j.at("clean_time").get_to(b.m_current_execute_time);
        j.at("is_urgency_stop").get_to(b.m_emergency_stop_status);
        j.at("is_charging").get_to(b.m_is_charging);
        j.at("aromaDiffuser").get_to(b.arom_status);
        j.at("absorb_status").get_to(b.m_absorb_status);
        j.at("push_status").get_to(b.m_push_status);
        j.at("lock_status").get_to(b.m_lock_status);
        j.at("dust_box_status").get_to(b.m_dust_box_status);
        j.at("map_name").get_to(b.m_map_name);
        j.at("map_id").get_to(b.m_map_id);
        j.at("clean_area").get_to(b.m_clean_area);
        j.at("clean_count").get_to(b.m_clean_count);
    }
};


#endif//APP_COMMUNICATION_ShowCloudStatus_H
