//
// Created by milan on 2021/12/15.
//

#ifndef C_PTHREAD_TASK_H
#define C_PTHREAD_TASK_H

#include "WorkStatus.h"
#include "vector"
#include "FullPath.h"
#include "CleanContinuity.h"
#include "CleanPolygon.h"
#include <ostream>
#include <stdlib.h>
#include <string>
#include "model/TeachModePoint.h"

#include <nlohmann/json.hpp>

using json = nlohmann::json;

class Combination {
private:

    std::string m_combination_id;

public:
    Combination() {}

    ~Combination() {}

    const std::string &getCombinationID() const {
        return m_combination_id;
    }

    void setCombinationID(const std::string &combination_id) {
        m_combination_id = combination_id;
    }

    friend void to_json(json &j, const Combination &b) {
        j = json{
                {"combination_id", b.m_combination_id}};
    }

    friend void from_json(const json &j, Combination &b) {

        j.at("combination_id").get_to(b.m_combination_id);
    }
};

class Task {

private:
    std::string task_id;
    int mode;
    int rate;
    WorkStatus work_status;
    CleanPolygon polygon;
    std::vector<float> zoned;
    CleanContinuity continuity;
    TeachPathInfo teach_path;
    std::string launch_people;
    long launch_time;
    std::string time_mode;
    bool in_execute;
    Combination m_combination;
    FullPath full_path;
    int task_type{1};
    int oper_event{0};

public:
    Task();

    virtual ~Task();

    Task(const std::string &taskId, int mode, int rate, const WorkStatus &workStatus, const CleanPolygon &polygon,
         const std::vector<float> &zoned, const CleanContinuity &continuity, const TeachPathInfo &teach_path,
         const std::string &launchPeople, long launchTime, const std::string &timeMode, bool inExecute);

    Task(const std::string &taskId, const WorkStatus &workStatus);

    const std::string &getTaskId() const;

    void setTaskId(const std::string &taskId);

    int getMode() const;

    void setMode(int mode);

    int gettask_type() const;

    void settask_type(int type);

    int getoper_event() const;

    void setoper_event(int oe);

    int getRate() const;

    void setRate(int rate);

    const CleanPolygon &getPolygon() const;

    void setPolygon(const CleanPolygon &polygon);

    const WorkStatus &getWorkStatus() const;

    void setWorkStatus(const WorkStatus &workStatus);

    const std::vector<float> &getZoned() const;

    void setZoned(const std::vector<float> &zoned);

    const CleanContinuity &getContinuity() const;

    void setContinuity(const CleanContinuity &continuity);

    const TeachPathInfo &getTeachPath() const;

    void setTeachPath(const TeachPathInfo &teachPath);

    const std::string &getLaunchPeople() const;

    void setLaunchPeople(const std::string &launchPeople);

    long getLaunchTime() const;

    void setLaunchTime(long launchTime);

    const std::string &getTimeMode() const;

    void setTimeMode(const std::string &timeMode);

    bool isInExecute() const;

    void setInExecute(bool inExecute);

    const Combination &getCombination() const;

    void setCombination(const Combination &combination);

    const FullPath &getFullPath() const;

    void setFullPath(const FullPath &fullPath);

    friend void to_json(json &j, const Task &b) {
        j = json{
                {"task_id",       b.task_id},
                {"mode",          b.mode},
                {"rate",          b.rate},
                {"work_status",   b.work_status},
                {"zoned",         b.zoned},
                {"polygon",       b.polygon},
                {"continuity",    b.continuity},
                {"teach_path",    b.teach_path},
                {"launch_people", b.launch_people},
                {"launch_time",   b.launch_time},
                {"time_mode",     b.time_mode},
                {"in_execute",    b.in_execute},
                {"full_path",     b.full_path},
                {"combination",   b.m_combination},
                {"task_type",     b.task_type},
                {"oper_event",    b.oper_event}};
    }

    friend void from_json(const json &j, Task &b) {
        j.at("task_id").get_to(b.task_id);
        j.at("mode").get_to(b.mode);
        j.at("rate").get_to(b.rate);
        j.at("work_status").get_to(b.work_status);
        j.at("zoned").get_to(b.zoned);
        j.at("polygon").get_to(b.polygon);
        j.at("continuity").get_to(b.continuity);
        j.at("teach_path").get_to(b.teach_path);
        j.at("launch_people").get_to(b.launch_people);
        j.at("launch_time").get_to(b.launch_time);
        j.at("time_mode").get_to(b.time_mode);
        j.at("in_execute").get_to(b.in_execute);
        j.at("full_path").get_to(b.full_path);
        j.at("combination").get_to(b.m_combination);
        try {
            j.at("task_type").get_to(b.task_type);
            j.at("oper_event").get_to(b.oper_event);

        }
        catch (...) {
            //   std::cout<<"task name "<<j.at("task_name")<<std::endl;

            b.task_type = 1;
            b.oper_event = 0;
        }
    }
};


#endif//C_PTHREAD_TASK_H
