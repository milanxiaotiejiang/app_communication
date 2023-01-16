//
// Created by lijiang on 2021/12/17.
//

#include "model/DeviceStatus.h"

DeviceStatus::DeviceStatus() {}

DeviceStatus::~DeviceStatus() {
}

DeviceStatus::DeviceStatus(int RSOC, long clean_area, long clean_time,
                           const MapInfo &current_map, bool is_urgency_stop,
                           bool lock_status, bool dust_box_status, bool water_box_status,
                           const WorkStatus &work_status, const string &work_status_message,
                           const int &work_status_code) : m_RSOC(RSOC),
                                                          m_clean_area(clean_area),
                                                          m_clean_time(clean_time),
                                                          m_current_map(current_map),
                                                          m_is_urgency_stop(is_urgency_stop),
                                                          m_lock_status(lock_status),
                                                          m_dust_box_status(dust_box_status),
                                                          m_water_box_status(water_box_status),
                                                          m_work_status(work_status),
                                                          m_work_status_message(work_status_message),
                                                          m_work_status_code(work_status_code) {}

int DeviceStatus::getRSOC() const {
    return m_RSOC;
}

void DeviceStatus::setRSOC(int RSOC) {
    DeviceStatus::m_RSOC = RSOC;
}

long DeviceStatus::getCleanArea() const {
    return m_clean_area;
}

void DeviceStatus::setCleanArea(long cleanArea) {
    m_clean_area = cleanArea;
}

long DeviceStatus::getCleanTime() const {
    return m_clean_time;
}

void DeviceStatus::setCleanTime(long cleanTime) {
    m_clean_time = cleanTime;
}

const MapInfo &DeviceStatus::getCurrentMap() const {
    return m_current_map;
}

void DeviceStatus::setCurrentMap(const MapInfo &currentMap) {
    m_current_map = currentMap;
}


bool DeviceStatus::getLockStatus() const {
    return m_lock_status;
}

void DeviceStatus::setLockStatus(bool lockStatus) {
    m_lock_status = lockStatus;
}

bool DeviceStatus::getDustBoxStatus() const {
    return m_dust_box_status;
}

void DeviceStatus::setDustBoxStatus(bool dustBoxStatus) {
    m_dust_box_status = dustBoxStatus;
}

bool DeviceStatus::getWaterBoxStatus() const {
    return m_water_box_status;
}

void DeviceStatus::setWaterBoxStatus(bool waterBoxStatus) {
    m_water_box_status = waterBoxStatus;
}

const WorkStatus &DeviceStatus::getWorkStatus() const {
    return m_work_status;
}

void DeviceStatus::setWorkStatus(const WorkStatus &workStatus) {
    m_work_status = workStatus;
}

const string &DeviceStatus::getWorkStatusMessage() const {
    return m_work_status_message;
}

void DeviceStatus::setWorkStatusMessage(const string &workStatusMessage) {
    m_work_status_message = workStatusMessage;
}

int DeviceStatus::getWorkStatusCode() const {
    return m_work_status_code;
}

void DeviceStatus::setWorkStatusCode(const int &work_status_code) {
    m_work_status_code = work_status_code;
}