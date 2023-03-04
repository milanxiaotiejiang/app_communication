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
};

class TimerVo {
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

};


#endif //APP_COMMUNICATION_TASK_H
