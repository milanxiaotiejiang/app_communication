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

public:
    static auto &instance() {
        static TaskDataBase obj;
        return obj;
    }

    std::vector<TaskVo> loadTaskFoMap(std::string mapId);

    void deleteOwnTask();

    void deleteTaskFoMap(std::string mapId);

    void deleteTaskFoId(long taskId);

    bool loadTask();
};


#endif //APP_COMMUNICATION_TASK_DATA_BASE_H
