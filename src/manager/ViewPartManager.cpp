/**
 * @file ViewPartManager.cpp
 * @author Quan Li (you@domain.com)
 * @brief 用于操控ViewPart
 * @version 0.1
 * @date 2022-03-04
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "manager/ViewPartManager.h"
#include "simulation.h"

bool ViewPartManager::FileOpenRead() {
    if (!sh::File::exists(m_filename)) {
        std::unique_ptr<sh::File> uFilePtr(new sh::File(m_filename));
        if (!uFilePtr->create(m_filename)) {
            std::cout << "create file " << m_filename << " fail !!!";
            return false;
        }
    }

    if (m_file_ptr->open(std::ios::in)) {
        m_file_string = m_file_ptr->readAll();
        m_file_ptr->close();
        if (m_file_string.size() > 0) {
            m_basicJson = json::parse(m_file_string);         //转json
            m_view_part_list = m_basicJson.get<ViewPartList>();//转数据
            file_empty = false;
        } else {
            file_empty = true;
        }
        return true;
    } else {
        std::cout << "ViewPartManager fail to open file" << std::endl;
        return false;
    }
}

bool ViewPartManager::FileSave() {
    m_basicJson = m_view_part_list;//转json
    m_base64Encode.clear();
    m_base64Encode.append(m_basicJson.dump());//转base64
    if (!sh::File::saveTextTo(m_filename, m_base64Encode)) {
        return false;
    }
    return true;
}

int ViewPartManager::AddViewPart(const ViewPart &view_part) {
    if (!FileOpenRead()) {//读取文件
        return FAILD_TO_OPEN_FILE_;
    }
    if (m_view_part_list.SearchName(view_part.getName())) {//添加成功
        return NAME_DUPLICATE_;
    }
    if (!m_view_part_list.AddViewPart(view_part)) {//添加成功
        return EXECUTE_FAILED_;
    }
    if (!FileSave()) {//保存文件
        return FAILED_TO_SAVE_FILE_;
    }
    return SUCCESS_;
}

int ViewPartManager::DelateViewPart(const std::string &part_id) {
    if (!FileOpenRead()) {//读取文件
        return FAILD_TO_OPEN_FILE_;
    }
    if (!m_view_part_list.DelateViewPart(part_id)) {//删除成功
        return EXECUTE_FAILED_;
    }
    if (!FileSave()) {//保存
        return FAILED_TO_SAVE_FILE_;
    }
    return SUCCESS_;
}

int ViewPartManager::ResetViewPart(const ViewPart &view_part, const std::string &part_id) {
    if (!FileOpenRead()) {//读取文件
        return FAILD_TO_OPEN_FILE_;
    }
    if (!m_view_part_list.ResetViewPart(view_part, part_id)) {//修改成功
        return EXECUTE_FAILED_;
    }
    if (!FileSave()) {//保存成功
        return FAILED_TO_SAVE_FILE_;
    }
    return SUCCESS_;
}

int ViewPartManager::GetViewPart(ViewPart &view_part, const std::string &part_id) {
    if (!FileOpenRead()) {//读取文件
        return FAILD_TO_OPEN_FILE_;
    }
    if (!m_view_part_list.GetViewPart(view_part, part_id)) {//修改成功
        return EXECUTE_FAILED_;
    }
    if (!FileSave()) {//保存成功
        return FAILED_TO_SAVE_FILE_;
    }
    return SUCCESS_;
}

bool ViewPartManager::ShowAllViewPart() {
    if (!FileOpenRead()) {//读取文件
        return false;
    }
    m_view_part_list.ShowAllViewPart();//显示成功
    return true;
}

int ViewPartManager::GetViewPartList(ViewPartList &view_part_list) {
    if (!FileOpenRead()) {//读取文件
        return FAILD_TO_OPEN_FILE_;
    }
    view_part_list = m_view_part_list;
    return SUCCESS_;
}