//
// Created by milan on 2021/12/15.
//

#ifndef C_PTHREAD_CLEANCONTINUITYTASK_H
#define C_PTHREAD_CLEANCONTINUITYTASK_H

#include "model/Continuity_info.h"
#include <nlohmann/json.hpp>

using namespace std;

using json = nlohmann::json;

class CleanContinuity {
private:
    std::string continuity_id;
    bool is_customized;
    std::vector<Continuity_info> continuity_list;

public:
    virtual ~CleanContinuity();

    CleanContinuity();

    CleanContinuity(std::string continuityId, bool isCustomized);

    const std::string &getContinuityId() const;

    void setContinuityId(const std::string &continuityId);


    bool isCustomized() const;

    void setIsCustomized(bool isCustomized);

    const vector<Continuity_info> &getContinuityList() const;

    void setContinuityList(const vector<Continuity_info> &continuityList);

    friend void to_json(json &j, const CleanContinuity &b) {
        j = json{
                {"continuity_id", b.continuity_id},
                {"is_customized", b.is_customized},
                {"continuity_list", b.continuity_list},
        };
    }

    friend void from_json(const json &j, CleanContinuity &b) {
        j.at("continuity_id").get_to(b.continuity_id);
        j.at("is_customized").get_to(b.is_customized);
        j.at("continuity_list").get_to(b.continuity_list);
    }

};


#endif //C_PTHREAD_CLEANCONTINUITYTASK_H
