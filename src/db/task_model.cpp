//
// Created by Looper on 2023/2/28.
//

#include "db/task_model.h"

PointPo::PointPo() {}

PointPo::PointPo(int x, int y) : x(x), y(y) {}

TaskPo::TaskPo() {}

TaskPo::TaskPo(long id, const std::string &oMapId, const std::string &name, int rate, TaskMode mode, int sweep, int mop,
               int vacuum, int push, int aromatherapy, int disinfect, const std::vector<ZonePo> &zones, bool partition,
               const std::string &subregionRange, TaskSource source, const std::string &launchPeople, long launchTime,
               time_t updateTime, time_t createTime) : id(id), o_map_id(oMapId), name(name), rate(rate), mode(mode),
                                                       sweep(sweep), mop(mop), vacuum(vacuum), push(push),
                                                       aromatherapy(aromatherapy), disinfect(disinfect), zones(zones),
                                                       partition(partition), subregion_range(subregionRange),
                                                       source(source), launch_people(launchPeople),
                                                       launch_time(launchTime), update_time(updateTime),
                                                       create_time(createTime) {}

ZonePo::ZonePo() {}

ZonePo::ZonePo(long id, long oTaskId, const std::string &pointRange) : id(id), o_task_id(oTaskId),
                                                                       point_range(pointRange) {}

TimerPo::TimerPo() {}

TimerPo::TimerPo(int id, const std::string &oMapId, long oTaskId, long oTaskName, const std::string &rule,
                 const std::string &name, bool isExecute, int rate, bool isNever, bool isSkip, int endYear,
                 int endMonth, int endDay) : id(id), o_map_id(oMapId), o_task_id(oTaskId), o_task_name(oTaskName),
                                             rule(rule), name(name), is_execute(isExecute), rate(rate),
                                             is_never(isNever), is_skip(isSkip), end_year(endYear), end_month(endMonth),
                                             end_day(endDay) {}
