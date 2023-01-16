//
// Created by lijiang on 2022/1/20.
//

#include "model/Continuity_info.h"

Continuity_info::Continuity_info() {}

Continuity_info::~Continuity_info() {

}

const std::string &Continuity_info::getChildId() const {
    return child_id;
}

void Continuity_info::setChildId(const std::string &childId) {
    child_id = childId;
}

const WorkStatus &Continuity_info::getWorkStatus() const {
    return work_status;
}

void Continuity_info::setWorkStatus(const WorkStatus &workStatus) {
    work_status = workStatus;
}

const std::vector<float> &Continuity_info::getPoints() const {
    return points;
}

void Continuity_info::setPoints(const std::vector<float> &points) {
    Continuity_info::points = points;
}
