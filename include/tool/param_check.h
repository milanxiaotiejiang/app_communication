//
// Created by Looper on 2023/4/3.
//

#ifndef APP_COMMUNICATION_PARAM_CHECK_H
#define APP_COMMUNICATION_PARAM_CHECK_H


#include "model/WorkStatus.h"
#include "model/RoomVo.h"

void checkWorkStatus(const WorkStatus &workStatus);

void checkName(const std::string &name);

void checkRate(int rate);

void checkMode(int mode);

void checkSource(const std::string &source);

void checkZoned(const std::vector<std::vector<PointVo>>& zones);

void checkSubregion(const std::vector<int>& subregions);

#endif //APP_COMMUNICATION_PARAM_CHECK_H
