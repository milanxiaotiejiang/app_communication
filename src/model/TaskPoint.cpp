//
// Created by milan on 2021/12/15.
//

#include "model/TaskPoint.h"

TaskPoint::TaskPoint() : currentStep(0) {
    ori_z = 0;
    ori_w = 1;
}

TaskPoint::TaskPoint(const string &id, float x, float y, int currentStep, int totalStep, int currentFrequency,
                     int totalFrequency, const WorkStatus &workStatus, int plannerType, bool is_cleaning, int mode)
        : id(id),
          x(x), y(y),
          currentStep(
                  currentStep),
          totalStep(
                  totalStep),
          currentFrequency(
                  currentFrequency),
          totalFrequency(
                  totalFrequency),
          work_status(
                  workStatus),
          plannerType(
                  plannerType),
          is_cleaning(
                  is_cleaning),
          m_mode(
                  mode) {
    ori_z = 0;
    ori_w = 1;
}

TaskPoint::~TaskPoint() = default;

const string &TaskPoint::getId() const {
    return id;
}

void TaskPoint::setId(const string &id) {
    TaskPoint::id = id;
}

float TaskPoint::getX() const {
    return x;
}

void TaskPoint::setX(float x) {
    TaskPoint::x = x;
}

float TaskPoint::getY() const {
    return y;
}

void TaskPoint::setY(float y) {
    TaskPoint::y = y;
}

int TaskPoint::getCurrentStep() const {
    return currentStep;
}

void TaskPoint::setCurrentStep(int currentStep) {
    TaskPoint::currentStep = currentStep;
}

int TaskPoint::getTotalStep() const {
    return totalStep;
}

void TaskPoint::setTotalStep(int totalStep) {
    TaskPoint::totalStep = totalStep;
}

int TaskPoint::getCurrentFrequency() const {
    return currentFrequency;
}

void TaskPoint::setCurrentFrequency(int currentFrequency) {
    TaskPoint::currentFrequency = currentFrequency;
}

int TaskPoint::getTotalFrequency() const {
    return totalFrequency;
}

void TaskPoint::setTotalFrequency(int totalFrequency) {
    TaskPoint::totalFrequency = totalFrequency;
}

const WorkStatus &TaskPoint::getWorkStatus() const {
    return work_status;
}

void TaskPoint::setWorkStatus(const WorkStatus &workStatus) {
    work_status = workStatus;
}

int TaskPoint::getPlannerType() const {
    return plannerType;
}

void TaskPoint::setPlannerType(int plannerType) {
    TaskPoint::plannerType = plannerType;
}


TaskPoint::TaskPoint(const string &id, float x, float y, int currentFrequency, int totalFrequency,
                     const WorkStatus &workStatus, int plannerType, bool is_cleaning, int mode) : id(id), x(x), y(y),
                                                                                                  currentFrequency(
                                                                                                          currentFrequency),
                                                                                                  totalFrequency(
                                                                                                          totalFrequency),
                                                                                                  work_status(
                                                                                                          workStatus),
                                                                                                  plannerType(
                                                                                                          plannerType),
                                                                                                  is_cleaning(
                                                                                                          is_cleaning),
                                                                                                  m_mode(mode) {
    ori_z = 0;
    ori_w = 1;
}

ostream &operator<<(ostream &os, const TaskPoint &point) {
    os << "x: " << point.x << " y: " << point.y << " currentStep: " << point.currentStep << " totalStep: "
       << point.totalStep << " currentFrequency: " << point.currentFrequency << " totalFrequency: "
       << point.totalFrequency << " plannerType: " << point.plannerType;
    return os;
}

float TaskPoint::getOriZ() const {
    return ori_z;
}

void TaskPoint::setOriZ(float oriZ) {
    ori_z = oriZ;
}

float TaskPoint::getOriW() const {
    return ori_w;
}

void TaskPoint::setOriW(float oriW) {
    ori_w = oriW;
}

bool TaskPoint::isCleaning() const {
    return is_cleaning;
}

void TaskPoint::setIsCleaning(bool isCleaning) {
    is_cleaning = isCleaning;
}

TaskPointInstruct::TaskPointInstruct(int instruct, const TaskPoint &taskPoint) : instruct(instruct),
                                                                                 taskPoint(taskPoint) {}

int TaskPointInstruct::getInstruct() const {
    return instruct;
}

const TaskPoint &TaskPointInstruct::getTaskPoint() const {
    return taskPoint;
}

TaskPointInstruct::TaskPointInstruct() {}

ostream &operator<<(ostream &os, const TaskPointInstruct &instruct) {
    os << "instruct: " << instruct.instruct << " taskPoint: " << instruct.taskPoint;
    return os;
}
