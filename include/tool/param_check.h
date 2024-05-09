//
// Created by Looper on 2023/4/3.
//

#ifndef APP_COMMUNICATION_PARAM_CHECK_H
#define APP_COMMUNICATION_PARAM_CHECK_H

#include "model/WorkStatus.h"
#include "model/RoomVo.h"
#include "model/task.h"
#include "db/segmentation_model.h"
#include "db/task_model.h"

void checkWorkStatus(const WorkStatus &workStatus);

void checkName(const std::string &name);

void checkRate(int rate);

void checkMode(int mode);

void checkSource(const std::string &source);

void checkZoned(const ZoneVo &zone);

void checkZoned(const std::vector<ZoneVo> &zones);

void checkSubregion(const SubregionVo &subregion);

void checkSubregion(const std::vector<SubregionVo> &subregions);

void checkSameTimer(const std::string &map_id, const std::string &timer_rule, int selfTimerId);

//void checkMapId(const std::vector<MapPo> &maps, const std::string &map_id);
//
//void checkTaskId(const std::vector<TaskPo> &tasks, long task_id);

#endif //APP_COMMUNICATION_PARAM_CHECK_H
