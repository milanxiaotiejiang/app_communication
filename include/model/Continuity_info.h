//
// Created by lijiang on 2022/1/20.
//

#include "WorkStatus.h"
#include "string"


#ifndef APP_COMMUNICATION_CONTINUITY_INFO_H
#define APP_COMMUNICATION_CONTINUITY_INFO_H


class Continuity_info {
private:
    std::string child_id;
    WorkStatus work_status;
    std::vector<float> points;

public:
    friend void to_json(json &j, const Continuity_info &b) {
        j = json{
                {"child_id",    b.child_id},
                {"work_status", b.work_status},
                {"points",      b.points}};
    }

    friend void from_json(const json &j, Continuity_info &b) {
        j.at("child_id").get_to(b.child_id);
        j.at("work_status").get_to(b.work_status);
        j.at("points").get_to(b.points);
    }

    Continuity_info();

    virtual ~Continuity_info();

    const std::string &getChildId() const;

    void setChildId(const std::string &childId);

    const WorkStatus &getWorkStatus() const;

    void setWorkStatus(const WorkStatus &workStatus);

    const std::vector<float> &getPoints() const;

    void setPoints(const std::vector<float> &points);
};


#endif//APP_COMMUNICATION_CONTINUITY_INFO_H
