//
// Created by admin1 on 22-12-13.
//

#include "manager/FullCleanManager.h"
#include "manager/ViewPartManager.h"

bool FullCleanManager::FileOpenRead() {
    if (!sh::File::exists(m_filename)) {
        unique_ptr<sh::File> uFilePtr(new sh::File(m_filename));
        if (!uFilePtr->create(m_filename)) {
            LOG(ERROR) << "create file failed!!!";
            return false;
        }
    }
    if (m_file_ptr->open(std::ios::in)) {
        m_file_string = m_file_ptr->readAll();
        m_file_ptr->close();
        if (m_file_string.size() > 0) {
            m_basicJson = json::parse(m_file_string);                         //转json
            m_full_clean_brief_list = m_basicJson.get<FullCleanBriefList>();//转数据
            file_empty = false;
        } else {
            file_empty = true;
        }
        return true;
    } else {
        LOG(ERROR) << "CombinationManager fail to open file";
        return false;
    }
}

bool FullCleanManager::FileSave() {
    if (m_full_clean_brief_list.getFullCleanBriefList().size() > 0) {
        m_basicJson = m_full_clean_brief_list;//转json
        m_base64Encode.clear();
        m_base64Encode.append(m_basicJson.dump());//转base64
    } else {
        m_base64Encode.clear();
    }

    if (!sh::File::saveTextTo(m_filename, m_base64Encode)) {
        return false;
    }
    return true;
}

int FullCleanManager::GetFullCleanBriefList(FullCleanBriefList &fullCleanBriefList) {
    if (!FileOpenRead()) {
        return FAILD_TO_OPEN_FILE_;
    }
    fullCleanBriefList = m_full_clean_brief_list;
    return SUCCESS_;
}

int FullCleanManager::AddFullClean(const FullCleanBrief &fullCleanBrief) {
    if (!FileOpenRead()) {
        return FAILD_TO_OPEN_FILE_;
    }
    if (!m_full_clean_brief_list.addFullCleanBrief(fullCleanBrief)) {
        return EXECUTE_FAILED_;
    }
    if (!FileSave()) {
        return FAILED_TO_SAVE_FILE_;
    }
    return SUCCESS_;
}

int FullCleanManager::DeleteFullClean(const std::string &full_clean_id) {
    if (!FileOpenRead()) {
        return FAILD_TO_OPEN_FILE_;
    }
    if (!m_full_clean_brief_list.deleteFullCleanBrief(full_clean_id)) {
        return EXECUTE_FAILED_;
    }
    if (!FileSave()) {
        return FAILED_TO_SAVE_FILE_;
    }
    return SUCCESS_;
}

int FullCleanManager::ResetFullCLean(const FullCleanBrief &fullCleanBrief, const std::string &full_clean_id) {
    if (!FileOpenRead()) {
        return FAILD_TO_OPEN_FILE_;
    }
    if (!m_full_clean_brief_list.resetFullCleanBrief(fullCleanBrief, full_clean_id)) {
        cout << "CombinationManager::ResetCombination 更改失败" << endl;
        return EXECUTE_FAILED_;
    }
    if (!FileSave()) {
        return FAILED_TO_SAVE_FILE_;
    }
    return SUCCESS_;
}

int FullCleanManager::GetFullCLeanBrief(FullCleanBrief &fullCleanBrief, const std::string &full_clean_id) {
    if (!FileOpenRead()) {
        return FAILD_TO_OPEN_FILE_;
    }
    if (!m_full_clean_brief_list.getFullCLeanBrief(fullCleanBrief, full_clean_id)) {
        cout << "CombinationManager::getCombination 获取失败" << endl;
        return EXECUTE_FAILED_;
    }
    if (!FileSave()) {
        return FAILED_TO_SAVE_FILE_;
    }
    return SUCCESS_;
}

int FullCleanManager::setMainFullClean(const std::string &full_clean_id) {
    if (!FileOpenRead()) {
        return FAILD_TO_OPEN_FILE_;
    }
    int findResult = false;
    for (const auto &item: m_full_clean_brief_list.getFullCleanBriefList()) {
        if (item.getFullPathID() == full_clean_id) {
            findResult = true;
        }
    }
    if (!findResult) {
        return EXECUTE_FAILED_;
    }
    m_full_clean_brief_list.setMainFullClean(full_clean_id);
    if (!FileSave()) {
        return FAILED_TO_SAVE_FILE_;
    }
    return SUCCESS_;
}

int FullCleanManager::cancelMainFullCLean(const std::string &full_clean_id) {
    if (!FileOpenRead()) {
        return FAILD_TO_OPEN_FILE_;
    }
    int findResult = false;
    for (const auto &item: m_full_clean_brief_list.getFullCleanBriefList()) {
        if (item.getFullPathID() == full_clean_id) {
            findResult = true;
        }
    }
    if (!findResult) {
        return EXECUTE_FAILED_;
    }
    m_full_clean_brief_list.cancelMainFullClean(full_clean_id);
    if (!FileSave()) {
        return FAILED_TO_SAVE_FILE_;
    }
    return SUCCESS_;
}



