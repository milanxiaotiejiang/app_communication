/**
 * @file Combination.h
 * @author Quan Li (1204018828@qq.com)
 * @brief 单个组合,包括简版和复杂版
 * @version 0.1
 * @date 2022-03-03
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef C_PTHREAD_COMBINATION_H
#define C_PTHREAD_COMBINATION_H

#include "model/ViewPart.h"
#include <nlohmann/json.hpp>

using namespace std;

using json = nlohmann::json;

class CombinationBrief;

class CombinationDetail;

class CombinationBrief {//单个路径组合简表
private:
    std::string m_combination_id;
    std::string m_name;
    int m_rate;
    std::vector<string> m_part_id_list;
    bool principal{false};
    WorkStatus work_status;
    int combination_type; //0--combination;1--full clean

public:
    virtual ~CombinationBrief();

    CombinationBrief();

    CombinationBrief(const CombinationDetail &combination_detail);

    CombinationBrief(std::string combination_id, std::string name, int rate) : m_combination_id(combination_id),
                                                                               m_name(name), m_rate(rate) {}

    const std::string &getCombinationID() const;

    void setCombinationID(const std::string &combination_id);

    const std::string &getName() const;

    void setName(const std::string &name);

    const int getRate() const;

    void setRate(int rate);

    const std::vector<string> getPartIDList() const;

    void setPartIDList(const std::vector<string> &part_id_list);

    bool isPrincipal() const;

    void setPrincipal(bool principal);

    const WorkStatus &getWorkStatus() const;

    void setWorkStatus(const WorkStatus &work_status);

    const int &getCombinationType() const;

    void setCombinationType(int type);

    bool serchPartID(const std::string &part_id);

    bool delatePartID(const std::string &part_id);

    vector<string> toDetail(CombinationDetail &combination_detail, ViewPartList &view_part_list);

    friend void to_json(json &j, const CombinationBrief &b) {
        j = json{
                {"combination_id", b.m_combination_id},
                {"name",           b.m_name},
                {"rate",           b.m_rate},
                {"part_id_list",   b.m_part_id_list},
                {"principal",      b.principal},
                {"work_status",    b.work_status},
                {"combination_type",  b.combination_type}
        };
    }

    friend void from_json(const json &j, CombinationBrief &b) {
        j.at("combination_id").get_to(b.m_combination_id);
        j.at("name").get_to(b.m_name);
        j.at("rate").get_to(b.m_rate);
        j.at("part_id_list").get_to(b.m_part_id_list);
        j.at("principal").get_to(b.principal);
        j.at("work_status").get_to(b.work_status);
        j.at("combination_type").get_to(b.combination_type);
    }
};

class CombinationDetail {//单个路径组合详情表
private:
    std::string m_combination_id;
    std::string m_name;
    int m_rate;
    std::vector<ViewPart> m_view_part_list;
    bool principal{false};
    WorkStatus work_status;
    int combination_type; //0--combination;1--full clean

public:
    virtual ~CombinationDetail();

    CombinationDetail();

    CombinationDetail(const CombinationBrief &combination_brief);

    CombinationDetail(std::string combination_id, std::string name, int rate, std::vector<ViewPart> view_part_list)
            : m_combination_id(combination_id),
              m_name(name), m_rate(rate), m_view_part_list(view_part_list) {}

    const std::string &getCombinationID() const;

    void setCombinationID(const std::string &combination_id);

    const std::string &getName() const;

    void setName(const std::string &name);

    const int getRate() const;

    void setRate(const int rate);

    const std::vector<ViewPart> getViewPartList() const;

    void setViewPartList(const std::vector<ViewPart> &view_part_list);

    bool isPrincipal() const;

    void setPrincipal(bool principal);

    void addViewPart(const ViewPart &view_part);

    const WorkStatus &getWorkStatus() const;

    void setWorkStatus(const WorkStatus &work_status);

    const int &getCombinationType() const;

    void setCombinationType(int type);

    friend void to_json(json &j, const CombinationDetail &b) {
        j = json{
                {"combination_id", b.m_combination_id},
                {"name",           b.m_name},
                {"rate",           b.m_rate},
                {"view_part_list", b.m_view_part_list},
                {"principal",      b.principal},
                {"work_status",    b.work_status},
                {"combination_type", b.combination_type}
        };
    }

    friend void from_json(const json &j, CombinationDetail &b) {
        j.at("combination_id").get_to(b.m_combination_id);
        j.at("name").get_to(b.m_name);
        j.at("rate").get_to(b.m_rate);
        j.at("view_part_list").get_to(b.m_view_part_list);
        j.at("principal").get_to(b.principal);
        j.at("work_status").get_to(b.work_status);
        j.at("combination_type").get_to(b.combination_type);
    }
};

class CombinationBriefList {//简化路径组合列表
private:
    std::vector<CombinationBrief> m_combination_brief_list;

public:
    friend void to_json(json &j, const CombinationBriefList &b) {
        j = json{
                {"m_combination_brief_list", b.m_combination_brief_list},
        };
    }

    friend void from_json(const json &j, CombinationBriefList &b) {
        j.at("m_combination_brief_list").get_to(b.m_combination_brief_list);
    }

    CombinationBriefList();

    ~CombinationBriefList();

    const vector<CombinationBrief> &getCombinationBriefList() const;

    void setCombinationBriefList(const vector<CombinationBrief> &combination_brief_list);

    bool addCombination(const CombinationBrief &combination_brief);

    bool delateCombination(const string &combination_id);

    bool getCombination(CombinationBrief &combination_brief, const string &combination_id);

    bool resetCombination(const CombinationBrief &combination_brief, const string &combination_id);

    bool serchPartID(const std::string &part_id);

    bool delatePartID(const std::string &part_id);

    bool searchPartName(const std::string &part_name);

    void ShowAllCombinationBrief();

    void setMainCombination(const string &combination_id);

    void cancelMainCombination(const string &combination_id);
};

class CombinationDetailList {//简化路径组合列表
private:
    std::vector<CombinationDetail> m_combination_detail_list;

public:
    friend void to_json(json &j, const CombinationDetailList &b) {
        j = json{
                {"m_combination_detail_list", b.m_combination_detail_list},
        };
    }

    friend void from_json(const json &j, CombinationDetailList &b) {
        j.at("m_combination_detail_list").get_to(b.m_combination_detail_list);
    }

    CombinationDetailList();

    ~CombinationDetailList();

    const vector<CombinationDetail> &getCombinationDetailList() const;

    void setCombinationDetailList(const vector<CombinationDetail> &combination_detail_list);

    void ShowAllCombinationDetail();
};


#endif//C_PTHREAD_COMBINATION_H
