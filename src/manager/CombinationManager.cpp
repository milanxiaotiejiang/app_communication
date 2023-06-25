/**
 * @file CombinationManager.cpp
 * @author Quan Li (you@domain.com)
 * @brief 组合路径
 * @version 0.1
 * @date 2022-03-06
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "manager/CombinationManager.h"
#include "manager/ViewPartManager.h"

bool CombinationManager::FileOpenRead() {
    if (!sh::File::exists(m_filename)) {
        std::unique_ptr<sh::File> uFilePtr(new sh::File(m_filename));
        if (!uFilePtr->create(m_filename)) {
            return false;
        }
    }

    if (m_file_ptr->open(std::ios::in)) {
        m_file_string = m_file_ptr->readAll();
        m_file_ptr->close();
        if (m_file_string.size() > 0) {
            m_basicJson = json::parse(m_file_string);                         //转json
            m_combination_brief_list = m_basicJson.get<CombinationBriefList>();//转数据
            file_empty = false;
        } else {
            file_empty = true;
        }
        return true;
    } else {
        std::cout << "CombinationManager fail to open file" << std::endl;
        return false;
    }
}

bool CombinationManager::FileSave() {
    if (m_combination_brief_list.getCombinationBriefList().size() > 0) {
        m_basicJson = m_combination_brief_list;//转json
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

int CombinationManager::GetCombinationBriefList(CombinationBriefList &combination_brief_list) {
    if (!FileOpenRead()) {//读取文件
        return FAILD_TO_OPEN_FILE_;
    }
    combination_brief_list = m_combination_brief_list;
    return SUCCESS_;
}

int CombinationManager::AddCombination(const CombinationDetail &combination_detail) {
    CombinationBrief combination_brief_temp(combination_detail);
    if (!FileOpenRead()) {
        return FAILD_TO_OPEN_FILE_;
    }
    if (m_combination_brief_list.searchPartName(combination_brief_temp.getName())) {
        return NAME_DUPLICATE_;
    }
    if (!m_combination_brief_list.addCombination(combination_brief_temp)) {
        return EXECUTE_FAILED_;
    }
    if (!FileSave()) {
        return FAILED_TO_SAVE_FILE_;
    }
    return SUCCESS_;
}

int CombinationManager::DelateCombination(const std::string &combination_id) {
    if (!FileOpenRead()) {
        return FAILD_TO_OPEN_FILE_;
    }
    if (!m_combination_brief_list.delateCombination(combination_id)) {
        std::cout << "CombinationManager::DelateCombination 删除失败" << std::endl;
        return EXECUTE_FAILED_;
    }
    if (!FileSave()) {
        return FAILED_TO_SAVE_FILE_;
    }
    return SUCCESS_;
}

int CombinationManager::DelatePartID(const std::string &part_id) {
    if (!FileOpenRead()) {
        return FAILD_TO_OPEN_FILE_;
    }
    if (!m_combination_brief_list.delatePartID(part_id)) {
        std::cout << "CombinationManager::DelateCombination 删除失败" << std::endl;
        return EXECUTE_FAILED_;
    }
    if (!FileSave()) {
        return FAILED_TO_SAVE_FILE_;
    }
    return SUCCESS_;
}

int CombinationManager::SearchPartID(const std::string &part_id) {
    if (!FileOpenRead()) {
        return FAILD_TO_OPEN_FILE_;
    }
    if (!m_combination_brief_list.serchPartID(part_id)) {
        std::cout << "CombinationManager::DelateCombination 未找到part_id" << part_id << std::endl;
        return EXECUTE_FAILED_;
    }
    if (!FileSave()) {
        return FAILED_TO_SAVE_FILE_;
    }
    return SUCCESS_;
}

int CombinationManager::ResetCombination(const CombinationBrief &combination_brief, const std::string &combination_id) {
    if (!FileOpenRead()) {
        return FAILD_TO_OPEN_FILE_;
    }
    if (!m_combination_brief_list.resetCombination(combination_brief, combination_id)) {
        std::cout << "CombinationManager::ResetCombination 更改失败" << std::endl;
        return EXECUTE_FAILED_;
    }
    if (!FileSave()) {
        return FAILED_TO_SAVE_FILE_;
    }
    return SUCCESS_;
}

int CombinationManager::GetCombination(CombinationBrief &combination_brief, const std::string &combination_id) {
    if (!FileOpenRead()) {
        return FAILD_TO_OPEN_FILE_;
    }
    if (!m_combination_brief_list.getCombination(combination_brief, combination_id)) {
        std::cout << "CombinationManager::getCombination 获取失败" << std::endl;
        return EXECUTE_FAILED_;
    }
    if (!FileSave()) {
        return FAILED_TO_SAVE_FILE_;
    }
    return SUCCESS_;
}

bool CombinationManager::ShowAllCombination() {}

int CombinationManager::setMainCombination(const std::string &combination_id) {
    if (!FileOpenRead()) {
        return FAILD_TO_OPEN_FILE_;
    }
    int findResult = false;
    for (const auto &item: m_combination_brief_list.getCombinationBriefList()) {
        if (item.getCombinationID() == combination_id) {
            findResult = true;
        }
    }
    if (!findResult) {
        return EXECUTE_FAILED_;
    }
    m_combination_brief_list.setMainCombination(combination_id);
    if (!FileSave()) {
        return FAILED_TO_SAVE_FILE_;
    }
    return SUCCESS_;
}


int CombinationManager::cancelMainCombination(const std::string &combination_id) {
    if (!FileOpenRead()) {
        return FAILD_TO_OPEN_FILE_;
    }
    int findResult = false;
    for (const auto &item: m_combination_brief_list.getCombinationBriefList()) {
        if (item.getCombinationID() == combination_id) {
            findResult = true;
        }
    }
    if (!findResult) {
        return EXECUTE_FAILED_;
    }
    m_combination_brief_list.cancelMainCombination(combination_id);
    if (!FileSave()) {
        return FAILED_TO_SAVE_FILE_;
    }
    return SUCCESS_;
}