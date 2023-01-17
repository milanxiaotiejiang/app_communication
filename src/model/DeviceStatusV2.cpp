//
// Created by mi on 2022/4/27.
//

#include "model/DeviceStatusV2.h"

DeviceStatusV2::DeviceStatusV2() {}

DeviceStatusV2::DeviceStatusV2(int mRsoc, long mCleanArea, long mCleanTime, const MapInfo &mCurrentMap,
                               bool mIsUrgencyStop, bool mLockStatus, bool mDustBoxStatus, bool mWaterBoxStatus,
                               const WorkStatusUpgrade &mWorkStatus, const string &mWorkStatusMessage,
                               int mWorkStatusCode,
                               const string &version, bool has_facerecognition,
                               bool has_basestation, bool hasknob, bool hasvoice, const string &device_rosversion,
                               const string &ecu_softversion, const string &ecu_hardversion,
                               const string &padversion, int aromstatus) : m_RSOC(mRsoc), m_clean_area(mCleanArea),
                                                                           m_clean_time(mCleanTime),
                                                                           m_current_map(mCurrentMap),
                                                                           m_is_urgency_stop(mIsUrgencyStop),
                                                                           m_lock_status(mLockStatus),
                                                                           m_dust_box_status(mDustBoxStatus),
                                                                           m_water_box_status(mWaterBoxStatus),
                                                                           m_work_status(mWorkStatus),
                                                                           m_work_status_message(mWorkStatusMessage),
                                                                           m_work_status_code(mWorkStatusCode),
                                                                           version(version),
                                                                           has_face_recognition(has_facerecognition),
                                                                           has_base_station(has_basestation),
                                                                           has_knob(hasknob), has_voice(hasvoice),
                                                                           device_ros_version(device_rosversion),
                                                                           ecu_soft_version(ecu_softversion),
                                                                           ecu_hard_version(ecu_hardversion),
                                                                           pad_version(padversion),
                                                                           aromatherapy_status(aromstatus) {}

int DeviceStatusV2::getMRsoc() const {
    return m_RSOC;
}

void DeviceStatusV2::setMRsoc(int mRsoc) {
    m_RSOC = mRsoc;
}

long DeviceStatusV2::getMCleanArea() const {
    return m_clean_area;
}

void DeviceStatusV2::setMCleanArea(long mCleanArea) {
    m_clean_area = mCleanArea;
}

long DeviceStatusV2::getMCleanTime() const {
    return m_clean_time;
}

void DeviceStatusV2::setMCleanTime(long mCleanTime) {
    m_clean_time = mCleanTime;
}

const MapInfo &DeviceStatusV2::getMCurrentMap() const {
    return m_current_map;
}

void DeviceStatusV2::setMCurrentMap(const MapInfo &mCurrentMap) {
    m_current_map = mCurrentMap;
}

bool DeviceStatusV2::isMIsUrgencyStop() const {
    return m_is_urgency_stop;
}

void DeviceStatusV2::setMIsUrgencyStop(bool mIsUrgencyStop) {
    m_is_urgency_stop = mIsUrgencyStop;
}

bool DeviceStatusV2::isMLockStatus() const {
    return m_lock_status;
}

void DeviceStatusV2::setMLockStatus(bool mLockStatus) {
    m_lock_status = mLockStatus;
}

bool DeviceStatusV2::isMDustBoxStatus() const {
    return m_dust_box_status;
}

void DeviceStatusV2::setMDustBoxStatus(bool mDustBoxStatus) {
    m_dust_box_status = mDustBoxStatus;
}

bool DeviceStatusV2::isMWaterBoxStatus() const {
    return m_water_box_status;
}

void DeviceStatusV2::setMWaterBoxStatus(bool mWaterBoxStatus) {
    m_water_box_status = mWaterBoxStatus;
}

const WorkStatusUpgrade &DeviceStatusV2::getMWorkStatus() const {
    return m_work_status;
}

void DeviceStatusV2::setMWorkStatus(const WorkStatusUpgrade &mWorkStatus) {
    m_work_status = mWorkStatus;
}

const string &DeviceStatusV2::getMWorkStatusMessage() const {
    return m_work_status_message;
}

void DeviceStatusV2::setMWorkStatusMessage(const string &mWorkStatusMessage) {
    m_work_status_message = mWorkStatusMessage;
}

int DeviceStatusV2::getMWorkStatusCode() const {
    return m_work_status_code;
}

void DeviceStatusV2::setMWorkStatusCode(int mWorkStatusCode) {
    m_work_status_code = mWorkStatusCode;
}

const string &DeviceStatusV2::getVersion() const {
    return version;
}

void DeviceStatusV2::setVersion(const string &version) {
    DeviceStatusV2::version = version;
}

const string &DeviceStatusV2::getdevice_ros_version() const {
    return device_ros_version;
}

void DeviceStatusV2::setdevice_ros_version(const string &version) {
    DeviceStatusV2::device_ros_version = version;
}

const string &DeviceStatusV2::getecu_softversion() const {
    return ecu_soft_version;
}

void DeviceStatusV2::setecu_softversion(const string &version) {
    DeviceStatusV2::ecu_soft_version = version;
}

const string &DeviceStatusV2::getecu_hardversion() const {
    return ecu_hard_version;
}

void DeviceStatusV2::setecu_hardversion(const string &hardversion) {
    DeviceStatusV2::ecu_hard_version = hardversion;
}

const string &DeviceStatusV2::getpad_version() const {
    return pad_version;
}

void DeviceStatusV2::setpad_version(const string &version) {
    DeviceStatusV2::pad_version = version;
}


bool DeviceStatusV2::ishas_face_recognition() const {
    return has_face_recognition;
}

void DeviceStatusV2::sethas_face_recognition(bool face_recognition) {
    has_face_recognition = face_recognition;
}

bool DeviceStatusV2::ishas_base_station() const {
    return has_base_station;
}

void DeviceStatusV2::sethas_base_station(bool base_recognition) {
    has_base_station = base_recognition;
}

bool DeviceStatusV2::ishas_knob() const {
    return has_knob;
}

void DeviceStatusV2::sethas_knob(bool knob) {
    has_knob = knob;
}

bool DeviceStatusV2::ishas_voice() const {
    return has_voice;
}

void DeviceStatusV2::sethas_voice(bool hvoice) {
    has_voice = hvoice;
}

int DeviceStatusV2::getAromStatus() const {
    return aromatherapy_status;
}

void DeviceStatusV2::setAromStatus(int code) {
    aromatherapy_status = code;
}