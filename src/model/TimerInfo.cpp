//
// Created by Paul Wang on 2022/05/10.
//

#include "model/TimerInfo.h"

TimerInfo::TimerInfo() {}

//TimerInfo::TimerInfo(const std::string &timerId, const std::string &timerRule,  const std::string &timername, const std::string &taskId, bool isExecute,int  Rate,bool isnever,bool isskip,int endyear,int endmonth,int endday)
TimerInfo::TimerInfo(const std::string &timerId, const std::string &timerRule, const std::string &timername,
                     const std::string &taskId, bool isExecute, int Rate, const std::string &taskname, bool isnever,
                     bool isskip, int endyear, int endmonth, int endday, bool old)
        : timer_id(timerId),
          timer_rule(timerRule),
          task_id(taskId),
          is_execute(isExecute),
          rate(Rate),
          timer_name(timername),
          task_name(taskname),
          is_never(isnever),
          is_skip(isskip),
          end_year(endyear),
          end_month(endmonth),
          end_day(endday),
          old(old) {

}

TimerInfo::~TimerInfo() {
}

const std::string &TimerInfo::getTimerId() const {
    return timer_id;
}

void TimerInfo::setTimerId(const std::string &timerId) {
    timer_id = timerId;
}

const std::string &TimerInfo::getTimerRule() const {
    return timer_rule;
}

void TimerInfo::setTimerRule(const std::string &timerule) {
    timer_rule = timerule;
}

const std::string &TimerInfo::getTimerName() const {
    return timer_name;
}

void TimerInfo::setTimerName(const std::string &timename) {
    timer_name = timename;
}

const std::string &TimerInfo::getTaskName() const {
    return task_name;
}

void TimerInfo::setTaskName(const std::string &taskname) {
    task_name = taskname;
}

bool TimerInfo::getIsNever() const {
    return is_never;

}

void TimerInfo::setIsNever(bool isnever) {
    is_never = isnever;
}

bool TimerInfo::getIsSkip() const {
    return is_skip;
}

void TimerInfo::setIsSkip(bool isskip) {
    is_skip = isskip;
}

int TimerInfo::getEndYear() const {
    return end_year;
}

void TimerInfo::setEndYear(int endyear) {
    end_year = endyear;
}

int TimerInfo::getEndMonth() const {
    return end_month;
}

void TimerInfo::setEndMonth(int endmonth) {
    end_month = endmonth;
}

int TimerInfo::getEndDay() const {
    return end_day;
}

void TimerInfo::setEndDay(int endday) {
    end_day = endday;
}

const std::string &TimerInfo::getTaskId() const {
    return task_id;
}

void TimerInfo::setTaskId(const std::string &taskid) {
    task_id = taskid;
}

bool TimerInfo::getIsExecute() const {
    return is_execute;
}

void TimerInfo::setIsExecute(bool isexecute) {
    is_execute = isexecute;
}

int TimerInfo::getRate() const {
    return rate;
}

void TimerInfo::setRate(int irate) {
    rate = irate;
}

bool TimerInfo::isOld() const {
    return old;
}

void TimerInfo::setOld(bool old) {
    TimerInfo::old = old;
}

///////////////////////////

TimerActive::TimerActive() {}

TimerActive::TimerActive(const std::string &timerId, const std::string &active, const std::string &deactive) : timer_id(
        timerId),
                                                                                                               sactivate(
                                                                                                                       active),
                                                                                                               sdeactive(
                                                                                                                       deactive) {}

TimerActive::~TimerActive() {
}

const std::string &TimerActive::getTimerId() const {
    return timer_id;
}

void TimerActive::setTimerId(const std::string &timerId) {
    timer_id = timerId;
}


const std::string &TimerActive::getActivate() const {
    return sactivate;
}

void TimerActive::setActivate(const std::string &active) {
    sactivate = active;
}

const std::string &TimerActive::getDeActive() const {
    return sdeactive;
}

void TimerActive::setDeActive(const std::string &deactive) {
    sdeactive = deactive;
}
