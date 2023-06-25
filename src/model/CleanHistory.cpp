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

                           int task_type) {
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

const std::vector<CleanHistory> &CleanHistoryList::GetCleanHistoryList() const {
    return m_clean_history_list;
}

void CleanHistoryList::SetCleanHistoryList(const std::vector<CleanHistory> &clean_history_list) {
    m_clean_history_list = clean_history_list;
}

bool CleanHistoryList::AddCleanHistory(const CleanHistory &clean_history) {
    //Ewen change begin
    if (m_clean_history_list.size() > 0 && m_clean_history_list.back().getTaskID() == clean_history.getTaskID()) {
        //Ewen change end
        std::cout << "new task id equals to the latest task id, refuse to add" << std::endl;
    } else {
        m_clean_history_list.push_back(clean_history);
    }
    for (auto &item: m_clean_history_list) {
//        cout<<"taskID"<<item.getTaskID()<<endl;
    }
}

bool CleanHistoryList::GetCleanHistory(CleanHistory &clean_history, const std::string &taskId) {
    for (auto &item: m_clean_history_list) {
//        cout<<"taskID"<<item.getTaskID()<<endl;
        if (item.getTaskID() == taskId) {
            clean_history = item;
//            cout << "At CleanHistoryList::GetCleanHistory successfully match taskid:"<<taskId << endl;
            return true;
        }
    }
//    cout << "At CleanHistoryList::GetCleanHistory can't match taskid:"<<taskId << endl;
    return false;
}

bool CleanHistoryList::ResetCleanHistory(const CleanHistory &clean_history, const std::string &taskId) {
    for (auto &item: m_clean_history_list) {
        if (item.getTaskID() == taskId) {
//            cout << "At CleanHistoryList::ResetCleanHistory successfully match taskid:"<<taskId << endl;
            item = clean_history;
            return true;
        }
    }
//    cout << "At CleanHistoryList::ResetCleanHistory can't match taskid:"<<taskId << endl;
    return false;
}

void CleanHistoryList::ShowAllCleanHistory() {
    for (auto &item: m_clean_history_list) {
//        cout<<"是否完成"<<item.getIsComplete()<<endl;
//        cout<<"下发时间"<<item.getLaunchTime()<<endl;
//        cout<<"执行时间"<<item.getExecuteTime()<<endl;
//        cout<<"结束时间"<<item.getEndTime()<<endl;
//        cout<<"任务模式"<<item.getTaskMode()<<endl;
//        cout<<"任务ID"<<item.getTaskID()<<endl;
//        // cout<<"工作状态"<<item.getWorkStatus()<<endl;
//        cout<<"清洁面积"<<item.getCleanArea()<<endl;
    }
}
