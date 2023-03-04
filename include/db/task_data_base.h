//
// Created by Looper on 2023/2/28.
//

#ifndef APP_COMMUNICATION_TASK_DATA_BASE_H
#define APP_COMMUNICATION_TASK_DATA_BASE_H

#include "SqliteDataBase.h"
#include "model/Task.h"
#include "model/task.h"

class TaskDataBase {
private:
    TaskStorage taskStorage = SqliteDataBase::initTaskStorage();

    void split(const std::string &s, std::vector<std::string> &tokens, char delim = ' ');

    TaskVo taskPo2Vo(const TaskPo &taskPo);

    TimerVo timerPo2Vo(const TimerPo &timerPo);

    void addTaskVo(const std::string &mapId, const TaskVo &taskVo);

public:
    static auto &instance() {
        static TaskDataBase obj;
        return obj;
    }

    void initialize();

    //增
    void addZoneTask(const std::string &mapId, const TaskVo &taskVo);

    void addCoverTask(const std::string &mapId, const TaskVo &taskVo);

    void addSubregionTask(const std::string &mapId, const TaskVo &taskVo);

    void addLineTask(const std::string &mapId, const TaskVo &taskVo);

    void addTimer(const std::string &mapId, long taskId, const TimerVo &timer);

    //删
    void deleteOwnTask();

    void deleteTaskFoId(long taskId);

    void deleteTaskFoMap(std::string mapId);

    void deleteTimerForId(long timerId);

    void deleteTimerForTask(long taskId);

    void deleteTimerForMap(std::string mapId);

    //改

    //查
    std::vector<TaskVo> loadTaskFoMap(std::string mapId);

    TaskVo loadTaskFoId(long taskId);

    std::vector<TimerVo> loadTimerFoMap(std::string mapId);

    TimerVo loadTimerFoId(long timerId);

};


#endif //APP_COMMUNICATION_TASK_DATA_BASE_H
