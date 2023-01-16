//
// Created by admin1 on 22-12-13.
//

#ifndef APP_COMMUNICATION_FULLCLEANMANAGER_H
#define APP_COMMUNICATION_FULLCLEANMANAGER_H

#include <ros/package.h>
#include "tool/write_file.hpp"
#include "model/FullClean.h"

class FullCleanManager {

private:
    std::string m_filename;
    std::shared_ptr<sh::File> m_file_ptr;
    std::string m_file_string;
    std::string m_base64Decode;
    std::string m_base64Encode;
    json m_basicJson;
    FullCleanBriefList m_full_clean_brief_list;
    bool file_empty;

public:
    FullCleanManager(){
        m_filename.append(ros::package::getPath("data_base"));
        m_filename.append("/config/fullclean_list_primcipal_json_work.txt");
        m_file_ptr = make_shared<sh::File>(m_filename);
    }

    ~FullCleanManager();

    bool FileOpenRead();

    bool FileSave();

    int GetFullCleanBriefList(FullCleanBriefList &fullCleanBriefList);

    int AddFullClean(const FullCleanBrief &fullCleanBrief);

    int DeleteFullClean(const std::string &full_clean_id);

    int ResetFullCLean(const FullCleanBrief &fullCleanBrief, const std::string &full_clean_id);

    int GetFullCLeanBrief(FullCleanBrief &fullCleanBrief, const std::string &full_clean_id);

    int setMainFullClean(const std::string &full_clean_id);

    int cancelMainFullCLean(const std::string &full_clean_id);

private:

    static FullCleanManager *m_instance_ptr;

public:
    static FullCleanManager *get_instance(){
        if(m_instance_ptr == nullptr){
            m_instance_ptr = new FullCleanManager;
        }
        return m_instance_ptr;
    }
};


#endif //APP_COMMUNICATION_FULLCLEANMANAGER_H
