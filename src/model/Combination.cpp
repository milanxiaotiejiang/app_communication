/**
 * @file Combination.cpp
 * @author Quan Li (1204018828@qq.com)
 * @brief   路径组合类
 * @version 0.1
 * @date 2022-03-03
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "model/Combination.h"
#include "model/ViewPart.h"
//CombinationBrief类
////////////////////////////////////////////////////////////////////////////////////////////////////////

CombinationBrief::~CombinationBrief() {
}

CombinationBrief::CombinationBrief() {
}

CombinationBrief::CombinationBrief(const CombinationDetail &combination_detail) {
    m_combination_id = combination_detail.getCombinationID();
    m_name = combination_detail.getName();
    m_rate = combination_detail.getRate();
    principal = combination_detail.isPrincipal();
    for (auto &item: combination_detail.getViewPartList()) {
        m_part_id_list.push_back(item.getPartID());
    }
    work_status = combination_detail.getWorkStatus();
    combination_type = combination_detail.getCombinationType();
}

const std::string &CombinationBrief::getCombinationID() const {
    return m_combination_id;
}

void CombinationBrief::setCombinationID(const std::string &combination_id) {
    m_combination_id = combination_id;
}

const std::string &CombinationBrief::getName() const {
    return m_name;
}

void CombinationBrief::setName(const std::string &name) {
    m_name = name;
}

const int CombinationBrief::getRate() const {
    return m_rate;
}

void CombinationBrief::setRate(int rate) {
    m_rate = rate;
}

const std::vector<std::string> CombinationBrief::getPartIDList() const {
    return m_part_id_list;
}

void CombinationBrief::setPartIDList(const std::vector<std::string> &part_id_list) {
    m_part_id_list = part_id_list;
}

bool CombinationBrief::serchPartID(const std::string &part_id) {
    for (auto &item: m_part_id_list) {
        if (part_id == item) {
            return true;
        }
    }
    return false;
}

bool CombinationBrief::isPrincipal() const {
    return principal;
}

void CombinationBrief::setPrincipal(bool principal) {
    CombinationBrief::principal = principal;
}

const WorkStatus &CombinationBrief::getWorkStatus() const {
    return work_status;
}

void CombinationBrief::setWorkStatus(const WorkStatus &work_status) {
    CombinationBrief::work_status = work_status;
}

const int &CombinationBrief::getCombinationType() const {
    return combination_type;
}

void CombinationBrief::setCombinationType(int type) {
    combination_type = type;
}

bool CombinationBrief::delatePartID(const std::string &part_id) {
    std::vector<std::string>::iterator iter;
    bool mached = false;
    for (iter = m_part_id_list.begin(); iter != m_part_id_list.end();) {
        if (*iter == part_id) {
            if (m_part_id_list.size() <= 1) {
                m_part_id_list.clear();
            } else {
                m_part_id_list.erase(iter);
            }
            mached = true;
            break;
        } else {
            iter++;
        }
    }
    return mached;
}

std::vector<std::string> CombinationBrief::toDetail(CombinationDetail &combination_detail, ViewPartList &view_part_list) {
    std::vector<std::string> part_not_mached;
    CombinationDetail combination_detail_temp(*this);
    std::vector<ViewPart> view_part_list_temp = view_part_list.GetViewPartList();
    for (auto &combination_part_item: m_part_id_list) {
        bool mached = false;
        for (auto &view_part_item: view_part_list_temp) {
            if (combination_part_item == view_part_item.getPartID()) {
                combination_detail_temp.addViewPart(view_part_item);
                mached = true;
                break;
            }
        }
        if (!mached) {
            part_not_mached.push_back(combination_part_item);
            std::cout << "can't mach part_id:" << combination_part_item << std::endl;
        }
    }
    combination_detail = combination_detail_temp;
    return part_not_mached;
}

//CombinationDetail类
///////////////////////////////////////////////////////////////////////////////////////////////////
CombinationDetail::~CombinationDetail() {
}

CombinationDetail::CombinationDetail() {
}

CombinationDetail::CombinationDetail(const CombinationBrief &combination_brief) {
    m_combination_id = combination_brief.getCombinationID();
    m_name = combination_brief.getName();
    m_rate = combination_brief.getRate();
    principal = combination_brief.isPrincipal();
    work_status = combination_brief.getWorkStatus();
    combination_type = combination_brief.getCombinationType();
}

const std::string &CombinationDetail::getCombinationID() const {
    return m_combination_id;
}

void CombinationDetail::setCombinationID(const std::string &combination_id) {
    m_combination_id = combination_id;
}

const std::string &CombinationDetail::getName() const {
    return m_name;
}

void CombinationDetail::setName(const std::string &name) {
    m_name = name;
}

const int CombinationDetail::getRate() const {
    return m_rate;
}

void CombinationDetail::setRate(const int rate) {
    m_rate = rate;
}

const std::vector<ViewPart> CombinationDetail::getViewPartList() const {
    return m_view_part_list;
}

void CombinationDetail::setViewPartList(const std::vector<ViewPart> &view_part_list) {
    m_view_part_list = view_part_list;
}

bool CombinationDetail::isPrincipal() const {
    return principal;
}

void CombinationDetail::setPrincipal(bool principal) {
    CombinationDetail::principal = principal;
}

const int &CombinationDetail::getCombinationType() const {
    return combination_type;
}

void CombinationDetail::setCombinationType(int type) {
    combination_type = type;
}

const WorkStatus &CombinationDetail::getWorkStatus() const {
    return work_status;
}

void CombinationDetail::setWorkStatus(const WorkStatus &work_status) {
    CombinationDetail::work_status = work_status;
}

void CombinationDetail::addViewPart(const ViewPart &view_part) {
    m_view_part_list.push_back(view_part);
}

//CombinationBriefList类
/////////////////////////////////////////////////////////////////////////////////////////////////
CombinationBriefList::CombinationBriefList() {}

CombinationBriefList::~CombinationBriefList() {}

const std::vector<CombinationBrief> &CombinationBriefList::getCombinationBriefList() const {
    return m_combination_brief_list;
}

void CombinationBriefList::setCombinationBriefList(const std::vector<CombinationBrief> &combination_brief_list) {
    m_combination_brief_list = combination_brief_list;
}

bool CombinationBriefList::addCombination(const CombinationBrief &combination_brief) {
    m_combination_brief_list.push_back(combination_brief);
    return true;
}

bool CombinationBriefList::delateCombination(const std::string &combination_id) {
    std::vector<CombinationBrief>::iterator iter;
    for (iter = m_combination_brief_list.begin(); iter != m_combination_brief_list.end(); iter++) {

        if (iter->getCombinationID() == combination_id) {
            if (m_combination_brief_list.size() == 1) {
                m_combination_brief_list.clear();
            } else {
                m_combination_brief_list.erase(iter);
            }
            return true;
        }
    }
    std::cout << "CombinationBriefList::delateCombination匹配失败" << std::endl;
    return false;
}

bool CombinationBriefList::getCombination(CombinationBrief &combination_brief, const std::string &combination_id) {
    for (auto &item: m_combination_brief_list) {
        if (item.getCombinationID() == combination_id) {
            combination_brief = item;
            return true;
        }
    }
    std::cout << "CombinationBriefList::getCombination匹配失败" << std::endl;
    return false;
}

bool CombinationBriefList::resetCombination(const CombinationBrief &combination_brief, const std::string &combination_id) {
    for (auto &item: m_combination_brief_list) {
        if (item.getCombinationID() == combination_id) {
            item = combination_brief;
            return true;
        }
    }
    std::cout << "CombinationBriefList::resetCombination匹配失败" << std::endl;
    return false;
}

bool CombinationBriefList::serchPartID(const std::string &part_id) {
    for (auto &item: m_combination_brief_list) {
        if (item.serchPartID(part_id)) {
            return true;
        }
    }
    return false;
}

bool CombinationBriefList::delatePartID(const std::string &part_id) {
    bool mached = false;
    std::vector<CombinationBrief>::iterator iter;
    for (iter = m_combination_brief_list.begin(); iter != m_combination_brief_list.end();) {
        if (iter->delatePartID(part_id)) {
            mached = true;
            if (iter->getPartIDList().size() == 0) {
                if (m_combination_brief_list.size() <= 1) {
                    m_combination_brief_list.clear();
                } else {
                    m_combination_brief_list.erase(iter);
                }
            } else {
                iter++;
            }
        } else {
            iter++;
        }
    }
    return mached;
}

bool CombinationBriefList::searchPartName(const std::string &part_name) {
    for (auto &item: m_combination_brief_list) {
        if (item.getName() == part_name) {
            return true;
        }
    }
    return false;
}

void CombinationBriefList::ShowAllCombinationBrief() {}

void CombinationBriefList::setMainCombination(const std::string &combination_id) {
    for (auto &item: m_combination_brief_list) {
        if (item.getCombinationID() == combination_id) {
            item.setPrincipal(true);
        } else {
            item.setPrincipal(false);
        }
    }
}

void CombinationBriefList::cancelMainCombination(const std::string &combination_id) {
    for (auto &item: m_combination_brief_list) {
        if (item.getCombinationID() == combination_id) {
            item.setPrincipal(false);
        }
    }
}

//CombinationDetailList类
/////////////////////////////////////////////////////////////////////////////////////////////////
CombinationDetailList::CombinationDetailList() {}

CombinationDetailList::~CombinationDetailList() {}

const std::vector<CombinationDetail> &CombinationDetailList::getCombinationDetailList() const {
    return m_combination_detail_list;
}

void CombinationDetailList::setCombinationDetailList(const std::vector<CombinationDetail> &combination_detail_list) {
    m_combination_detail_list = combination_detail_list;
}

void CombinationDetailList::ShowAllCombinationDetail() {}