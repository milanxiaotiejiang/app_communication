//
// Created by admin1 on 22-12-13.
//

#ifndef APP_COMMUNICATION_FULLCLEAN_H
#define APP_COMMUNICATION_FULLCLEAN_H
#include "vector"
#include "Point.h"
#include "WorkStatus.h"

using namespace std;

class FullCleanBrief {
private:
    std::string m_full_clean_path_id;
    std::string m_full_clean_name;
    int m_rate;
    vector<Point> full_path;
    WorkStatus work_status;
    std::vector<string> m_part_id_list;

    bool principal{false};

public:
    virtual ~FullCleanBrief() {}
    FullCleanBrief() {}

    FullCleanBrief(std::string full_path_id, std::string name, int rate, const vector<Point> &fullPath, const WorkStatus &workStatus) : m_full_clean_path_id(full_path_id),
                                                                                                                                        m_full_clean_name(name),
                                                                                                                                        m_rate(rate),
                                                                                                                                        full_path(fullPath),
                                                                                                                                        work_status(workStatus){}


    const std::string &getFullPathID() const;

    void setFullPathID(const std::string &full_path_id);

    const std::string &getName() const;

    void setName(const std::string &full_path_name);

    const int getRate() const;

    void setRate(int rate);

    const WorkStatus &getWorkStatus() const;

    void setWorkStatus(const WorkStatus &workStatus);

    const vector<Point> &getFullPath() const;

    void setFullPath(const vector<Point> &fullPath);

    bool isPrincipal() const;

    void setPrincipal(bool principal);

    friend void to_json(json &j, const FullCleanBrief &b) {
        j = json{
                {"full_path_id", b.m_full_clean_path_id},
                {"name", b.m_full_clean_name},
                {"rate", b.m_rate},
                {"work_status", b.work_status},
                {"full_path", b.full_path}
        };
    }

    friend void from_json(const json &j, FullCleanBrief &b) {
        j.at("full_path_id").get_to(b.m_full_clean_path_id);
        j.at("name").get_to(b.m_full_clean_name);
        j.at("rate").get_to(b.m_rate);
        j.at("work_status").get_to(b.work_status);
        j.at("full_path").get_to(b.full_path);
    }
};

class FullCleanBriefList{
private:
    std::vector<FullCleanBrief> m_fullclean_brief_list;
public:
    friend void to_json(json &j, const FullCleanBriefList &b){
        j = json {
            {"m_full_clean_brief_list", b.m_fullclean_brief_list},
        };
    }

    friend void from_json(const json &j, FullCleanBriefList &b){
        j.at("m_full_clean_brief_list").get_to(b.m_fullclean_brief_list);
    }

    FullCleanBriefList();
    ~FullCleanBriefList();

    const vector<FullCleanBrief> &getFullCleanBriefList() const;
    void setFullCLeanBriefList(const vector<FullCleanBrief> &fullclean_brief_list);

    bool addFullCleanBrief(const FullCleanBrief &fullCleanBrief);
    bool deleteFullCleanBrief(const string &fullclean_id);
    bool getFullCLeanBrief(FullCleanBrief &fullcleanBrief, const string &fullclean_id);
    bool resetFullCleanBrief(const FullCleanBrief &full_clean_brief, const string &fullclean_id);
    void setMainFullClean(const string &fullclean_id);
    void cancelMainFullClean(const string &fullclean_id);

};


#endif //APP_COMMUNICATION_FULLCLEAN_H
