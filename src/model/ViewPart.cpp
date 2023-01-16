/**
 * @file ViewPart.cpp
 * @author Quan Li (you@domain.com)
 * @brief 单个Part，用于区域自由组合
 * @version 0.1
 * @date 2022-03-03
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "model/ViewPart.h"

//ViwePart类
////////////////////////////////////////////////////////////////////////////////////////////
const int &ViewPart::getMode() const {
    return m_mode;
}

void ViewPart::setMode(const int &mode) {
    m_mode = mode;
}

const std::string &ViewPart::getName() const {
    return m_name;
}

void ViewPart::setName(const std::string &name) {
    m_name = name;
}

const std::string &ViewPart::getPartID() const {
    return m_part_id;
}

void ViewPart::setPartID(const std::string &part_id) {
    m_part_id = part_id;
}

const std::vector<Point> &ViewPart::getPolygon() const {
    return m_polygon;
}

void ViewPart::setPolygon(const std::vector<Point> &polygon) {
    m_polygon = polygon;
}

const int &ViewPart::getRate() const {
    return m_rate;
}

void ViewPart::setRate(const int &rate) {
    m_rate = rate;
}

const std::vector<Point> &ViewPart::getTeach() const {
    return m_teach;
}

void ViewPart::setTeach(const std::vector<Point> &teach) {
    m_teach = teach;
}

const std::string &ViewPart::getPathID() const {
    return m_path_id;
}

void ViewPart::setPathID(const std::string &path_id) {
    m_path_id = path_id;
}

const WorkStatus &ViewPart::getWorkStatus() const {
    return m_work_status;
}

void ViewPart::setWorkStatus(const WorkStatus &work_status) {
    m_work_status = work_status;
}

const std::vector<Point> &ViewPart::getZoned() const {
    return m_zoned;
}

void ViewPart::setZoned(const std::vector<Point> &zoned) {
    m_zoned = zoned;
}

void ViewPart::showViewPart() {
    std::cout << "Mode:" << m_mode << std::endl;
    std::cout << "Name:" << m_name << std::endl;
    std::cout << "Part_ID:" << m_part_id << std::endl;
    std::cout << "Rate:" << m_rate << std::endl;
    std::cout << "Path_ID:" << m_path_id << std::endl;
    switch (m_mode) {//根据模式不同读取不同的点列
        case 1:
            std::cout << "矩形点列如下" << std::endl;
            ShowPointList(m_zoned);
            break;
        case 2:
            std::cout << "多边形点列如下" << std::endl;
            ShowPointList(m_polygon);
            break;
        case 3:
            std::cout << "示教点列如下" << std::endl;
            ShowPointList(m_teach);
        default:
            std::cout << "错误的ViewPart模式" << std::endl;
            break;
    }
}

void ViewPart::ShowPointList(std::vector<Point> point_list) {
    for (auto &item: point_list) {
        item.show_point();
    }
}
//ViewPartList类
///////////////////////////////////////////////////////////////////////////////////////////////////
ViewPartList::ViewPartList() {}

ViewPartList::~ViewPartList() {}
const std::vector<ViewPart> &ViewPartList::GetViewPartList() const {
    return m_view_part_list;
}

void ViewPartList::SetViewPartList(const std::vector<ViewPart> &view_part_list) {
    m_view_part_list = view_part_list;
}

bool ViewPartList::AddViewPart(const ViewPart &view_part) {
    m_view_part_list.push_back(view_part);
    return true;
}

bool ViewPartList::CheckViewPart(const std::string &part_id) {//查看相应id是否存在
    for (auto &item: m_view_part_list) {
        if (item.getPartID() == part_id)
            return true;
    }
    return false;
}

bool ViewPartList::GetViewPart(ViewPart &view_part, const std::string &part_id) {
    for (auto &item: m_view_part_list) {
        if (item.getPartID() == part_id) {
            view_part = item;
            std::cout << "GetViewPart获取成功" << std::endl;
            return true;
        }
    }
    std::cout << "GetViewPart未找到相应part_id" << std::endl;
    return false;
}


bool ViewPartList::ResetViewPart(const ViewPart &view_part, const std::string &part_id) {
    for (auto &item: m_view_part_list) {
        if (item.getPartID() == part_id) {
            item = view_part;
            std::cout << "ResetViewPart修改成功" << std::endl;
            return true;
        }
    }
    std::cout << "ResetViewPart未找到相应part_id" << std::endl;
    return false;
}
bool ViewPartList::DelateViewPart(const std::string &part_id) {
    std::vector<ViewPart>::iterator iter;
    for (iter = m_view_part_list.begin(); iter != m_view_part_list.end(); iter++) {
        if (iter->getPartID() == part_id) {
            std::cout << "DelateViewPart匹配成功"
                      << "  ID:" << part_id << std::endl;
            if (m_view_part_list.size() <= 1) {
                m_view_part_list.clear();
            } else {
                m_view_part_list.erase(iter);
            }
            std::cout << "删除成功" << std::endl;
            return true;
        }
    }
    std::cout << "DelateViewPart未找到PartID:" << part_id << std::endl;
    return false;
}

bool ViewPartList::SearchName(const std::string &part_name) {
    for (auto &item: m_view_part_list) {
        if (item.getName() == part_name) {
            return true;
        }
    }
    return false;
}
void ViewPartList::ShowAllViewPart() {
    for (auto &item: m_view_part_list) {
        item.showViewPart();
    }
}