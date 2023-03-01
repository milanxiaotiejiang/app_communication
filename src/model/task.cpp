//
// Created by Looper on 2023/3/1.
//

#include "model/task.h"

TaskVo::TaskVo() {}

TaskVo::TaskVo(long id, const std::string &oMapId, const std::string &name, int rate, int mode,
               const WorkStatus &workStatus, const std::vector<std::vector<PointVo>> &zones, bool partition,
               const std::vector<int> &subregions, const std::string &source, const std::string &launchPeople,
               long launchTime, long updateTime, long createTime) : id(id), o_map_id(oMapId), name(name), rate(rate),
                                                                    mode(mode), workStatus(workStatus), zones(zones),
                                                                    partition(partition), subregions(subregions),
                                                                    source(source), launch_people(launchPeople),
                                                                    launch_time(launchTime), update_time(updateTime),
                                                                    create_time(createTime) {}

TaskVo::TaskVo(long id, const std::string &oMapId, const std::string &name, int rate, int mode, bool partition,
               const std::string &source, const std::string &launchPeople, long launchTime, long updateTime,
               long createTime) : id(id), o_map_id(oMapId), name(name), rate(rate), mode(mode), partition(partition),
                                  source(source), launch_people(launchPeople), launch_time(launchTime),
                                  update_time(updateTime), create_time(createTime) {}

void TaskVo::setId(long id) {
    TaskVo::id = id;
}

void TaskVo::setWorkStatus(const WorkStatus &workStatus) {
    TaskVo::workStatus = workStatus;
}

void TaskVo::setZones(const std::vector<std::vector<PointVo>> &zones) {
    TaskVo::zones = zones;
}

void TaskVo::setSubregions(const std::vector<int> &subregions) {
    TaskVo::subregions = subregions;
}
