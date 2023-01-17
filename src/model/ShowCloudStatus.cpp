//
// Created by lijiang on 2021/12/22.
//

#include "model/ShowCloudStatus.h"

ShowCloudStatus::ShowCloudStatus(int electric, int mop, int sweep, int water, const std::string &workStatusMessage,
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
                                 int clean_count) : electric(electric), mop(mop), sweep(sweep),
                                                    water(water), work_status_message(workStatusMessage),
                                                    m_work_status_code(work_status_code),
                                                    m_emergency_stop_status(emergency_stop_status),
                                                    m_current_execute_time(current_execute_time),
                                                    m_is_charging(is_charging), arom_status(arom),
                                                    m_absorb_status(absorb_status),
                                                    m_push_status(push_status),
                                                    m_lock_status(lock_status),
                                                    m_dust_box_status(dust_box_status),
                                                    m_map_name(map_name),
                                                    m_map_id(map_id),
                                                    m_clean_area(clean_area),
                                                    m_clean_count(clean_count) {}

ShowCloudStatus::~ShowCloudStatus() {
}

int ShowCloudStatus::getElectric() const {
    return electric;
}

void ShowCloudStatus::setElectric(int electric) {
    ShowCloudStatus::electric = electric;
}

int ShowCloudStatus::getMop() const {
    return mop;
}

void ShowCloudStatus::setMop(int mop) {
    ShowCloudStatus::mop = mop;
}

int ShowCloudStatus::getSweep() const {
    return sweep;
}

void ShowCloudStatus::setSweep(int sweep) {
    ShowCloudStatus::sweep = sweep;
}

int ShowCloudStatus::getWater() const {
    return water;
}

void ShowCloudStatus::setWater(int water) {
    ShowCloudStatus::water = water;
}

int ShowCloudStatus::getArom() const {
    return arom_status;
}

void ShowCloudStatus::setArom(int arom) {
    arom_status = arom;
}

const std::string &ShowCloudStatus::getWorkStatusMessage() const {
    return work_status_message;
}

void ShowCloudStatus::setWorkStatusMessage(const std::string &workStatusMessage) {
    work_status_message = workStatusMessage;
}
