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
#ifndef APP_COMMUNICATION_CLEANHISTORYMANAGER_H
#define APP_COMMUNICATION_CLEANHISTORYMANAGER_H

#include "model/CleanHistory.h"
#include "tool/write_file.hpp"
#include <ros/package.h>

class CleanHistoryManager {

private:
    string m_filename;
    //sh::File *m_file_ptr;
    std::shared_ptr<sh::File> m_file_ptr;
    std::string m_file_string;
    std::string m_base64Decode;
    std::string m_base64Encode;
    json m_basicJson;
    CleanHistoryList m_CleanHistoryList;


public:
    CleanHistoryManager() {
        m_filename.append(ros::package::getPath("data_base"));
        m_filename.append("/config/clean_history_principal_json.txt");
        // m_filename.append("/home/admin1/test_ws/src/data_base/config/clean_history.txt");
        m_file_ptr = make_shared<sh::File>(m_filename);//new sh::File(m_filename);
    }

    ~CleanHistoryManager();

    bool FileOpenRead();

    bool AddCleanHistory(const CleanHistory &clean_history);

    bool DelateAllCleanHistory();

    bool ResetCleanHistory(const CleanHistory &clean_history, const string &taskId);

    bool GetCleanHistory(CleanHistory &clean_history, const string &taskId);

    bool GetLatestCleanHistory(CleanHistory &clean_history);
    bool ShowAllCleanHistory();

private:
    static CleanHistoryManager *m_instance_ptr;


public:
    static CleanHistoryManager *get_instance() {
        if (m_instance_ptr == nullptr) {
            m_instance_ptr = new CleanHistoryManager;
        }
        return m_instance_ptr;
    }
};


#endif//APP_COMMUNICATION_CLEANHISTORYMANAGER_H