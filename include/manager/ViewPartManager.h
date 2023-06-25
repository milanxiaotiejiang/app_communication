/**
 * @file ViewPartManager.cpp
 * @author Quan Li (you@domain.com)
 * @brief 用于操控ViewPartList
 * @version 0.1
 * @date 2022-03-04
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef APP_COMMUNICATION_VIEWPARTMANAGER_H
#define APP_COMMUNICATION_VIEWPARTMANAGER_H

#include "model/ViewPart.h"
#include "tool/write_file.hpp"
#include "db/path.h"
#include <ros/package.h>

enum StartegyResult {
    FAILD_TO_OPEN_FILE_ = 0,
    NAME_DUPLICATE_ = 1,
    EXECUTE_FAILED_,
    FAILED_TO_SAVE_FILE_,
    SUCCESS_
};

class ViewPartManager {

private:
    std::string m_filename;
    //sh::File *m_file_ptr;
    std::shared_ptr<sh::File> m_file_ptr;
    std::string m_file_string;
//    std::string m_base64Decode;
    std::string m_base64Encode;
    json m_basicJson;
    ViewPartList m_view_part_list;
    bool file_empty;


public:
    ViewPartManager() {
        m_filename.append(path::data_base_config_dir());
        m_filename.append("view_part_principal_json.txt");
        // m_filename.append("/home/admin1/test_ws/src/data_base/config/view_part.txt");
        // m_file_ptr = new sh::File(m_filename);
        m_file_ptr = std::make_shared<sh::File>(m_filename);
    }

    ~ViewPartManager();

    bool FileOpenRead();

    bool FileSave();

    int AddViewPart(const ViewPart &view_part);

    int DelateViewPart(const std::string &part_id);

    int ResetViewPart(const ViewPart &view_part, const std::string &part_id);

    int GetViewPart(ViewPart &view_part, const std::string &part_id);

    int GetViewPartList(ViewPartList &view_part_list);

    bool ShowAllViewPart();

private:
    static ViewPartManager *m_instance_ptr;


public:
    static ViewPartManager *get_instance() {
        if (m_instance_ptr == nullptr) {
            m_instance_ptr = new ViewPartManager;
        }
        return m_instance_ptr;
    }
};


#endif//APP_COMMUNICATION_VIEWPARTMANAGER_H