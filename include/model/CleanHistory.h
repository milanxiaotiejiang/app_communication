#ifndef APP_COMMUNICATION_CLEANHISTORY_H
#define APP_COMMUNICATION_CLEANHISTORY_H

#include "model/WorkStatus.h"
#include <std_msgs/String.h>
#include <vector>

using json = nlohmann::json;

class CleanHistory {//回复清洁历史
private:
    bool m_is_complete;
    bool m_base_complete;
    long m_launch_time;
    long m_execute_time;
    long m_end_time;
    int m_task_mode;
    std::string m_task_id;
    WorkStatus m_work_status;
    int m_clean_area;
    int m_clean_time;
    int m_error_code;
    std::string m_error_message;
    int m_task_type{1};//1:manual task 2:Timer task
    // Ewen change begin
    std::vector<int> m_oper_event;
    // Ewen change end

    bool renew;//新旧任务标志位
    std::string old_task_id;//旧任务id
    long new_task_id;//关联的新任务id

public:
    friend void to_json(json &j, const CleanHistory &b) {
        j = json{
                {"is_complete",   b.m_is_complete},
                {"base_complete", b.m_base_complete},
                {"launch_time",   b.m_launch_time},
                {"execute_time",  b.m_execute_time},
                {"end_time",      b.m_end_time},
                {"task_mode",     b.m_task_mode},
                {"task_id",       b.m_task_id},
                {"work_status",   b.m_work_status},
                {"clean_area",    b.m_clean_area},
                {"clean_time",    b.m_clean_time},
                {"error_code",    b.m_error_code},
                {"error_message", b.m_error_message},
                {"task_type",     b.m_task_type},
                {"oper_event",    b.m_oper_event},

                {"renew",         b.renew},
                {"old_task_id",   b.old_task_id},
                {"new_task_id",   b.new_task_id},
        };
    }

    friend void from_json(const json &j, CleanHistory &b) {
        j.at("is_complete").get_to(b.m_is_complete);
        j.at("launch_time").get_to(b.m_launch_time);
        j.at("execute_time").get_to(b.m_execute_time);
        j.at("end_time").get_to(b.m_end_time);
        j.at("task_mode").get_to(b.m_task_mode);
        j.at("task_id").get_to(b.m_task_id);
        j.at("work_status").get_to(b.m_work_status);
        j.at("clean_area").get_to(b.m_clean_area);
        j.at("clean_time").get_to(b.m_clean_time);
        j.at("error_code").get_to(b.m_error_code);
        j.at("error_message").get_to(b.m_error_message);

        try {
            j.at("base_complete").get_to(b.m_base_complete);
            j.at("task_type").get_to(b.m_task_type);
            j.at("oper_event").get_to(b.m_oper_event);
        }
        catch (...) {
            //   std::cout<<"task name "<<j.at("task_name")<<std::endl;
            b.m_base_complete = true;
            b.m_task_type = 1;
            b.m_oper_event.clear();
        }

        j.at("renew").get_to(b.renew);
        j.at("old_task_id").get_to(b.old_task_id);
        j.at("new_task_id").get_to(b.new_task_id);
    }

    CleanHistory();

    CleanHistory(bool is_complete,
                 bool base_complete,
                 long launch_time,
                 long execute_time,
                 long m_end_time,
                 int m_task_mode,
                 std::string m_task_id,
                 WorkStatus m_work_status,
                 int m_clean_area,
                 int m_clean_time,
                 int m_error_code,
                 std::string m_error_message,
                 int tasktype,

                 bool renew,
                 std::string old_task_id,
                 long new_task_id);

    virtual ~CleanHistory();

    const bool &getIsComplete() const;

    void setIsComplete(const bool &is_complete);

    const long &getLaunchTime() const;

    void setLaunchTime(const long &launch_time);

    const long &getExecuteTime() const;

    void setExecuteTime(const long &execute_time);

    const long &getEndTime() const;

    void setEndTime(const long &end_time);

    const int &getTaskMode() const;

    void setTaskMode(const int &task_mode);

    const std::string &getTaskID() const;

    void setTaskID(const std::string &task_id);

    const WorkStatus &getWorkStatus() const;

    void setWorkStatus(const WorkStatus &work_status);

    const int &getCleanArea() const;

    void setCleanArea(const int &clean_time);

    const int &getCleanTime() const;

    void setCleanTime(const int &clean_time);

    void setErrorCode(const int &error_code) {
        m_error_code = error_code;
    }

    const int &getErrorCode() const {
        return m_error_code;
    }

    const std::string &getErrorMessage() const {
        return m_error_message;
    }

    void setErrorMessage(const std::string &error_message) {
        m_error_message = error_message;
    }

    const int &gettask_type() const {
        return m_task_type;
    }

    void settask_type(const int &type) {
        m_task_type = type;
    }

    // Ewen change begin
    std::vector<int> getoper_event() const {
        // Ewen change end
        return m_oper_event;
    }

    // Ewen change begin
    void setoper_event(std::vector<int> &oe) {
        // Ewen change end 
        m_oper_event = oe;
    }

    const bool &getBaseComplete() const {
        return m_base_complete;
    }

    void setBaseComplete(const bool &base_complete) {
        m_base_complete = base_complete;
    }
};

class CleanHistoryList {//清洁历史列表，储存用
private:
    std::vector<CleanHistory> m_clean_history_list;

public:
    friend void to_json(json &j, const CleanHistoryList &b) {
        j = json{
                {"m_clean_history_list", b.m_clean_history_list},
        };
    }

    friend void from_json(const json &j, CleanHistoryList &b) {
        j.at("m_clean_history_list").get_to(b.m_clean_history_list);
    }

    CleanHistoryList();

    ~CleanHistoryList();

};


#endif//APP_COMMUNICATION_CLEANHISTORY_H
