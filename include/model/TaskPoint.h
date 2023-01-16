//
// Created by milan on 2021/12/15.
//

#ifndef C_PTHREAD_TASKPOINT_H
#define C_PTHREAD_TASKPOINT_H

#include "WorkStatus.h"
#include "string"
#include <ostream>


using namespace std;

class TaskPoint {
private:
    string id;
    float x{};
    float y{};
    float ori_z{};
    float ori_w{};
    int currentStep{};
    int totalStep{};
    int currentFrequency{};
    int totalFrequency{};
    WorkStatus work_status;
    int plannerType{0};
    bool is_cleaning{false};
    int m_mode;
    int m_back_to_base{0};

public:
    TaskPoint();

    TaskPoint(const string &id, float x, float y, int currentFrequency, int totalFrequency,
              const WorkStatus &workStatus, int plannerType, bool is_cleaning, int mode);

    TaskPoint(const string &id, float x, float y, int currentStep, int totalStep, int currentFrequency,
              int totalFrequency, const WorkStatus &workStatus, int plannerType, bool is_cleaning, int mode);

    virtual ~TaskPoint();

    const string &getId() const;

    void setId(const string &id);

    float getX() const;

    void setX(float x);

    float getY() const;

    void setY(float y);

    float getOriZ() const;

    void setOriZ(float oriZ);

    float getOriW() const;

    void setOriW(float oriW);

    int getCurrentStep() const;

    void setCurrentStep(int currentStep);

    int getTotalStep() const;

    void setTotalStep(int totalStep);

    int getCurrentFrequency() const;

    void setCurrentFrequency(int currentFrequency);

    int getTotalFrequency() const;

    void setTotalFrequency(int totalFrequency);

    const WorkStatus &getWorkStatus() const;

    void setWorkStatus(const WorkStatus &workStatus);

    int getPlannerType() const;

    void setPlannerType(int plannerType);

    bool isCleaning() const;

    void setIsCleaning(bool isCleaning);

    int getMode() const {
        return m_mode;
    }

    int getBcakToBase() const {
        return m_back_to_base;
    }

    void setBcakToBase(int backToBase) {
        m_back_to_base = backToBase;
    }

    void setMode(int mode) {
        m_mode = mode;
    }

    friend void to_json(json &j, const TaskPoint &b) {
        j = json{
                {"id", b.id},
                {"x", b.x},
                {"y", b.y},
                {"currentStep", b.currentStep},
                {"totalStep", b.totalStep},
                {"currentFrequency", b.currentFrequency},
                {"totalFrequency", b.totalFrequency},
                {"work_status", b.work_status},
                {"plannerType", b.plannerType},
                {"is_cleaning", b.is_cleaning},
                {"mode", b.m_mode}};
    }

    friend void from_json(const json &j, TaskPoint &b) {
        j.at("id").get_to(b.id);
        j.at("x").get_to(b.x);
        j.at("y").get_to(b.y);
        j.at("currentStep").get_to(b.currentStep);
        j.at("totalStep").get_to(b.totalStep);
        j.at("currentFrequency").get_to(b.currentFrequency);
        j.at("totalFrequency").get_to(b.totalFrequency);
        j.at("work_status").get_to(b.work_status);
        j.at("plannerType").get_to(b.plannerType);
        j.at("is_cleaning").get_to(b.is_cleaning);
        j.at("mode").get_to(b.m_mode);
    }

    friend ostream &operator<<(ostream &os, const TaskPoint &point);
};

const int INSTRUCT_TASK_POINT_MOVE = 1;
const int INSTRUCT_TASK_POINT_BASE = 2;
const int CANCEL_GOAL = 3;

class TaskPointInstruct {
private:
    int instruct;
    TaskPoint taskPoint;

public:
    TaskPointInstruct();

    TaskPointInstruct(int instruct, const TaskPoint &taskPoint);

    int getInstruct() const;

    const TaskPoint &getTaskPoint() const;

    friend ostream &operator<<(ostream &os, const TaskPointInstruct &instruct);
};

enum {
    NOT_BACK_TO_BASE = 0,
    NORMAL_BACK_TO_BASE = 1,
    LOWPOWER_BACK_TO_BASE = 2
};


#endif//C_PTHREAD_TASKPOINT_H
