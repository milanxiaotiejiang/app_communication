/**
 * @file CleanPolygon.cpp
 * @author Quan Li (1204018828@qq.com)
 * @brief  CleanPolygon类
 * @version 0.1
 * @date 2022-02-16
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "CleanPolygon.h"

CleanPolygon::~CleanPolygon() {

}

CleanPolygon::CleanPolygon() {}

CleanPolygon::CleanPolygon(std::string polygonId, bool isCustomized) : polygon_id(polygonId),
                                                                                is_customized(isCustomized) {}


const std::string &CleanPolygon::getPolygonId() const {
    return polygon_id;
}

void CleanPolygon::setPolygonId(const std::string &polygonId) {
    polygon_id = polygonId;
}

bool CleanPolygon::isCustomized() const {
    return is_customized;
}

void CleanPolygon::setIsCustomized(bool isCustomized) {
    is_customized = isCustomized;
}

const std::vector<Polygon_info> &CleanPolygon::getPolygonList() const {
    return polygon_list;
}

void CleanPolygon::setPolygonList(const std::vector<Polygon_info> &polygonList) {
    polygon_list = polygonList;
}
