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
                taskPo.partition, SqliteDataBase::SourceToString(taskPo.source), taskPo.launch_people,
                taskPo.launch_time, taskPo.update_time, taskPo.create_time);
    //zone
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
    return task;
}

TimerVo TaskDataBase::timerPo2Vo(const TimerPo &timerPo, const std::string &taskName) {
    return TimerVo(timerPo.id, timerPo.rule, timerPo.o_task_id, timerPo.name,
                   taskName, timerPo.is_execute, timerPo.rate, timerPo.is_never, timerPo.is_skip,
                   timerPo.end_year, timerPo.end_month, timerPo.end_day);
}

TEST_CASE() {
    TaskDataBase::instance().initialize();

    TaskDataBase::instance().deleteOwnTask();

    std::string map_id = "888";
    TaskVo task1(0, map_id, "task test 1", 1, 0, false, "App", "looper", 0l, 0l, 0l);
    TaskVo task2(0, map_id, "task test 2", 1, 0, false, "App", "looper", 0l, 0l, 0l);
    TaskVo task3(0, map_id, "task test 3", 1, 0, false, "App", "looper", 0l, 0l, 0l);

    std::vector<std::vector<PointVo>> zones;
    for (int i = 0; i < rand() % 5; i++) {
        std::vector<PointVo> points;
        for (int j = 0; j < 4; j++) {
            points.emplace_back(rand() % 10, rand() % 10);
        }
        zones.push_back(points);
    }
    task1.setZones(zones);
    task2.setZones(zones);
    task3.setZones(zones);

    TaskDataBase::instance().addZoneTask(map_id, task1);
    TaskDataBase::instance().addZoneTask(map_id, task2);
    TaskDataBase::instance().addZoneTask(map_id, task3);

    const std::vector<TaskVo> &vector = TaskDataBase::instance().loadTaskFoMap(map_id);
    std::cout << "loadTaskFoMap ------------------------------------------------------" << std::endl;
    for (const auto &item: vector) {
        std::cout << item << std::endl;
    }

    if (!vector.empty()) {
        auto task = vector[0];
        const TaskVo &vo = TaskDataBase::instance().loadTaskFoId(task.getId());
        std::cout << "loadTaskFoTask ------------------------------------------------------" << std::endl;
        std::cout << vo << std::endl;


        TimerVo timer1(0, "1357", 0, "timer test 1", "", false, 1, false, false, 0, 0, 0);
        TaskDataBase::instance().addTimer(map_id, task.getId(), timer1);

        TimerVo timer2(0, "1357", 0, "timer test 2", "", false, 1, false, false, 0, 0, 0);
        TaskDataBase::instance().addTimer(map_id, task.getId(), timer2);

        //todo
        TaskDataBase::instance().loadTimerFoMap(map_id);

        TaskDataBase::instance().deleteTaskFoId(task.getId());
    }
    const std::vector<TaskVo> &vector2 = TaskDataBase::instance().loadTaskFoMap(map_id);
    std::cout << "deleteTaskFoId ------------------------------------------------------" << std::endl;
    for (const auto &item: vector2) {
        std::cout << item << std::endl;
    }

    TaskDataBase::instance().deleteTaskFoMap(map_id);
    const std::vector<TaskVo> &vector3 = TaskDataBase::instance().loadTaskFoMap(map_id);
    std::cout << "deleteTaskFoMap ------------------------------------------------------" << std::endl;
    for (const auto &item: vector3) {
        std::cout << item << std::endl;
    }
}

void TaskDataBase::initialize() {
    taskStorage.sync_schema();
}

void TaskDataBase::addZoneTask(const std::string &mapId, const TaskVo &taskVo) {
    std::vector<std::string> ranges;
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
        ranges.push_back(pointRange);
    }

    std::vector<ZonePo> v;
    TaskPo taskPo(0,
                  mapId,
                  taskVo.getName(),
                  taskVo.getRate(),
                  TaskMode::Zoned,
                  taskVo.getWorkStatus().getSweepStatus(),
                  taskVo.getWorkStatus().getMopStatus(),
                  taskVo.getWorkStatus().getVacuumStatus(),
                  taskVo.getWorkStatus().getPushStatus(),
                  taskVo.getWorkStatus().getAromatherapyStatus(),
                  taskVo.getWorkStatus().getDisinfectStatus(),
                  v,
                  false,
                  "",
                  SqliteDataBase::TaskSourceFromString(taskVo.getSource()),
                  taskVo.getLaunchPeople(),
                  time_t(),
                  std::time(nullptr),
                  std::time(nullptr)
    );

    taskStorage.transaction([this, taskPo, ranges] {
        auto taskId = taskStorage.insert(taskPo);
        for (const auto &item: ranges) {
            const ZonePo &zonePo = ZonePo(0, taskId, item);
            taskStorage.insert(zonePo);
        }
        return true;
    });

}

void TaskDataBase::addTimer(const std::string &mapId, long taskId, const TimerVo &timer) {
    TimerPo timerPo(0, mapId, taskId, timer.getTimerRule(), timer.getTimerName(), timer.isExecute(), timer.getRate(),
                    timer.isNever(), timer.isSkip(), timer.getEndYear(), timer.getEndMonth(), timer.getEndDay());
    taskStorage.insert(timerPo);
}

void TaskDataBase::deleteOwnTask() {
    taskStorage.remove_all<TimerPo>();
    taskStorage.remove_all<ZonePo>();
    taskStorage.remove_all<TaskPo>();
}

void TaskDataBase::deleteTaskFoMap(std::string mapId) {
    auto taskPos = taskStorage.get_all<TaskPo>(where(c(&TaskPo::o_map_id) == std::move(mapId)));
    for (const auto &task: taskPos) {
        deleteTaskFoId(task.id);
    }
}

void TaskDataBase::deleteTaskFoId(long taskId) {
    taskStorage.remove_all<TimerPo>(where(c(&TimerPo::o_task_id) == taskId));
    taskStorage.remove_all<ZonePo>(where(c(&ZonePo::o_task_id) == taskId));
    taskStorage.remove<TaskPo>(taskId);
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
    return taskPo2Vo(taskPo);
}

std::vector<TimerVo> TaskDataBase::loadTimerFoMap(std::string mapId) {
    auto results = taskStorage.select(
            columns(&TimerPo::id,
                    &TimerPo::o_map_id,
                    &TimerPo::o_task_id,
                    &TimerPo::rule,
                    &TimerPo::name,

                    &TimerPo::is_execute,
                    &TimerPo::rate,
                    &TimerPo::is_never,
                    &TimerPo::is_skip,
                    &TimerPo::end_year,
                    &TimerPo::end_month,

                    &TimerPo::end_day,

                    &TaskPo::name
            ),
            join<ZonePo>(
                    on(
                            c(&TaskPo::id) == &TimerPo::o_task_id
                            and
                            c(&TimerPo::o_map_id) == std::move(mapId)
                    )
            )
    );

    for (const auto &row: results) {
        std::cout << std::get<0>(row) << std::endl;
        TimerPo timerPo(std::get<0>(row), std::get<0>(row), std::get<0>(row), std::get<0>(row), const std::string &name,
        bool isExecute, int rate, bool isNever, bool isSkip, int endYear, int endMonth, int endDay);
    }
}

TimerVo TaskDataBase::loadTimerFoId(long timerId) {
//    auto timerPo = taskStorage.get<TimerPo>(timerId);
//    return timerPo2Vo(timerPo);
}
