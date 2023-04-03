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

    if (taskPo.mode == TaskMode::Zoned) {
        std::vector<std::vector<PointVo>> zones;
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
            zones.push_back(points);
        }
        task.setZones(zones);
    } else if (taskPo.mode == TaskMode::Subregion) {
        std::vector<int> subregions;
        auto range = taskPo.subregion_range;
        std::vector<std::string> tokens;
        split(range, tokens, ',');
        for (const auto &item: tokens) {
            subregions.push_back(atoi(item.c_str()));
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
    std::string subregion_range;
    if (mode == TaskMode::Zoned) {
        std::vector<std::vector<PointVo>> zones = taskVo.getZones();
        for (const auto &zs: zones) {
            std::string pointRange;
            for (int i = 0; i < zs.size(); i++) {
                auto point = zs[i];
                if (i == zs.size() - 1) {
                    pointRange.append(std::to_string(point.getX()) + "," + std::to_string(point.getY()));
                } else {
                    pointRange.append(std::to_string(point.getX()) + "," + std::to_string(point.getY()) + ",");
                }
            }
            zoneRanges.push_back(pointRange);
        }
    } else if (mode == TaskMode::Subregion) {
        std::vector<int> subregions = taskVo.getSubregions();
        for (int i = 0; i < subregions.size(); i++) {
            auto value = subregions[i];
            if (i == subregions.size() - 1) {
                subregion_range.append(std::to_string(value));
            } else {
                subregion_range.append(std::to_string(value) + ",");
            }
        }
    }

    std::vector<ZonePo> v;
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
                  v,
                  taskVo.isPartition(),
                  subregion_range,
                  taskVo.isKnife(),
                  SqliteDataBase::TaskSourceFromString(taskVo.getSource()),
                  taskVo.getLaunchPeople(),
                  time_t(),
                  std::time(nullptr),
                  std::time(nullptr)
    );

//    taskStorage.transaction([this, taskPo, zoneRanges] {
//        auto taskId = taskStorage.insert(taskPo);
//        for (const auto &item: zoneRanges) {
//            const ZonePo &zonePo = ZonePo(0, taskId, item);
//            taskStorage.insert(zonePo);
//        }
//        return true;
//    });
    auto taskId = taskStorage.insert(taskPo);
    for (const auto &item: zoneRanges) {
        const ZonePo &zonePo = ZonePo(0, taskId, item);
        taskStorage.insert(zonePo);
    }
    return taskId;
}

//TEST_CASE() {
//    TaskDataBase::instance().initialize();
//
//    TaskDataBase::instance().deleteOwnTask();
//
//    std::string map_id = "888";
//    TaskVo task1(0, map_id, "task test 1", 1, 0, false, "App", "looper", 0l, 0l, 0l);
//    TaskVo task2(0, map_id, "task test 2", 1, 0, false, "App", "looper", 0l, 0l, 0l);
//    TaskVo task3(0, map_id, "task test 3", 1, 0, false, "App", "looper", 0l, 0l, 0l);
//
//    std::vector<std::vector<PointVo>> zones;
//    for (int i = 0; i < rand() % 5; i++) {
//        std::vector<PointVo> points;
//        for (int j = 0; j < 4; j++) {
//            points.emplace_back(rand() % 10, rand() % 10);
//        }
//        zones.push_back(points);
//    }
//    task1.setZones(zones);
//    task2.setZones(zones);
//    task3.setZones(zones);
//
//    TaskDataBase::instance().addZoneTask(map_id, task1);
//    TaskDataBase::instance().addZoneTask(map_id, task2);
//    TaskDataBase::instance().addZoneTask(map_id, task3);
//
//    const std::vector<TaskVo> &vector = TaskDataBase::instance().loadTaskFoMap(map_id);
//    std::cout << "loadTaskFoMap ------------------------------------------------------" << std::endl;
//    for (const auto &item: vector) {
//        std::cout << item << std::endl;
//    }
//
//    if (!vector.empty()) {
//        auto task = vector[0];
//        const TaskVo &vo = TaskDataBase::instance().loadTaskFoId(task.getId());
//        std::cout << "loadTaskFoTask ------------------------------------------------------" << std::endl;
//        std::cout << vo << std::endl;
//
//
//        TimerVo timer1(0, "1357", 0, "timer test 1", "", false, 1, false, false, 0, 0, 0);
//        TaskDataBase::instance().addTimer(map_id, task.getId(), timer1);
//
//        TimerVo timer2(0, "1357", 0, "timer test 2", "", false, 1, false, false, 0, 0, 0);
//        TaskDataBase::instance().addTimer(map_id, task.getId(), timer2);
//
//        const std::vector<TimerVo> &vos = TaskDataBase::instance().loadTimerFoMap(map_id);
//
////        TimerVo timerVo = vos[0];
////        TaskDataBase::instance().deleteTimerForId(timerVo.getTimerId());
////        const std::vector<TimerVo> &vos1 = TaskDataBase::instance().loadTimerFoMap(map_id);
//
////        TaskDataBase::instance().deleteTaskFoId(task.getId());
////        TaskDataBase::instance().deleteTimerForMap(map_id);
//
//        const std::vector<TimerVo> &vos2 = TaskDataBase::instance().loadTimerFoMap(map_id);
//    }
//    const std::vector<TaskVo> &vector2 = TaskDataBase::instance().loadTaskFoMap(map_id);
//    std::cout << "deleteTaskFoId ------------------------------------------------------" << std::endl;
//    for (const auto &item: vector2) {
//        std::cout << item << std::endl;
//    }
//
//    TaskDataBase::instance().deleteTaskFoMap(map_id);
//    const std::vector<TaskVo> &vector3 = TaskDataBase::instance().loadTaskFoMap(map_id);
//    std::cout << "deleteTaskFoMap ------------------------------------------------------" << std::endl;
//    for (const auto &item: vector3) {
//        std::cout << item << std::endl;
//    }
//}

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
    taskStorage.remove_all<TaskPo>();
}

void TaskDataBase::deleteTaskFoId(long taskId) {
    deleteTimerForTask(taskId);
    taskStorage.remove_all<ZonePo>(where(c(&ZonePo::o_task_id) == taskId));
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
    task.knife = knife;
    taskStorage.update(task);
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

TaskVo TaskDataBase::loadPrincipalTask(std::string mapId) {
    TaskVo taskVo;
    taskVo.setId(-1);

    auto taskPos = taskStorage.get_all<TaskPo>(
            where(
                    c(&TaskPo::o_map_id) == std::move(mapId)
                    and c(&TaskPo::principal) == true
            )
    );
    if (taskPos.empty()) {
        return taskVo;
    }
    if (taskPos.size() != 1) {
        taskStorage.update_all(sqlite_orm::set(c(&TaskPo::partition) = false),
                               where(c(&TaskPo::o_map_id) == std::move(mapId))
        );
        return taskVo;
    }
    auto principalTask = taskPos[0];
    return taskPo2Vo(principalTask);
}

std::vector<TimerVo> TaskDataBase::loadTimerFoMap(std::string mapId) {
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
            inner_join<ZonePo>(on(c(&TaskPo::id) == &TimerPo::o_task_id)),
            where(c(&TimerPo::o_map_id) == std::move(mapId))
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
