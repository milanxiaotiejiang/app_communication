//
// Created by Looper on 2023/3/1.
//

#ifndef APP_COMMUNICATION_TASK_H
#define APP_COMMUNICATION_TASK_H

#include <memory>
#include <ostream>
#include "string"
#include "vector"
#include "RoomVo.h"
#include "WorkStatus.h"

class ZoneVo {
private:
    long zoneId;
    std::vector<PointVo> points;
public:
    ZoneVo();

    ZoneVo(long zoneId, const std::vector<PointVo> &points);

    long getZoneId() const;

    void setZoneId(long zoneId);

    const std::vector<PointVo> &getPoints() const;

    void setPoints(const std::vector<PointVo> &points);

    friend void to_json(json &j, const ZoneVo &vo) {
        j = json{
                {"zoneId", vo.zoneId},
                {"points", vo.points},
        };
    }

    friend void from_json(const json &j, ZoneVo &vo) {
        j.at("zoneId").get_to(vo.zoneId);
        j.at("points").get_to(vo.points);
    }
};

class SubregionVo {
private:
    long subregionId;
    long subregionValue;
public:

    SubregionVo();

    SubregionVo(long subregionId, long subregionValue);

    friend void to_json(json &j, const SubregionVo &vo) {
        j = json{
                {"subregionId",    vo.subregionId},
                {"subregionValue", vo.subregionValue}
        };
    }

    friend void from_json(const json &j, SubregionVo &vo) {
        j.at("subregionId").get_to(vo.subregionId);
        j.at("subregionValue").get_to(vo.subregionValue);
    }

    long getSubregionId() const;

    void setSubregionId(long subregionId);

    long getSubregionValue() const;

    void setSubregionValue(long subregionValue);
};

class TaskVo {

private:
    long id;
    std::string o_map_id;
    std::string name;
    int rate;
    int mode;

    WorkStatus workStatus;

    bool principal{false};

    std::vector<ZoneVo> zones;
    bool partition;
    std::vector<SubregionVo> subregions;
    bool knife;

    std::string source;
    std::string launch_people;
    long launch_time;

    long update_time;
    long create_time;

    bool rain_snow = false;

public:
    TaskVo();

    TaskVo(long id, const std::string &oMapId, const std::string &name, int rate, int mode, bool principal,
           bool partition, bool knife, const std::string &source, const std::string &launchPeople, long launchTime,
           long updateTime, long createTime, bool rain_snow);

    TaskVo(long id, const std::string &oMapId, const std::string &name, int rate, int mode,
           const WorkStatus &workStatus, bool principal, const std::vector<ZoneVo> &zones, bool partition,
           const std::vector<SubregionVo> &subregions, bool knife, const std::string &source,
           const std::string &launchPeople,
           long launchTime, long updateTime, long createTime, bool rain_snow);

    void setId(long id);

    void setWorkStatus(const WorkStatus &workStatus);

    void setZones(const std::vector<ZoneVo> &zones);

    void setSubregions(const std::vector<SubregionVo> &subregions);

    void setSource(const std::string &source);

    long getId() const;

    const std::string &getOMapId() const;

    const std::string &getName() const;

    int getRate() const;

    int getMode() const;

    const WorkStatus &getWorkStatus() const;

    bool isPrincipal() const;

    const std::vector<ZoneVo> &getZones() const;

    bool isPartition() const;

    const std::vector<SubregionVo> &getSubregions() const;

    bool isKnife() const;

    const std::string &getSource() const;

    const std::string &getLaunchPeople() const;

    long getLaunchTime() const;

    long getUpdateTime() const;

    long getCreateTime() const;

    bool isRainSnow() const;

    void setRainSnow(bool rainSnow);

    friend std::ostream &operator<<(std::ostream &os, const TaskVo &vo);

    friend void to_json(json &j, const TaskVo &vo) {
        j = json{
                {"id",            vo.id},
                {"o_map_id",      vo.o_map_id},
                {"name",          vo.name},
                {"rate",          vo.rate},
                {"mode",          vo.mode},
                {"workStatus",    vo.workStatus},
                {"principal",     vo.principal},
                {"zones",         vo.zones},
                {"partition",     vo.partition},
                {"subregions",    vo.subregions},
                {"knife",         vo.knife},
                {"source",        vo.source},
                {"launch_people", vo.launch_people},
                {"launch_time",   vo.launch_time},
                {"partition",     vo.partition},
                {"update_time",   vo.update_time},
                {"create_time",   vo.create_time},
                {"rain_snow",     vo.rain_snow}
        };
    }

    friend void from_json(const json &j, TaskVo &vo) {
        j.at("id").get_to(vo.id);
        j.at("o_map_id").get_to(vo.o_map_id);
        j.at("name").get_to(vo.name);
        j.at("rate").get_to(vo.rate);
        j.at("mode").get_to(vo.mode);
        j.at("workStatus").get_to(vo.workStatus);
        j.at("principal").get_to(vo.principal);
        j.at("zones").get_to(vo.zones);
        j.at("partition").get_to(vo.partition);
        j.at("subregions").get_to(vo.subregions);
        j.at("knife").get_to(vo.knife);
        j.at("source").get_to(vo.source);
        j.at("launch_people").get_to(vo.launch_people);
        j.at("launch_time").get_to(vo.launch_time);
        j.at("update_time").get_to(vo.update_time);
        j.at("create_time").get_to(vo.create_time);
        if (j.contains("rain_snow")) {
            j.at("rain_snow").get_to(vo.rain_snow);
        }
    }

};

class TimerVo {

private:
    long timer_id;
    std::string timer_rule;
    long task_id;
    std::string timer_name;
    std::string task_name;
    bool is_execute{true};//是否开启
    int rate;
    bool is_never;//True 不结束 False 截止日期
    bool is_skip{true};//跳过节假日
    int end_year;//截止日期年
    int end_month;//截止日期月
    int end_day;//截止日期日

public:
    TimerVo();

    TimerVo(long timerId, const std::string &timerRule, long taskId, const std::string &timerName,
            const std::string &taskName, bool isExecute, int rate, bool isNever, bool isSkip, int endYear, int endMonth,
            int endDay);

    long getTimerId() const;

    void setTimerId(long timerId);

    const std::string &getTimerRule() const;

    void setTimerRule(const std::string &timerRule);

    long getTaskId() const;

    void setTaskId(long taskId);

    const std::string &getTimerName() const;

    void setTimerName(const std::string &timerName);

    const std::string &getTaskName() const;

    void setTaskName(const std::string &taskName);

    bool isExecute() const;

    void setIsExecute(bool isExecute);

    int getRate() const;

    void setRate(int rate);

    bool isNever() const;

    void setIsNever(bool isNever);

    bool isSkip() const;

    void setIsSkip(bool isSkip);

    int getEndYear() const;

    void setEndYear(int endYear);

    int getEndMonth() const;

    void setEndMonth(int endMonth);

    int getEndDay() const;

    void setEndDay(int endDay);

    friend void to_json(json &j, const TimerVo &vo) {
        j = json{
                {"timer_id",   vo.timer_id},
                {"timer_rule", vo.timer_rule},
                {"task_id",    vo.task_id},
                {"timer_name", vo.timer_name},
                {"task_name",  vo.task_name},
                {"is_execute", vo.is_execute},
                {"rate",       vo.rate},
                {"is_never",   vo.is_never},
                {"is_skip",    vo.is_skip},
                {"end_year",   vo.end_year},
                {"end_month",  vo.end_month},
                {"end_day",    vo.end_day}
        };
    }

    friend void from_json(const json &j, TimerVo &vo) {
        j.at("timer_id").get_to(vo.timer_id);
        j.at("timer_rule").get_to(vo.timer_rule);
        j.at("task_id").get_to(vo.task_id);
        j.at("timer_name").get_to(vo.timer_name);
        j.at("task_name").get_to(vo.task_name);
        j.at("is_execute").get_to(vo.is_execute);
        j.at("rate").get_to(vo.rate);
        j.at("is_never").get_to(vo.is_never);
        j.at("is_skip").get_to(vo.is_skip);
        j.at("end_year").get_to(vo.end_year);
        j.at("end_month").get_to(vo.end_month);
        j.at("end_day").get_to(vo.end_day);
    }
};

struct ModifyTaskName {
    long id;
    std::string name;

    friend void to_json(json &j, const ModifyTaskName &vo) {
        j = json{
                {"id",   vo.id},
                {"name", vo.name},
        };
    }

    friend void from_json(const json &j, ModifyTaskName &vo) {
        j.at("id").get_to(vo.id);
        j.at("name").get_to(vo.name);
    }
};

struct ModifyTaskRate {
    long id;
    int rate;

    friend void to_json(json &j, const ModifyTaskRate &vo) {
        j = json{
                {"id",   vo.id},
                {"rate", vo.rate},
        };
    }

    friend void from_json(const json &j, ModifyTaskRate &vo) {
        j.at("id").get_to(vo.id);
        j.at("rate").get_to(vo.rate);
    }
};

struct ModifyTaskWorkStatus {
    long id;
    WorkStatus workStatus;

    friend void to_json(json &j, const ModifyTaskWorkStatus &vo) {
        j = json{
                {"id",         vo.id},
                {"workStatus", vo.workStatus},
        };
    }

    friend void from_json(const json &j, ModifyTaskWorkStatus &vo) {
        j.at("id").get_to(vo.id);
        j.at("workStatus").get_to(vo.workStatus);
    }
};

struct ModifyTaskKnife {
    long id;
    bool knife;

    friend void to_json(json &j, const ModifyTaskKnife &vo) {
        j = json{
                {"id",    vo.id},
                {"knife", vo.knife},
        };
    }

    friend void from_json(const json &j, ModifyTaskKnife &vo) {
        j.at("id").get_to(vo.id);
        j.at("knife").get_to(vo.knife);
    }
};

struct ModifyTaskZone {
    long id;
    ZoneVo zone;

    friend void to_json(json &j, const ModifyTaskZone &vo) {
        j = json{
                {"id",   vo.id},
                {"zone", vo.zone},
        };
    }

    friend void from_json(const json &j, ModifyTaskZone &vo) {
        j.at("id").get_to(vo.id);
        j.at("zone").get_to(vo.zone);
    }
};

struct ModifyTaskPartition {
    long id;
    bool partition;

    friend void to_json(json &j, const ModifyTaskPartition &vo) {
        j = json{
                {"id",        vo.id},
                {"partition", vo.partition},
        };
    }

    friend void from_json(const json &j, ModifyTaskPartition &vo) {
        j.at("id").get_to(vo.id);
        j.at("partition").get_to(vo.partition);
    }
};

struct ModifyTaskSubregion {
    long id;
    SubregionVo subregion;

    friend void to_json(json &j, const ModifyTaskSubregion &vo) {
        j = json{
                {"id",        vo.id},
                {"subregion", vo.subregion},
        };
    }

    friend void from_json(const json &j, ModifyTaskSubregion &vo) {
        j.at("id").get_to(vo.id);
        j.at("subregion").get_to(vo.subregion);
    }
};

struct ModifyTimerName {
    long id;
    std::string timer_name;

    friend void to_json(json &j, const ModifyTimerName &vo) {
        j = json{
                {"id",         vo.id},
                {"timer_name", vo.timer_name},
        };
    }

    friend void from_json(const json &j, ModifyTimerName &vo) {
        j.at("id").get_to(vo.id);
        j.at("timer_name").get_to(vo.timer_name);
    }
};

struct OnTask {
    long task_id;
    std::string on_source;
    int on_rate;

    friend void to_json(json &j, const OnTask &vo) {
        j = json{
                {"task_id",   vo.task_id},
                {"on_source", vo.on_source},
                {"on_rate",   vo.on_rate},
        };
    }

    friend void from_json(const json &j, OnTask &vo) {
        j.at("task_id").get_to(vo.task_id);
        j.at("on_source").get_to(vo.on_source);
        j.at("on_rate").get_to(vo.on_rate);
    }

    friend std::ostream &operator<<(std::ostream &os, const OnTask &task) {
        os << "task_id: " << task.task_id << " on_source: " << task.on_source << " on_rate: " << task.on_rate;
        return os;
    }
};

struct RunTask {
    std::string taskId;//运行中的任务ID

    bool renew{false};//新旧任务标志位
    std::string oldTaskId;//旧任务，有值就是CombinationID，没值就是全覆盖
    long newTaskId{0};//新任务，可从数据库查找到的

    RunTask() = default;

    RunTask(const std::string &taskId) : taskId(taskId) {}

    RunTask(const std::string &taskId, bool renew, const std::string &oldTaskId, long newTaskId) : taskId(taskId),
                                                                                                   renew(renew),
                                                                                                   oldTaskId(oldTaskId),
                                                                                                   newTaskId(
                                                                                                           newTaskId) {}

    friend void to_json(json &j, const RunTask &vo) {
        j = json{
                {"task_id",     vo.taskId},
                {"renew",       vo.renew},
                {"old_task_id", vo.oldTaskId},
                {"new_task_id", vo.newTaskId}
        };
    }

    friend void from_json(const json &j, RunTask &vo) {
        j.at("task_id").get_to(vo.taskId);
        j.at("renew").get_to(vo.renew);
        j.at("old_task_id").get_to(vo.oldTaskId);
        j.at("new_task_id").get_to(vo.newTaskId);
    }
};

#endif //APP_COMMUNICATION_TASK_H
