//
// Created by admin1 on 22-12-13.
//

#include "model/FullClean.h"

const std::string &FullCleanBrief::getFullPathID() const{
    return m_full_clean_path_id;
}

void FullCleanBrief::setFullPathID(const std::string &full_path_id){
    m_full_clean_path_id = full_path_id;
}

const std::string &FullCleanBrief::getName() const{
    return m_full_clean_name;
}

void FullCleanBrief::setName(const std::string &full_path_name){
    m_full_clean_name = full_path_name;
}

const int FullCleanBrief::getRate() const{
    return m_rate;
}

void FullCleanBrief::setRate(int rate){
    m_rate = rate;
}

const WorkStatus &FullCleanBrief::getWorkStatus() const{
    return  work_status;
}

void FullCleanBrief::setWorkStatus(const WorkStatus &workStatus){
    work_status = workStatus;
}

const vector<Point> &FullCleanBrief::getFullPath() const {
    return full_path;
}

void FullCleanBrief::setFullPath(const vector<Point> &fullPath) {
    full_path = fullPath;
}

bool FullCleanBrief::isPrincipal() const {
    return principal;
}

void FullCleanBrief::setPrincipal(bool principal) {
    FullCleanBrief::principal = principal;
}


FullCleanBriefList::FullCleanBriefList() {}

FullCleanBriefList::~FullCleanBriefList() {}

const vector<FullCleanBrief> &FullCleanBriefList::getFullCleanBriefList() const {
    return  m_fullclean_brief_list;
}

void FullCleanBriefList::setFullCLeanBriefList(const vector<FullCleanBrief> &fullcleanBriefList) {
    m_fullclean_brief_list = fullcleanBriefList;
}

bool FullCleanBriefList::addFullCleanBrief(const FullCleanBrief &fullCleanBrief) {
    m_fullclean_brief_list.push_back(fullCleanBrief);
    return true;
}

bool FullCleanBriefList::deleteFullCleanBrief(const string &fullclean_id) {
    vector<FullCleanBrief>::iterator iter;
    for (iter = m_fullclean_brief_list.begin(); iter != m_fullclean_brief_list.end(); iter++) {
        if (iter->getFullPathID() == fullclean_id) {
            if (m_fullclean_brief_list.size() == 1) {
                m_fullclean_brief_list.clear();
            } else {
                m_fullclean_brief_list.erase(iter);
            }
            return true;
        }
    }
    return false;
}

bool FullCleanBriefList::getFullCLeanBrief(FullCleanBrief &fullCleanBrief,const string &fullclean_id) {
    for (auto &item: m_fullclean_brief_list) {
        if (item.getFullPathID() == fullclean_id) {
            fullCleanBrief = item;
            return true;
        }
    }
    return false;
}

bool FullCleanBriefList::resetFullCleanBrief(const FullCleanBrief &full_clean_brief, const string &fullclean_id) {
    for (auto &item: m_fullclean_brief_list) {
        if (item.getFullPathID() == fullclean_id) {
            item = full_clean_brief;
            return true;
        }
    }
    return false;
}

void FullCleanBriefList::setMainFullClean(const string &fullclean_id) {
    for (auto &item: m_fullclean_brief_list) {
        if (item.getFullPathID() == fullclean_id) {
            item.setPrincipal(true);
        } else {
            item.setPrincipal(false);
        }
    }
}

void FullCleanBriefList::cancelMainFullClean(const string &fullclean_id) {
    for (auto &item: m_fullclean_brief_list) {
        if (item.getFullPathID() == fullclean_id) {
            item.setPrincipal(false);
        }
    }
}



