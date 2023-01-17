#ifndef C_PTHREAD_POLYGONTASK_H
#define C_PTHREAD_POLYGONTASK_H

#include "string"
#include "WorkStatus.h"

class Polygon_info {
private:
    std::string child_id;
    WorkStatus work_status;
    std::vector<float> points;

public:

    friend void to_json(json &j, const Polygon_info &b) {
        j = json{
                {"child_id",    b.child_id},
                {"work_status", b.work_status},
                {"points",      b.points},
        };
    }

    friend void from_json(const json &j, Polygon_info &b) {
        j.at("child_id").get_to(b.child_id);
        j.at("work_status").get_to(b.work_status);
        j.at("points").get_to(b.points);
    }

    Polygon_info();

    virtual ~Polygon_info();

    const std::string &getChildId() const;

    void setChildId(const std::string &childId);

    const WorkStatus &getWorkStatus() const;

    void setWorkStatus(const WorkStatus &workStatus);

    const std::vector<float> &getPoints() const;

    void setPoints(const std::vector<float> &points);
};

#endif //C_PTHREAD_POLYGONTASK_H