//
// Created by Looper on 2023/2/28.
//

#include "db/task_data_base.h"

#include <utility>

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
}

std::vector<TaskVo> TaskDataBase::loadTaskFoMap(std::string mapId) {
    std::vector<TaskVo> tasks;

    auto taskPos = taskStorage.get_all<TaskPo>(c(&TaskPo::o_map_id) == std::move(mapId));
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

void TaskDataBase::deleteOwnTask() {
    taskStorage.remove_all<ZonePo>();
    taskStorage.remove_all<TaskPo>();
}

void TaskDataBase::deleteTaskFoMap(std::string mapId) {
    auto taskPos = taskStorage.get_all<TaskPo>(c(&TaskPo::o_map_id) == std::move(mapId));
    for (const auto &task: taskPos) {
        deleteTaskFoId(task.id);
    }
}

void TaskDataBase::deleteTaskFoId(long taskId) {
    taskStorage.remove_all<ZonePo>(where(c(&ZonePo::o_task_id) == taskId));
    taskStorage.remove<TaskPo>(taskId);
}

bool TaskDataBase::loadTask() {
    taskStorage.sync_schema();
    const auto taskList = taskStorage.get_all<TaskPo>();
    return false;
}
