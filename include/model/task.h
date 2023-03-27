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

class TaskVo {

private:
    long id;
    std::string o_map_id;
    std::string name;
    int rate;
    int mode;

    WorkStatus workStatus;

    std::vector<std::vector<PointVo>> zones;
    bool partition;
    std::vector<int> subregions;

    std::string source;
    std::string launch_people;
    long launch_time;

    long update_time;
    long create_time;

public:
    TaskVo();

    TaskVo(long id, const std::string &oMapId, const std::string &name, int rate, int mode,
           const WorkStatus &workStatus, const std::vector<std::vector<PointVo>> &zones, bool partition,
           const std::vector<int> &subregions, const std::string &source, const std::string &launchPeople,
           long launchTime, long updateTime, long createTime);

    TaskVo(long id, const std::string &oMapId, const std::string &name, int rate, int mode, bool partition,
           const std::string &source, const std::string &launchPeople, long launchTime, long updateTime,
           long createTime);

    void setId(long id);

    void setWorkStatus(const WorkStatus &workStatus);

    void setZones(const std::vector<std::vector<PointVo>> &zones);

    void setSubregions(const std::vector<int> &subregions);

    long getId() const;

    const std::string &getOMapId() const;

    const std::string &getName() const;

    int getRate() const;

    int getMode() const;

    const WorkStatus &getWorkStatus() const;

    const std::vector<std::vector<PointVo>> &getZones() const;

    bool isPartition() const;

    const std::vector<int> &getSubregions() const;

    const std::string &getSource() const;

    const std::string &getLaunchPeople() const;

    long getLaunchTime() const;

    long getUpdateTime() const;

    long getCreateTime() const;

    friend std::ostream &operator<<(std::ostream &os, const TaskVo &vo);

    friend void to_json(json &j, const TaskVo &vo) {
        j = json{
                {"id",            vo.id},
                {"o_map_id",      vo.o_map_id},
                {"name",          vo.name},
                {"rate",          vo.rate},
                {"mode",          vo.mode},
                {"workStatus",    vo.workStatus},
                {"zones",         vo.zones},
                {"partition",     vo.partition},
                {"subregions",    vo.subregions},
                {"source",        vo.source},
                {"launch_people", vo.launch_people},
                {"launch_time",   vo.launch_time},
                {"partition",     vo.partition},
                {"update_time",   vo.update_time},
                {"create_time",   vo.create_time}
        };
    }

    friend void from_json(const json &j, TaskVo &vo) {
        j.at("id").get_to(vo.id);
        j.at("o_map_id").get_to(vo.o_map_id);
        j.at("name").get_to(vo.name);
        j.at("rate").get_to(vo.rate);
        j.at("mode").get_to(vo.mode);
        j.at("workStatus").get_to(vo.workStatus);
        j.at("zones").get_to(vo.zones);
        j.at("partition").get_to(vo.partition);
        j.at("subregions").get_to(vo.subregions);
        j.at("source").get_to(vo.source);
        j.at("launch_people").get_to(vo.launch_people);
        j.at("launch_time").get_to(vo.launch_time);
        j.at("update_time").get_to(vo.update_time);
        j.at("create_time").get_to(vo.create_time);
    }
};

class TimerVo {

private:
    long timer_id;
    std::string timer_rule;
    long task_id;
    std::string timer_name;
    std::string task_name;
    bool is_execute{true};
    int rate;
    bool is_never;
    bool is_skip{true};
    int end_year;
    int end_month;
    int end_day;

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

#endif //APP_COMMUNICATION_TASK_H
