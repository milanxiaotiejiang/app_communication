//
// Created by Looper on 2023/2/28.
//

#ifndef APP_COMMUNICATION_TASK_MODEL_H
#define APP_COMMUNICATION_TASK_MODEL_H

#include <memory>
#include "string"
#include "vector"

struct ZonePo;
struct SubregionPo;

enum class TaskMode {
    Zoned,
    Cover,
    Subregion,
    Line,
};

enum class TaskSource {
    App,
    Pad,
    Cloud,
    Self,
};

struct TaskPo {
    TaskPo();

    TaskPo(long id, const std::string &oMapId, const std::string &name, int rate, TaskMode mode, int sweep, int mop,
           int vacuum, int push, int aromatherapy, int disinfect, bool principal, const std::vector<ZonePo> &zones,
           bool partition, const std::string &subregionRange, bool knife, TaskSource source,
           const std::string &launchPeople, long launchTime, time_t updateTime, time_t createTime);

    long id;
    std::string o_map_id;
    std::string name;
    int rate;
    TaskMode mode;

    int sweep{-1};
    int mop{-1};
    int vacuum{-1};
    int push{-1};
    int aromatherapy{-1};
    int disinfect{-1};

    bool principal{false};

    std::vector<ZonePo> zones;
    bool partition;
    std::string subregion_range;
    bool knife;

    TaskSource source;
    std::string launch_people;
    long launch_time;

    time_t update_time;
    time_t create_time;
};

struct ZonePo {
    ZonePo();

    ZonePo(long id, long oTaskId, const std::string &pointRange);

    long id;
    decltype(TaskPo::id) o_task_id;
    std::string point_range;
};

struct SubregionPo {
    SubregionPo();

    SubregionPo(long id, long oTaskId, long subregionValue);

    long id;
    decltype(TaskPo::id) o_task_id;
    long subregion_value;
};

struct TimerPo {
    TimerPo();

    TimerPo(long id, const std::string &oMapId, long oTaskId, const std::string &oTaskName, const std::string &rule,
            const std::string &name, bool isExecute, int rate, bool isNever, bool isSkip, int endYear, int endMonth,
            int endDay);

    long id;
    std::string o_map_id;
    long o_task_id;
    std::string o_task_name;
    std::string rule;
    std::string name;
    bool is_execute;
    int rate;
    bool is_never;
    bool is_skip;
    int end_year;
    int end_month;
    int end_day;
};

#endif //APP_COMMUNICATION_TASK_MODEL_H
