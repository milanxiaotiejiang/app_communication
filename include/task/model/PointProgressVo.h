//
// Created by Looper on 2022/11/27.
//

#ifndef APP_COMMUNICATION_POINTPROGRESSVO_H
#define APP_COMMUNICATION_POINTPROGRESSVO_H

#include "model/WorkStatus.h"

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <ostream>

class PointProgressVo {
private:
    std::string id;
    float x{};
    float y{};
    int currentStep{};
    int totalStep{};
    int currentFrequency{};
    int totalFrequency{};
    WorkStatus work_status;
    int mode;
    bool is_cleaning;

    std::string taskId;//运行中的任务ID

    bool renew{false};//新旧任务标志位
    std::string oldTaskId;//旧任务，有值就是CombinationID，没值就是全覆盖
    long newTaskId{0};//新任务，可从数据库查找到的

    double value;

public:
    PointProgressVo() = default;

    PointProgressVo(float x, float y, int currentStep, int totalStep, int currentFrequency,
                    int totalFrequency, const WorkStatus &workStatus, int mode, bool isCleaning,
                    const std::string &taskId, bool renew, const std::string &oldTaskId, long newTaskId,
                    double value) :
            id(boost::uuids::to_string(boost::uuids::random_generator()())),
            x(x), y(y),
            currentStep(currentStep), totalStep(totalStep),
            currentFrequency(currentFrequency), totalFrequency(totalFrequency),
            work_status(workStatus),
            mode(mode),
            is_cleaning(isCleaning),
            taskId(taskId),
            renew(renew),
            oldTaskId(oldTaskId),
            newTaskId(newTaskId),
            value(value) {}

    friend void to_json(json &j, const PointProgressVo &vo) {
        j = json{
                {"id",               vo.id},
                {"x",                vo.x},
                {"y",                vo.y},
                {"currentStep",      vo.currentStep},
                {"totalStep",        vo.totalStep},
                {"currentFrequency", vo.currentFrequency},
                {"totalFrequency",   vo.totalFrequency},
                {"work_status",      vo.work_status},
                {"mode",             vo.mode},
                {"is_cleaning",      vo.is_cleaning},
                {"task_id",          vo.taskId},
                {"renew",            vo.renew},
                {"old_task_id",      vo.oldTaskId},
                {"new_task_id",      vo.newTaskId},
                {"value",            vo.value},
        };
    }

    friend void from_json(const json &j, PointProgressVo &vo) {
        j.at("id").get_to(vo.id);
        j.at("x").get_to(vo.x);
        j.at("y").get_to(vo.y);
        j.at("currentStep").get_to(vo.currentStep);
        j.at("totalStep").get_to(vo.totalStep);
        j.at("currentFrequency").get_to(vo.currentFrequency);
        j.at("totalFrequency").get_to(vo.totalFrequency);
        j.at("work_status").get_to(vo.work_status);
        j.at("mode").get_to(vo.mode);
        j.at("is_cleaning").get_to(vo.is_cleaning);
        j.at("task_id").get_to(vo.taskId);
        j.at("renew").get_to(vo.renew);
        j.at("old_task_id").get_to(vo.oldTaskId);
        j.at("new_task_id").get_to(vo.newTaskId);
        j.at("value").get_to(vo.value);
    }

    friend std::ostream &operator<<(std::ostream &os, const PointProgressVo &vo) {
        os << " currentStep: " << vo.currentStep
           << " totalStep: " << vo.totalStep
           << " currentFrequency: " << vo.currentFrequency
           << " totalFrequency: " << vo.totalFrequency
           << " is_cleaning: " << vo.is_cleaning
           << " taskId: " << vo.taskId
           << " renew: " << vo.renew
           << " x: " << vo.x << " y: " << vo.y
           << " value: " << vo.value
           << " newTaskId: " << vo.newTaskId;
        return os;
    }

    float getX() const {
        return x;
    }

    float getY() const {
        return y;
    }

    int getCurrentStep() const {
        return currentStep;
    }

    int getTotalStep() const {
        return totalStep;
    }

    int getCurrentFrequency() const {
        return currentFrequency;
    }

    int getTotalFrequency() const {
        return totalFrequency;
    }

    bool isRenew() const {
        return renew;
    }

    long getNewTaskId() const {
        return newTaskId;
    }

    double getValue() const {
        return value;
    }
};


#endif //APP_COMMUNICATION_POINTPROGRESSVO_H
