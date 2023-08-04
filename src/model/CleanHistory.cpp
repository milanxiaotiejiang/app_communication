/**
 * @file CleanHistory.cpp
 * @author Quan Li (1204018828@qq.com)
 * @brief 清洁历史类函数定义
 * @version 0.1
 * @date 2022-02-21
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "model/CleanHistory.h"

CleanHistory::CleanHistory() {}

CleanHistory::CleanHistory(bool is_complete, bool base_complete,
                           long launch_time,
                           long execute_time,
                           long end_time,
                           int task_mode,
                           std::string task_id,
                           WorkStatus work_status,
                           int clean_area,
                           int clean_time,
                           int error_code,
                           std::string error_message,

                           int task_type,

                           bool renew,
                           std::string old_task_id,
                           long new_task_id) {
    m_is_complete = is_complete;
    m_base_complete = base_complete;
    m_launch_time = launch_time;
    m_execute_time = execute_time;
    m_end_time = end_time;
    m_task_mode = task_mode;
    m_task_id = task_id;
    m_work_status = work_status;
    m_clean_area = clean_area;
    m_clean_time = clean_time;
    m_error_code = error_code;
    m_error_message = error_message;
    m_task_type = task_type;

    this->renew = renew;
    this->old_task_id = old_task_id;
    this->new_task_id = new_task_id;
}

CleanHistory::~CleanHistory() {}

const bool &CleanHistory::getIsComplete() const {
    return m_is_complete;
}

void CleanHistory::setIsComplete(const bool &is_complete) {
    m_is_complete = is_complete;
}

const long &CleanHistory::getLaunchTime() const {
    return m_launch_time;
}

void CleanHistory::setLaunchTime(const long &launch_time) {
    m_launch_time = launch_time;
}

const long &CleanHistory::getExecuteTime() const {
    return m_execute_time;
}

void CleanHistory::setExecuteTime(const long &execute_time) {
    m_execute_time = execute_time;
}

const long &CleanHistory::getEndTime() const {
    return m_end_time;
}

void CleanHistory::setEndTime(const long &end_time) {
    m_end_time = end_time;
}

const int &CleanHistory::getTaskMode() const {
    return m_task_mode;
}

void CleanHistory::setTaskMode(const int &task_mode) {
    m_task_mode = task_mode;
}

const std::string &CleanHistory::getTaskID() const {
    return m_task_id;
}

void CleanHistory::setTaskID(const std::string &task_id) {
    m_task_id = task_id;
}

const WorkStatus &CleanHistory::getWorkStatus() const {
    return m_work_status;
}

void CleanHistory::setWorkStatus(const WorkStatus &work_status) {
    m_work_status = work_status;
}

const int &CleanHistory::getCleanArea() const {
    return m_clean_area;
}

void CleanHistory::setCleanArea(const int &clean_area) {
    m_clean_area = clean_area;
}

const int &CleanHistory::getCleanTime() const {
    return m_clean_time;
}

void CleanHistory::setCleanTime(const int &clean_time) {
    m_clean_time = clean_time;
}

CleanHistoryList::CleanHistoryList() {}

CleanHistoryList::~CleanHistoryList() {}

