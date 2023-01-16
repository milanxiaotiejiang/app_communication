/**
 * @file Polygon_info.cpp
 * @author Quan (1204018828@qq.com)
 * @brief  polygon_info类实现
 * @version 0.1
 * @date 2022-02-16
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "model/Polygon_info.h"

Polygon_info::Polygon_info() {}

Polygon_info::~Polygon_info() {

}

const std::string &Polygon_info::getChildId() const {
    return child_id;
}

void Polygon_info::setChildId(const std::string &childId) {
    child_id = childId;
}

const WorkStatus &Polygon_info::getWorkStatus() const {
    return work_status;
}

void Polygon_info::setWorkStatus(const WorkStatus &workStatus) {
    work_status = workStatus;
}

const std::vector<float> &Polygon_info::getPoints() const {
    return points;
}

void Polygon_info::setPoints(const std::vector<float> &points) {
    Polygon_info::points = points;
}
