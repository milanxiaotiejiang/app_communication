//
// Created by Looper on 2023/4/23.
//

#ifndef APP_COMMUNICATION_TASK_UTIL_H
#define APP_COMMUNICATION_TASK_UTIL_H

#include "vector"
#include "task/RealPoint.h"

void generateChildPointFlow(const std::vector<RealPoint> &points, std::vector<Cp> &cpList, float resolution_ = 0.2);

#endif //APP_COMMUNICATION_TASK_UTIL_H
