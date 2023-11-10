//
// Created by Looper on 2023/3/1.
//

#include "model/task.h"

ZoneVo::ZoneVo() {}

ZoneVo::ZoneVo(long zoneId, const std::vector<PointVo> &points) : zoneId(zoneId), points(points) {}

long ZoneVo::getZoneId() const {
    return zoneId;
}

void ZoneVo::setZoneId(long zoneId) {
    ZoneVo::zoneId = zoneId;
}

const std::vector<PointVo> &ZoneVo::getPoints() const {
    return points;
}

void ZoneVo::setPoints(const std::vector<PointVo> &points) {
    ZoneVo::points = points;
}

SubregionVo::SubregionVo() {}

SubregionVo::SubregionVo(long subregionId, long subregionValue) : subregionId(subregionId),
                                                                  subregionValue(subregionValue) {}

long SubregionVo::getSubregionId() const {
    return subregionId;
}

void SubregionVo::setSubregionId(long subregionId) {
    SubregionVo::subregionId = subregionId;
}

long SubregionVo::getSubregionValue() const {
    return subregionValue;
}

void SubregionVo::setSubregionValue(long subregionValue) {
    SubregionVo::subregionValue = subregionValue;
}

TaskVo::TaskVo() {}

TaskVo::TaskVo(long id, const std::string &oMapId, const std::string &name, int rate, int mode, bool principal,
               bool partition, bool knife, const std::string &source, const std::string &launchPeople, long launchTime,
               long updateTime, long createTime, bool rain_snow) : id(id), o_map_id(oMapId), name(name), rate(rate),
                                                                   mode(mode),
                                                                   principal(principal), partition(partition),
                                                                   knife(knife),
                                                                   source(source), launch_people(launchPeople),
                                                                   launch_time(launchTime),
                                                                   update_time(updateTime), create_time(createTime),
                                                                   rain_snow(rain_snow) {}

TaskVo::TaskVo(long id, const std::string &oMapId, const std::string &name, int rate, int mode,
               const WorkStatus &workStatus, bool principal, const std::vector<ZoneVo> &zones,
               bool partition, const std::vector<SubregionVo> &subregions, bool knife, const std::string &source,
               const std::string &launchPeople, long launchTime, long updateTime, long createTime, bool rain_snow)
        : id(id),
          o_map_id(oMapId),
          name(name),
          rate(rate),
          mode(mode),
          workStatus(workStatus),
          principal(principal),
          zones(zones),
          partition(partition),
          subregions(subregions),
          knife(knife),
          source(source),
          launch_people(launchPeople),
          launch_time(launchTime),
          update_time(updateTime),
          create_time(createTime),
          rain_snow(rain_snow) {}

void TaskVo::setId(long id) {
    TaskVo::id = id;
}

void TaskVo::setWorkStatus(const WorkStatus &workStatus) {
    TaskVo::workStatus = workStatus;
}

void TaskVo::setZones(const std::vector<ZoneVo> &zones) {
    TaskVo::zones = zones;
}

void TaskVo::setSubregions(const std::vector<SubregionVo> &subregions) {
    TaskVo::subregions = subregions;
}

void TaskVo::setSource(const std::string &source) {
    TaskVo::source = source;
}

long TaskVo::getId() const {
    return id;
}

const std::string &TaskVo::getOMapId() const {
    return o_map_id;
}

const std::string &TaskVo::getName() const {
    return name;
}

int TaskVo::getRate() const {
    return rate;
}

int TaskVo::getMode() const {
    return mode;
}

const WorkStatus &TaskVo::getWorkStatus() const {
    return workStatus;
}

bool TaskVo::isPrincipal() const {
    return principal;
}

const std::vector<ZoneVo> &TaskVo::getZones() const {
    return zones;
}

bool TaskVo::isPartition() const {
    return partition;
}

const std::vector<SubregionVo> &TaskVo::getSubregions() const {
    return subregions;
}

bool TaskVo::isKnife() const {
    return knife;
}

const std::string &TaskVo::getSource() const {
    return source;
}

const std::string &TaskVo::getLaunchPeople() const {
    return launch_people;
}

long TaskVo::getLaunchTime() const {
    return launch_time;
}

long TaskVo::getUpdateTime() const {
    return update_time;
}

long TaskVo::getCreateTime() const {
    return create_time;
}

bool TaskVo::isRainSnow() const {
    return rain_snow;
}

void TaskVo::setRainSnow(bool rainSnow) {
    rain_snow = rainSnow;
}

std::ostream &operator<<(std::ostream &os, const TaskVo &vo) {
    os << "id: " << vo.id << " o_map_id: " << vo.o_map_id << " name: " << vo.name << " rate: " << vo.rate << " mode: "
       << vo.mode
       << " workStatus: " << vo.workStatus
       << " zones: " << vo.zones.size()
       << " partition: " << vo.partition
       << " subregions: " << vo.subregions.size()
       << " source: " << vo.source << " launch_people: " << vo.launch_people
       << " launch_time: " << vo.launch_time
       << " update_time: " << vo.update_time
       << " create_time: " << vo.create_time;
    return os;
}

TimerVo::TimerVo() {}

TimerVo::TimerVo(long timerId, const std::string &timerRule, long taskId, const std::string &timerName,
                 const std::string &taskName, bool isExecute, int rate, bool isNever, bool isSkip, int endYear,
                 int endMonth, int endDay) : timer_id(timerId), timer_rule(timerRule), task_id(taskId),
                                             timer_name(timerName), task_name(taskName), is_execute(isExecute),
                                             rate(rate), is_never(isNever), is_skip(isSkip), end_year(endYear),
                                             end_month(endMonth), end_day(endDay) {}

long TimerVo::getTimerId() const {
    return timer_id;
}

void TimerVo::setTimerId(long timerId) {
    timer_id = timerId;
}

const std::string &TimerVo::getTimerRule() const {
    return timer_rule;
}

void TimerVo::setTimerRule(const std::string &timerRule) {
    timer_rule = timerRule;
}

long TimerVo::getTaskId() const {
    return task_id;
}

void TimerVo::setTaskId(long taskId) {
    task_id = taskId;
}

const std::string &TimerVo::getTimerName() const {
    return timer_name;
}

void TimerVo::setTimerName(const std::string &timerName) {
    timer_name = timerName;
}

const std::string &TimerVo::getTaskName() const {
    return task_name;
}

void TimerVo::setTaskName(const std::string &taskName) {
    task_name = taskName;
}

bool TimerVo::isExecute() const {
    return is_execute;
}

void TimerVo::setIsExecute(bool isExecute) {
    is_execute = isExecute;
}

int TimerVo::getRate() const {
    return rate;
}

void TimerVo::setRate(int rate) {
    TimerVo::rate = rate;
}

bool TimerVo::isNever() const {
    return is_never;
}

void TimerVo::setIsNever(bool isNever) {
    is_never = isNever;
}

bool TimerVo::isSkip() const {
    return is_skip;
}

void TimerVo::setIsSkip(bool isSkip) {
    is_skip = isSkip;
}

int TimerVo::getEndYear() const {
    return end_year;
}

void TimerVo::setEndYear(int endYear) {
    end_year = endYear;
}

int TimerVo::getEndMonth() const {
    return end_month;
}

void TimerVo::setEndMonth(int endMonth) {
    end_month = endMonth;
}

int TimerVo::getEndDay() const {
    return end_day;
}

void TimerVo::setEndDay(int endDay) {
    end_day = endDay;
}
