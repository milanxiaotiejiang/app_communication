//
// Created by milan on 2021/12/15.
//

#include "model/CleanContinuity.h"

CleanContinuity::~CleanContinuity() {

}

CleanContinuity::CleanContinuity() {}

CleanContinuity::CleanContinuity(std::string continuityId, bool isCustomized) : continuity_id(continuityId),
                                                                                is_customized(isCustomized) {}


const std::string &CleanContinuity::getContinuityId() const {
    return continuity_id;
}

void CleanContinuity::setContinuityId(const std::string &continuityId) {
    continuity_id = continuityId;
}

bool CleanContinuity::isCustomized() const {
    return is_customized;
}

void CleanContinuity::setIsCustomized(bool isCustomized) {
    is_customized = isCustomized;
}

const std::vector<Continuity_info> &CleanContinuity::getContinuityList() const {
    return continuity_list;
}

void CleanContinuity::setContinuityList(const std::vector<Continuity_info> &continuityList) {
    continuity_list = continuityList;
}
