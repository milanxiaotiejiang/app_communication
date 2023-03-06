/**
 * @file CombinationManager.h
 * @author Quan Li (you@domain.com)
 * @brief 用于操作CombinationList
 * @version 0.1
 * @date 2022-03-04
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef APP_COMMUNICATION_COMBINATIONMANAGER_H
#define APP_COMMUNICATION_COMBINATIONMANAGER_H

#include "model/Combination.h"
#include "tool/write_file.hpp"
#include "db/path.h"
#include <ros/package.h>

class CombinationManager {

private:
    std::string m_filename;
    //sh::File *m_file_ptr;
    std::shared_ptr<sh::File> m_file_ptr;
    std::string m_file_string;
    std::string m_base64Decode;
    std::string m_base64Encode;
    json m_basicJson;
    CombinationBriefList m_combination_brief_list;
    bool file_empty;


public:
    CombinationManager() {
        m_filename.append(path::data_base_config_dir());
//        m_filename.append("/config/combination_list.txt");
        m_filename.append("combination_list_principal_json_work.txt");
        cout << "combination_list_path:" << m_filename << endl;
        // m_filename.append("/home/admin1/test_ws/src/data_base/config/combination_list.txt");
        //m_file_ptr = new sh::File(m_filename);
        m_file_ptr = make_shared<sh::File>(m_filename);
    }

    ~CombinationManager();

    bool FileOpenRead();

    bool FileSave();

    int GetCombinationBriefList(CombinationBriefList &combination_brief_list);

    int AddCombination(const CombinationDetail &combination_detail);

    int DelateCombination(const std::string &combination_id);

    int SearchPartID(const std::string &part_id);

    bool SearchPathID(const std::string &path_id);

    int DelatePartID(const std::string &part_id);

    bool DelatePathID(const std::string &path_id);

    int ResetCombination(const CombinationBrief &combination_brief, const std::string &combination_id);

    int GetCombination(CombinationBrief &combination_brief, const std::string &combination_id);

    bool ShowAllCombination();

    int setMainCombination(const std::string &combination_id);

    int cancelMainCombination(const std::string &combination_id);

private:
    static CombinationManager *m_instance_ptr;


public:
    static CombinationManager *get_instance() {
        if (m_instance_ptr == nullptr) {
            m_instance_ptr = new CombinationManager;
        }
        return m_instance_ptr;
    }
};


#endif//APP_COMMUNICATION_VIEWPARTMANAGER_H