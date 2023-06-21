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

    long addTaskVo(const std::string &mapId, const TaskVo &taskVo);

public:
    static auto &instance() {
        static TaskDataBase obj;
        return obj;
    }

    void initialize();

    //增
    long addTask(const std::string &mapId, const TaskVo &taskVo);

    long addTimer(const std::string &mapId, const TimerVo &timer);

    //删
    void deleteOwnTask();

    void deleteTaskFoId(long taskId);

    void deleteTaskFoMap(std::string mapId);

    void deleteTaskFoMode(std::string mapId, TaskMode mode);

    void deleteTaskFoMode(std::string mapId, TaskMode mode, bool partition);

    void deleteTimerForId(long timerId);

    void deleteTimerForTask(long taskId);

    void deleteTimerForMap(std::string mapId);

    //改
    TaskVo modifyPrincipalTask(std::string mapId, long taskId, bool principal);

    TaskVo modifyRainSnowTask(std::string mapId, long taskId, bool rainSnow);

    void modifyName(long taskId, std::string name);

    void modifyRate(long taskId, int rate);

    void modifyWorkStatus(long taskId, const WorkStatus &status);

    void modifyKnife(long taskId, bool knife);

    TaskVo modifyTask(const TaskVo &taskVo);

    long operateAddZone(long taskId, const ZoneVo &zone);

    void operateDeleteZone(long taskId, const ZoneVo &zone);

    void operateModifyZone(long taskId, const ZoneVo &zone);

    void modifyPartition(long taskId, bool partition);

    long operateAddSubregion(long taskId, const SubregionVo &subregion);

    void operateDeleteSubregion(long taskId, const SubregionVo &subregion);

    void modifyTimerName(long timerId, std::string name);

    void modifyTimer(const std::string &mapId, const TimerVo &timer);

    //查
    std::vector<TaskVo> loadTaskFoMap(std::string mapId);

    TaskVo loadTaskFoId(long taskId);

    TaskVo loadPrincipalTask(const std::string &mapId);

    TaskVo loadRainSnowTask(const std::string &mapId);

    std::vector<TimerVo> loadTimerFoMap(const std::string &mapId);

    TimerVo loadTimerFoId(long timerId);

};


#endif //APP_COMMUNICATION_TASK_DATA_BASE_H
