//
// Created by Paul Wang on 2022/05/10.
//

#ifndef APP_COMMUNICATION_TIMERINFO_H
#define APP_COMMUNICATION_TIMERINFO_H

#include <string>

#include "nlohmann/json.hpp"
#include <iostream>

using json = nlohmann::json;

class TimerActive {
private:
    std::string timer_id; //timer_id用作记录id的排序
    std::string sactivate; //使能/失能
    std::string sdeactive; //执行的任务ID，传 “” 是全局，其他都是组合任务

public:
    TimerActive();

    virtual ~TimerActive();

    TimerActive(const std::string &timerId, const std::string &active, const std::string &deactive);

    const std::string &getTimerId() const;

    void setTimerId(const std::string &timerId);


    const std::string &getActivate() const;

    void setActivate(const std::string &active);

    const std::string &getDeActive() const;

    void setDeActive(const std::string &deactive);

    friend void to_json(json &j, const TimerActive &b) {
        j = json{
                {"timer_id", b.timer_id},
                {"activate", b.sactivate},
                {"deactive", b.sdeactive},


        };
    }

    friend void from_json(const json &j, TimerActive &b) {
        j.at("timer_id").get_to(b.timer_id);
        j.at("activate").get_to(b.sactivate);
        j.at("deactive").get_to(b.sdeactive);
    }

};

class TimerInfo {
private:
    std::string timer_id; //timer_id用作记录id的排序
    std::string timer_rule; //cron-like中的计时符号
    std::string task_id; //执行的任务ID，传 “” 是全局，其他都是组合任务
    std::string timer_name; //定时器名字
    std::string task_name; //定时器名字
    bool is_execute{true};//是否开启
    int rate;//清扫次数
    bool is_never;//True 不结束 False 截止日期
    bool is_skip{true};//跳过节假日

    int end_year;//截止日期年
    int end_month;//截止日期月
    int end_day;//截止日期日

public:
    TimerInfo();

    virtual ~TimerInfo();

    //TimerInfo(const std::string &timerId, const std::string &timerRule,  const std::string &timername, const std::string &taskId, bool isExecute,int  Rate,bool is_never,bool is_skip,const std::string &endyear,const std::string &endmonth,const std::string &endday);
    TimerInfo(const std::string &timerId, const std::string &timerRule, const std::string &timername,
              const std::string &taskId, bool isExecute, int Rate, const std::string &taskname, bool is_never,
              bool is_skip, int endyear, int endmonth, int endday);

    const std::string &getTimerId() const;

    void setTimerId(const std::string &timerId);

    const std::string &getTimerRule() const;

    void setTimerRule(const std::string &timerule);

    const std::string &getTimerName() const;

    void setTimerName(const std::string &timename);

    const std::string &getTaskName() const;

    void setTaskName(const std::string &taskname);

    const std::string &getTaskId() const;

    void setTaskId(const std::string &taskid);

    bool getIsExecute() const;

    void setIsExecute(bool isexecute);

    int getRate() const;

    void setRate(int irate);

    bool getIsNever() const;

    void setIsNever(bool isnever);

    bool getIsSkip() const;

    void setIsSkip(bool isskip);

    int getEndYear() const;

    void setEndYear(int endyear);

    int getEndMonth() const;

    void setEndMonth(int endmonth);

    int getEndDay() const;

    void setEndDay(int endday);

    friend void to_json(json &j, const TimerInfo &b) {
        j = json{
                {"timer_id",   b.timer_id},
                {"timer_rule", b.timer_rule},
                {"task_id",    b.task_id},
                {"is_execute", b.is_execute},
                {"rate",       b.rate},
                {"timer_name", b.timer_name},
                {"task_name",  b.task_name},
                {"is_never",   b.is_never},
                {"is_skip",    b.is_skip},
                {"year",       b.end_year},
                {"month",      b.end_month},
                {"day",        b.end_day}

        };
    }

    friend void from_json(const json &j, TimerInfo &b) {

        j.at("timer_id").get_to(b.timer_id);
        j.at("timer_rule").get_to(b.timer_rule);
        j.at("task_id").get_to(b.task_id);
        j.at("is_execute").get_to(b.is_execute);
        j.at("rate").get_to(b.rate);
        j.at("timer_name").get_to(b.timer_name);
        //   if(j.at("task_name") != undefined)
        //    std::cout<<"task name "<<j.at("task_name")<<std::endl;
        try {
            j.at("task_name").get_to(b.task_name);
        }
        catch (...) {
            //   std::cout<<"task name "<<j.at("task_name")<<std::endl;
            b.task_name = "empty";
        }


        j.at("is_never").get_to(b.is_never);
        j.at("is_skip").get_to(b.is_skip);
        j.at("year").get_to(b.end_year);
        j.at("month").get_to(b.end_month);
        j.at("day").get_to(b.end_day);
        // j.at("task_name").get_to(b.task_name);
    }
};


#endif //APP_COMMUNICATION_TIMERINFO_H
