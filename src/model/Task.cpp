//
// Created by milan on 2021/12/15.
//

#include "model/Task.h"

Task::Task() {}

Task::~Task() {
}

const std::string &Task::getTaskId() const {
    return task_id;
}

void Task::setTaskId(const std::string &taskId) {
    task_id = taskId;
}

int Task::getMode() const {
    return mode;
}

void Task::setMode(int mode) {
    Task::mode = mode;
}

int Task::getRate() const {
    return rate;
}

int Task::gettask_type() const {
    return task_type;
}

void Task::settask_type(int type) {
    task_type = type;
}

int Task::getoper_event() const {
    return oper_event;
}

void Task::setoper_event(int oe) {
    oper_event = oe;
}

void Task::setRate(int rate) {
    Task::rate = rate;
}

const WorkStatus &Task::getWorkStatus() const {
    return work_status;
}

void Task::setWorkStatus(const WorkStatus &workStatus) {
    work_status = workStatus;
}

const std::vector<float> &Task::getZoned() const {
    return zoned;
}

void Task::setZoned(const std::vector<float> &zoned) {
    Task::zoned = zoned;
}

const CleanContinuity &Task::getContinuity() const {
    return continuity;
}

void Task::setContinuity(const CleanContinuity &continuity) {
    Task::continuity = continuity;
}


const std::string &Task::getLaunchPeople() const {
    return launch_people;
}

void Task::setLaunchPeople(const std::string &launchPeople) {
    launch_people = launchPeople;
}

long Task::getLaunchTime() const {
    return launch_time;
}

void Task::setLaunchTime(long launchTime) {
    launch_time = launchTime;
}

const std::string &Task::getTimeMode() const {
    return time_mode;
}

void Task::setTimeMode(const std::string &timeMode) {
    time_mode = timeMode;
}

bool Task::isInExecute() const {
    return in_execute;
}

void Task::setInExecute(bool inExecute) {
    in_execute = inExecute;
}

Task::Task(const std::string &taskId, const WorkStatus &workStatus) : task_id(taskId), work_status(workStatus) {

}

const CleanPolygon &Task::getPolygon() const {
    return polygon;
}

void Task::setPolygon(const CleanPolygon &polygon) {
    Task::polygon = polygon;
}

Task::Task(const std::string &taskId, int mode, int rate, const WorkStatus &workStatus, const CleanPolygon &polygon,
           const std::vector<float> &zoned, const CleanContinuity &continuity, const TeachPathInfo &teach_path,
           const std::string &launchPeople, long launchTime, const std::string &timeMode, bool inExecute) : task_id(taskId),
                                                                                                  mode(mode),
                                                                                                  time_mode(timeMode),
                                                                                                  in_execute(
                                                                                                          inExecute) {}

const TeachPathInfo &Task::getTeachPath() const {
    return teach_path;
}

void Task::setTeachPath(const TeachPathInfo &teachPath) {
    teach_path = teachPath;
}

const Combination &Task::getCombination() const {
    return m_combination;
}

void Task::setCombination(const Combination &combination) {
    m_combination = combination;
}

const FullPath &Task::getFullPath() const {
    return full_path;
}

void Task::setFullPath(const FullPath &fullPath) {
    full_path = fullPath;
}
