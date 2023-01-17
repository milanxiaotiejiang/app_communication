/**
 * @file CleanHistoryManager.cpp
 * @author Quan Li (1204018828@qq.com)
 * @brief 用于操控历史储存文件，
 * @version 0.1
 * @date 2022-02-21
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "manager/CleanHistoryManager.h"
#include "glog/logging.h"

CleanHistoryManager::~CleanHistoryManager() {}

bool CleanHistoryManager::FileOpenRead() {
    if (!sh::File::exists(m_filename)) {
        unique_ptr<sh::File> uFilePtr(new sh::File(m_filename));
        if (!uFilePtr->create(m_filename)) {
            cout << "create file " << m_filename << " fail !!!";
            return false;
        }
    }
    if (m_file_ptr->open(std::ios::in)) {
        m_file_string = m_file_ptr->readAll();
        return true;
    } else {
        cout << "CleanHistoryManager fail to open file" << endl;
        return false;
    }
}

bool CleanHistoryManager::AddCleanHistory(const CleanHistory &clean_history) {
    if (m_file_ptr->open(std::ios::in)) {//读取文件
        m_file_string = m_file_ptr->readAll();
        m_file_ptr->close();
    } else {
        cout << "fail to open file" << endl;
        return false;
    }
    if (m_file_string.size() > 0) {
        m_basicJson = json::parse(m_file_string);               //转json
        cout << "At CleanHistoryManager::AddCleanHistory  000000000 " << endl;
        m_CleanHistoryList = m_basicJson.get<CleanHistoryList>();//转数据
    }
    m_CleanHistoryList.AddCleanHistory(clean_history);//增加一条历史
    m_basicJson = m_CleanHistoryList;                 //转json
    m_base64Encode.clear();
    m_base64Encode.append(m_basicJson.dump());//转base64
    sh::File::saveTextTo(m_filename, m_base64Encode);        //储存

}

bool CleanHistoryManager::DelateAllCleanHistory() {
    if (m_file_ptr->open(std::ios::in)) {//读取文件
        m_file_string = m_file_ptr->readAll();
    } else {
        cout << "At CleanHistoryManager::DelateAllCleanHistory fail to open file" << endl;
        return false;
    }
    if (m_file_string.size() > 0) {
        m_file_string.clear();//清空
    }
    sh::File::saveTextTo(m_filename, m_file_string);//储存
    return true;
}

bool CleanHistoryManager::ResetCleanHistory(const CleanHistory &clean_history, const string &taskId) {
    if (m_file_ptr->open(std::ios::in)) {//读取文件
        m_file_string = m_file_ptr->readAll();
    } else {
        cout << "AT CleanHistoryManager::ResetCleanHistory fail to open file" << endl;
        return false;
    }
    m_file_ptr->close();
    if (m_file_string.size() > 0) {
        m_basicJson = json::parse(m_file_string);               //转json
        m_CleanHistoryList = m_basicJson.get<CleanHistoryList>();//转数据
    }
    if (!m_CleanHistoryList.ResetCleanHistory(clean_history, taskId)) {
        //   cout << "AT CleanHistoryManager::ResetCleanHistory return false" << endl;
        return false;
    }

    m_basicJson = m_CleanHistoryList;//转json
    m_base64Encode.clear();
    m_base64Encode.append(m_basicJson.dump());//转base64
    sh::File::saveTextTo(m_filename, m_base64Encode);        //储存
}

bool CleanHistoryManager::GetCleanHistory(CleanHistory &clean_history, const string &task_Id) {
    if (m_file_ptr->open(std::ios::in)) {//读取文件
        m_file_string = m_file_ptr->readAll();
    } else {
        cout << "fail to open file" << endl;
        return false;
    }
    m_file_ptr->close();
    ///LOG(ERROR) << "At CleanHistoryManager::GetCleanHistory before parse " << m_file_string << endl;
    if (m_file_string.size() > 0) {
        try {
            m_basicJson = json::parse(m_file_string);
        }
        catch (...) {
            LOG(ERROR) << "error!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
            cout << m_file_string << endl;
            cout << "error!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
            return false;
        }
        //转json
        ///LOG(ERROR) << "At CleanHistoryManager::GetCleanHistory  2222222 " << endl;
        m_CleanHistoryList = m_basicJson.get<CleanHistoryList>();//转数据
        //  cout << "At CleanHistoryManager::GetCleanHistory  3333333 " << endl;
    }
    //找到taskid匹配的任务，并赋值给clean_history
    if (!m_CleanHistoryList.GetCleanHistory(clean_history, task_Id)) {
        // cout << "At CleanHistoryManager::GetCleanHistory can't match taskid:" << task_Id << endl;
        return false;
    }
}

bool CleanHistoryManager::GetLatestCleanHistory(CleanHistory &clean_history) {
    if (m_file_ptr->open(std::ios::in)) {//读取文件
        m_file_string = m_file_ptr->readAll();
    } else {
        cout << "fail to open file" << endl;
        return false;
    }
    m_file_ptr->close();
    if (m_file_string.size() > 0) {
        m_basicJson = json::parse(m_file_string);                //转json
        m_CleanHistoryList = m_basicJson.get<CleanHistoryList>();//转数据
    }
    //找到taskid匹配的任务，并赋值给clean_history
    if (!m_CleanHistoryList.GetLatestCleanHistory(clean_history)) {
        return false;
    }
    return true;
}

bool CleanHistoryManager::ShowAllCleanHistory() {
    if (m_file_ptr->open(std::ios::in)) {//读取文件
        m_file_string = m_file_ptr->readAll();
    } else {
        cout << "At CleanHistoryManager::ShowAllCleanHistory fail to open file" << endl;
        return false;
    }
    m_file_ptr->close();
    if (m_file_string.size() > 0) {
        m_basicJson = json::parse(m_file_string);               //转json
        m_CleanHistoryList = m_basicJson.get<CleanHistoryList>();//转数据
    }
    //找到taskid匹配的任务，并赋值给clean_history
    if (m_CleanHistoryList.GetCleanHistoryList().size() > 0) {
        m_CleanHistoryList.ShowAllCleanHistory();
    } else {
        cout << "历史记录中无数据" << endl;
    }
}
