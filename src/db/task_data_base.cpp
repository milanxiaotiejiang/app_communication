//
// Created by Looper on 2023/2/28.
//

#include "db/task_data_base.h"

#include <utility>

#include <catch2/catch.hpp>

void TaskDataBase::split(const std::string &s, std::vector<std::string> &tokens, char delim) {
    tokens.clear();
    size_t lastPos = s.find_first_not_of(delim, 0);
    size_t pos = s.find(delim, lastPos);
    while (lastPos != std::string::npos) {
        tokens.emplace_back(s.substr(lastPos, pos - lastPos));
        lastPos = s.find_first_not_of(delim, pos);
        pos = s.find(delim, lastPos);
    }
}

TaskVo TaskDataBase::taskPo2Vo(const TaskPo &taskPo) {
    TaskVo task(taskPo.id, taskPo.o_map_id, taskPo.name, taskPo.rate, SqliteDataBase::ModeToInt(taskPo.mode),
                taskPo.principal, taskPo.partition, taskPo.knife, SqliteDataBase::SourceToString(taskPo.source),
                taskPo.launch_people, taskPo.launch_time, taskPo.update_time, taskPo.create_time);

    WorkStatus ws(taskPo.sweep, taskPo.mop, taskPo.vacuum, taskPo.push, taskPo.aromatherapy, taskPo.disinfect);
    task.setWorkStatus(ws);

    if (taskPo.mode == TaskMode::Zoned) {
        std::vector<ZoneVo> zones;
        for (const auto &zone: taskPo.zones) {
            auto range = zone.point_range;
            std::vector<std::string> tokens;
            split(range, tokens, ',');
            if (tokens.size() != 8) {
                break;
            }
            std::vector<PointVo> points;
            for (int i = 0; i < tokens.size(); i = i + 2) {
                auto x = atoi(tokens[i].c_str());
                auto y = atoi(tokens[i + 1].c_str());
                PointVo pointVo(x, y);
                points.push_back(pointVo);
            }

            ZoneVo zoneVo;
            zoneVo.setZoneId(zone.id);
            zoneVo.setPoints(points);
            zones.push_back(zoneVo);
        }
        task.setZones(zones);
    } else if (taskPo.mode == TaskMode::Subregion) {
        std::vector<SubregionVo> subregions;
        // way 1
//        auto range = taskPo.subregion_range;
//        std::vector<std::string> tokens;
//        split(range, tokens, ',');
//        for (const auto &item: tokens) {
//            SubregionVo sub(0, atoi(item.c_str()));
//            subregions.push_back(sub);
//        }
        // way 2
        for (const auto &subregion: taskPo.subregions) {
            SubregionVo sub(subregion.id, subregion.subregion_value);
            subregions.push_back(sub);
        }
        task.setSubregions(subregions);
    }
    return task;
}

TimerVo TaskDataBase::timerPo2Vo(const TimerPo &timerPo) {
    return TimerVo(timerPo.id, timerPo.rule, timerPo.o_task_id, timerPo.name,
                   timerPo.o_task_name, timerPo.is_execute, timerPo.rate, timerPo.is_never, timerPo.is_skip,
                   timerPo.end_year, timerPo.end_month, timerPo.end_day);
}

long TaskDataBase::addTaskVo(const std::string &mapId, const TaskVo &taskVo) {
    TaskMode mode = SqliteDataBase::TaskModeFromInt(taskVo.getMode());

    std::vector<std::string> zoneRanges;
    std::vector<long> subs;
    std::string subregion_range;

    if (mode == TaskMode::Zoned) {
        std::vector<ZoneVo> zones = taskVo.getZones();
        for (const auto &zs: zones) {
            std::string pointRange;
            std::vector<PointVo> points = zs.getPoints();
            for (int i = 0; i < points.size(); i++) {
                auto point = points[i];
                if (i == points.size() - 1) {
                    pointRange.append(std::to_string(point.getX()) + "," + std::to_string(point.getY()));
                } else {
                    pointRange.append(std::to_string(point.getX()) + "," + std::to_string(point.getY()) + ",");
                }
            }
            zoneRanges.push_back(pointRange);
        }
    } else if (mode == TaskMode::Subregion) {
        std::vector<SubregionVo> subregions = taskVo.getSubregions();
        // way 1
        for (int i = 0; i < subregions.size(); i++) {
            auto subregion = subregions[i];
            if (i == subregions.size() - 1) {
                subregion_range.append(std::to_string(subregion.getSubregionValue()));
            } else {
                subregion_range.append(std::to_string(subregion.getSubregionValue()) + ",");
            }
        }
        // way 2
        for (const auto &subregion: subregions) {
            subs.push_back(subregion.getSubregionValue());
        }
    }

    std::vector<ZonePo> z;
    std::vector<SubregionPo> s;
    TaskPo taskPo(0,
                  mapId,
                  taskVo.getName(),
                  taskVo.getRate(),
                  mode,
                  taskVo.getWorkStatus().getSweepStatus(),
                  taskVo.getWorkStatus().getMopStatus(),
                  taskVo.getWorkStatus().getVacuumStatus(),
                  taskVo.getWorkStatus().getPushStatus(),
                  taskVo.getWorkStatus().getAromatherapyStatus(),
                  taskVo.getWorkStatus().getDisinfectStatus(),
                  taskVo.isPrincipal(),
                  z,
                  taskVo.isPartition(),
                  subregion_range,
                  s,
                  taskVo.isKnife(),
                  SqliteDataBase::TaskSourceFromString(taskVo.getSource()),
                  taskVo.getLaunchPeople(),
                  time_t(),
                  std::time(nullptr),
                  std::time(nullptr)
    );

    auto taskId = taskStorage.insert(taskPo);
    for (const auto &item: zoneRanges) {
        ZonePo zonePo(0, taskId, item);
        taskStorage.insert(zonePo);
    }
    for (const auto &item: subs) {
        SubregionPo subregionPo(0, taskId, item);
        taskStorage.insert(subregionPo);
    }
    return taskId;
}

void TaskDataBase::initialize() {
    taskStorage.sync_schema();
}

long TaskDataBase::addTask(const std::string &mapId, const TaskVo &taskVo) {
    return addTaskVo(mapId, taskVo);
}

long TaskDataBase::addTimer(const std::string &mapId, const TimerVo &timer) {
    TimerPo timerPo(0, mapId, timer.getTaskId(), timer.getTaskName(), timer.getTimerRule(), timer.getTimerName(),
                    timer.isExecute(), timer.getRate(), timer.isNever(), timer.isSkip(),
                    timer.getEndYear(), timer.getEndMonth(), timer.getEndDay());
    return taskStorage.insert(timerPo);
}

void TaskDataBase::deleteOwnTask() {
    taskStorage.remove_all<TimerPo>();
    taskStorage.remove_all<ZonePo>();
    taskStorage.remove_all<SubregionPo>();
    taskStorage.remove_all<TaskPo>();
}

void TaskDataBase::deleteTaskFoId(long taskId) {
    deleteTimerForTask(taskId);
    taskStorage.remove_all<ZonePo>(where(c(&ZonePo::o_task_id) == taskId));
    taskStorage.remove_all<SubregionPo>(where(c(&SubregionPo::o_task_id) == taskId));
    taskStorage.remove<TaskPo>(taskId);
}

void TaskDataBase::deleteTaskFoMap(std::string mapId) {
    deleteTimerForMap(mapId);
    auto taskPos = taskStorage.get_all<TaskPo>(where(c(&TaskPo::o_map_id) == std::move(mapId)));
    for (const auto &task: taskPos) {
        deleteTaskFoId(task.id);
    }
}

void TaskDataBase::deleteTaskFoMode(std::string mapId, TaskMode mode) {
    const auto &taskPos = taskStorage.get_all<TaskPo>(
            where(c(&TaskPo::o_map_id) == std::move(mapId) and c(&TaskPo::mode) == static_cast<int>(mode))
    );
    for (const auto &task: taskPos) {
        deleteTaskFoId(task.id);
    }
}

void TaskDataBase::deleteTaskFoMode(std::string mapId, TaskMode mode, bool partition) {
    const auto &taskPos = taskStorage.get_all<TaskPo>(
            where(c(&TaskPo::o_map_id) == std::move(mapId)
                  and c(&TaskPo::mode) == static_cast<int>(mode)
                  and c(&TaskPo::partition) == partition
            )
    );
    for (const auto &task: taskPos) {
        deleteTaskFoId(task.id);
    }
}

void TaskDataBase::deleteTimerForId(long timerId) {
    taskStorage.remove<TimerPo>(timerId);
}

void TaskDataBase::deleteTimerForTask(long taskId) {
    taskStorage.remove_all<TimerPo>(where(c(&TimerPo::o_task_id) == taskId));
}

void TaskDataBase::deleteTimerForMap(std::string mapId) {
    taskStorage.remove_all<TimerPo>(where(c(&TimerPo::o_map_id) == std::move(mapId)));
}

TaskVo TaskDataBase::modifyPrincipalTask(std::string mapId, long taskId, bool principal) {
    if (principal) {
        taskStorage.update_all(sqlite_orm::set(c(&TaskPo::partition) = false),
                               where(c(&TaskPo::o_map_id) == std::move(mapId))
        );
    }
    auto task = taskStorage.get<TaskPo>(taskId);
    task.principal = principal;
    taskStorage.update(task);
    return taskPo2Vo(task);
}

void TaskDataBase::modifyName(long taskId, std::string name) {
    TaskPo task = taskStorage.get<TaskPo>(taskId);
    task.name = std::move(name);
    taskStorage.update(task);
}

void TaskDataBase::modifyRate(long taskId, int rate) {
    TaskPo task = taskStorage.get<TaskPo>(taskId);
    task.rate = rate;
    taskStorage.update(task);
}

void TaskDataBase::modifyWorkStatus(long taskId, const WorkStatus &status) {
    TaskPo task = taskStorage.get<TaskPo>(taskId);
    task.sweep = status.getSweepStatus();
    task.mop = status.getMopStatus();
    task.vacuum = status.getVacuumStatus();
    task.push = status.getPushStatus();
    task.aromatherapy = status.getAromatherapyStatus();
    task.disinfect = status.getDisinfectStatus();
    taskStorage.update(task);
}

void TaskDataBase::modifyKnife(long taskId, bool knife) {
    TaskPo task = taskStorage.get<TaskPo>(taskId);
    if (task.mode == TaskMode::Line) {
        task.knife = knife;
        taskStorage.update(task);
    }
}

long TaskDataBase::operateAddZone(long taskId, const ZoneVo &zone) {
    TaskPo task = taskStorage.get<TaskPo>(taskId);
    if (task.mode == TaskMode::Zoned) {
        std::string pointRange;
        std::vector<PointVo> points = zone.getPoints();
        for (int i = 0; i < points.size(); i++) {
            auto point = points[i];
            if (i == points.size() - 1) {
                pointRange.append(std::to_string(point.getX()) + "," + std::to_string(point.getY()));
            } else {
                pointRange.append(std::to_string(point.getX()) + "," + std::to_string(point.getY()) + ",");
            }
        }

        ZonePo zonePo(0, taskId, pointRange);
        auto zoneId = taskStorage.insert(zonePo);
        return zoneId;
    }
    return -1;
}

void TaskDataBase::operateDeleteZone(long taskId, const ZoneVo &zone) {
    TaskPo task = taskStorage.get<TaskPo>(taskId);
    if (task.mode == TaskMode::Zoned) {
        taskStorage.remove<ZonePo>(zone.getZoneId());
    }
}

void TaskDataBase::operateModifyZone(long taskId, const ZoneVo &zone) {
    TaskPo task = taskStorage.get<TaskPo>(taskId);
    if (task.mode == TaskMode::Zoned) {
        ZonePo zonePo = taskStorage.get<ZonePo>(zone.getZoneId());

        std::string pointRange;
        std::vector<PointVo> points = zone.getPoints();
        for (int i = 0; i < points.size(); i++) {
            auto point = points[i];
            if (i == points.size() - 1) {
                pointRange.append(std::to_string(point.getX()) + "," + std::to_string(point.getY()));
            } else {
                pointRange.append(std::to_string(point.getX()) + "," + std::to_string(point.getY()) + ",");
            }
        }
        zonePo.point_range = pointRange;
        taskStorage.update(zonePo);
    }
}

void TaskDataBase::modifyPartition(long taskId, bool partition) {
    TaskPo task = taskStorage.get<TaskPo>(taskId);
    if (task.mode == TaskMode::Cover) {
        task.partition = partition;
        taskStorage.update(task);
    }
}

long TaskDataBase::operateAddSubregion(long taskId, const SubregionVo &subregion) {
    TaskPo task = taskStorage.get<TaskPo>(taskId);
    if (task.mode == TaskMode::Subregion) {
        SubregionPo subregionPo(0, taskId, subregion.getSubregionValue());
        long subregionId = taskStorage.insert(subregionPo);
        return subregionId;
    }
    return -1;
}

void TaskDataBase::operateDeleteSubregion(long taskId, const SubregionVo &subregion) {
    TaskPo task = taskStorage.get<TaskPo>(taskId);
    if (task.mode == TaskMode::Subregion) {
        taskStorage.remove<SubregionPo>(subregion.getSubregionId());
    }
}

void TaskDataBase::modifyTimerName(long timerId, std::string name) {
    TimerPo timer = taskStorage.get<TimerPo>(timerId);
    timer.name = std::move(name);
    taskStorage.update(timer);
}

void TaskDataBase::modifyTimer(const string &mapId, const TimerVo &timer) {
    TimerPo timerPo(timer.getTimerId(), mapId, timer.getTaskId(), timer.getTaskName(),
                    timer.getTimerRule(), timer.getTimerName(),
                    timer.isExecute(), timer.getRate(), timer.isNever(), timer.isSkip(),
                    timer.getEndYear(), timer.getEndMonth(), timer.getEndDay());
    taskStorage.update(timerPo);
}

std::vector<TaskVo> TaskDataBase::loadTaskFoMap(std::string mapId) {
    std::vector<TaskVo> tasks;

    auto taskPos = taskStorage.get_all<TaskPo>(where(c(&TaskPo::o_map_id) == std::move(mapId)));
    if (taskPos.empty()) {
        return tasks;
    }

    for (auto &t: taskPos) {
        auto zs = taskStorage.get_all<ZonePo>(where(c(&ZonePo::o_task_id) == t.id));
        for (const auto &z: zs) {
            t.zones.push_back(z);
        }
    }

    for (auto &t: taskPos) {
        tasks.push_back(taskPo2Vo(t));
    }
}

TaskVo TaskDataBase::loadTaskFoId(long taskId) {
    auto taskPo = taskStorage.get<TaskPo>(taskId);
    auto zs = taskStorage.get_all<ZonePo>(where(c(&ZonePo::o_task_id) == taskPo.id));
    for (const auto &z: zs) {
        taskPo.zones.push_back(z);
    }
    return taskPo2Vo(taskPo);
}

TaskVo TaskDataBase::loadPrincipalTask(const std::string &mapId) {
    TaskVo taskVo;
    taskVo.setId(-1);

    auto taskPos = taskStorage.get_all<TaskPo>(
            where(
                    c(&TaskPo::o_map_id) == mapId
                    and c(&TaskPo::principal) == true
            )
    );
    if (taskPos.empty()) {
        return taskVo;
    }
    if (taskPos.size() != 1) {
        taskStorage.update_all(sqlite_orm::set(c(&TaskPo::partition) = false),
                               where(c(&TaskPo::o_map_id) == mapId)
        );
        return taskVo;
    }
    auto principalTask = taskPos[0];
    return taskPo2Vo(principalTask);
}

std::vector<TimerVo> TaskDataBase::loadTimerFoMap(const std::string &mapId) {
    auto results = taskStorage.select(
            distinct(columns(&TimerPo::id,
                             &TimerPo::o_map_id,
                             &TimerPo::o_task_id,
                             &TaskPo::name,
                             &TimerPo::rule,
                             &TimerPo::name,
                             &TimerPo::is_execute,
                             &TimerPo::rate,
                             &TimerPo::is_never,
                             &TimerPo::is_skip,
                             &TimerPo::end_year,
                             &TimerPo::end_month,
                             &TimerPo::end_day
            )),
            inner_join<TaskPo>(on(c(&TaskPo::id) == &TimerPo::o_task_id)),
            where(c(&TimerPo::o_map_id) == mapId)
    );

    std::vector<TimerVo> vos;
    for (const auto &row: results) {
        TimerPo timerPo(
                std::get<0>(row),
                std::get<1>(row),
                std::get<2>(row),
                std::get<3>(row),
                std::get<4>(row),
                std::get<5>(row),
                std::get<6>(row),
                std::get<7>(row),
                std::get<8>(row),
                std::get<9>(row),
                std::get<10>(row),
                std::get<11>(row),
                std::get<12>(row)
        );
        const TimerVo &vo = timerPo2Vo(timerPo);
        vos.push_back(vo);
    }
    return vos;
}

TimerVo TaskDataBase::loadTimerFoId(long timerId) {
    auto timerPo = taskStorage.get<TimerPo>(timerId);
    return timerPo2Vo(timerPo);
}
