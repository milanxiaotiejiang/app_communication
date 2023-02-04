//
// Created by mi on 2022/8/1.
//

#ifndef APP_COMMUNICATION_UPGRADEMODEL_H
#define APP_COMMUNICATION_UPGRADEMODEL_H

#include <nlohmann/json.hpp>
#include "model/Point.h"
#include <CleanContinuity.h>
#include <CleanPolygon.h>
#include "TeachModePoint.h"
#include "FullPath.h"
#include "Task.h"

using namespace std;

using json = nlohmann::json;

class CombinationBriefUpgrade {
private:
    std::string combination_id;
    std::string name;
    int rate;
    std::vector<string> part_id_list;
    int combination_type; //0--combination;1--full clean

public:
    CombinationBriefUpgrade();

    CombinationBriefUpgrade(const string &combinationId, const string &name, int rate,
                            const vector<string> &partIdList);

    const string &getCombinationId() const;

    void setCombinationId(const string &combinationId);

    const string &getName() const;

    void setName(const string &name);

    int getRate() const;

    void setRate(int rate);

    const int &getCombinationType() const;

    void setCombinationType(int type);

    const vector<string> &getPartIdList() const;

    void setPartIdList(const vector<string> &partIdList);

    friend void to_json(json &j, const CombinationBriefUpgrade &model) {
        j = json{
                {"combination_id", model.combination_id},
                {"name",           model.name},
                {"rate",           model.rate},
                {"part_id_list",   model.part_id_list}
        };
    }

    friend void from_json(const json &j, CombinationBriefUpgrade &model) {
        j.at("combination_id").get_to(model.combination_id);
        j.at("name").get_to(model.name);
        j.at("rate").get_to(model.rate);
        j.at("part_id_list").get_to(model.part_id_list);
    }
};

//class CombinationBriefUpgradeList {//简化路径组合列表
//private:
//    std::vector<CombinationBriefUpgrade> m_combination_brief_list;
//public:
//    CombinationBriefUpgradeList();
//
//    CombinationBriefUpgradeList(const vector<CombinationBriefUpgrade> &mCombinationBriefList);
//
//    const vector<CombinationBriefUpgrade> &getMCombinationBriefList() const;
//
//    void setMCombinationBriefList(const vector<CombinationBriefUpgrade> &mCombinationBriefList);
//
//    friend void to_json(json &j, const CombinationBriefUpgradeList &b) {
//        j = json{
//                {"m_combination_brief_list", b.m_combination_brief_list},
//        };
//    }
//
//    friend void from_json(const json &j, CombinationBriefUpgradeList &b) {
//        j.at("m_combination_brief_list").get_to(b.m_combination_brief_list);
//    }
//};

class WorkStatusUpgrade {
private:
    int sweep_status;
    int drag_status;
    int absorb_status;
    int push_status;
    int aromatherapy_status;
public:
    WorkStatusUpgrade();

    WorkStatusUpgrade(int sweepStatus, int dragStatus, int absorbStatus, int pushStatus, int aromStatus);

    int getSweepStatus() const;

    void setSweepStatus(int sweepStatus);

    int getDragStatus() const;

    void setDragStatus(int dragStatus);

    int getAbsorbStatus() const;

    void setAbsorbStatus(int absorbStatus);

    int getPushStatus() const;

    void setPushStatus(int pushStatus);

    int getAromatherapyStatus();

    void setAromatherapyStatus(int st);

    std::string getPushMsg() const;

    void setPushMsg(std::string ss);

    std::string getSweepMsg() const;

    void setSweepMsg(std::string ss);

    friend void to_json(json &j, const WorkStatusUpgrade &b) {
        j = json{
                {"sweep_status",        b.sweep_status},
                {"drag_status",         b.drag_status},
                {"absorb_status",       b.absorb_status},
                {"push_status",         b.push_status},
                {"aromatherapy_status", b.aromatherapy_status}
        };
    }

    friend void from_json(const json &j, WorkStatusUpgrade &b) {
        j.at("sweep_status").get_to(b.sweep_status);
        j.at("drag_status").get_to(b.drag_status);
        j.at("absorb_status").get_to(b.absorb_status);
        j.at("push_status").get_to(b.push_status);
        j.at("aromatherapy_status").get_to(b.aromatherapy_status);
    }
};

class ViewPartUpgrade {
private:
    int mode;
    std::string name;
    std::string part_id;
    std::vector<Point> polygon;
    int rate;
    std::vector<Point> teach;
    std::string path_id;
    WorkStatusUpgrade work_status;
    std::vector<Point> zoned;
public:

    ViewPartUpgrade();

    ViewPartUpgrade(int mode, const string &name, const string &partId, const vector<Point> &polygon, int rate,
                    const vector<Point> &teach, const string &pathId, const WorkStatusUpgrade &workStatus,
                    const vector<Point> &zoned);

    int getMode() const;

    void setMode(int mode);

    const string &getName() const;

    void setName(const string &name);

    const string &getPartId() const;

    void setPartId(const string &partId);

    const vector<Point> &getPolygon() const;

    void setPolygon(const vector<Point> &polygon);

    int getRate() const;

    void setRate(int rate);

    const vector<Point> &getTeach() const;

    void setTeach(const vector<Point> &teach);

    const string &getPathId() const;

    void setPathId(const string &pathId);

    const WorkStatusUpgrade &getWorkStatus() const;

    void setWorkStatus(const WorkStatusUpgrade &workStatus);

    const vector<Point> &getZoned() const;

    void setZoned(const vector<Point> &zoned);

    friend void to_json(json &j, const ViewPartUpgrade &b) {
        j = json{
                {"mode",        b.mode},
                {"name",        b.name},
                {"part_id",     b.part_id},
                {"polygon",     b.polygon},
                {"rate",        b.rate},
                {"teach",       b.teach},
                {"path_id",     b.path_id},
                {"work_status", b.work_status},
                {"zoned",       b.zoned}};
    }

    friend void from_json(const json &j, ViewPartUpgrade &b) {
        j.at("mode").get_to(b.mode);
        j.at("name").get_to(b.name);
        j.at("part_id").get_to(b.part_id);
        j.at("polygon").get_to(b.polygon);
        j.at("rate").get_to(b.rate);
        j.at("teach").get_to(b.teach);
        j.at("path_id").get_to(b.path_id);
        j.at("work_status").get_to(b.work_status);
        j.at("zoned").get_to(b.zoned);
    }
};

//class ViewPartListUpgrade {
//private:
//    std::vector<ViewPartUpgrade> m_view_part_list;
//public:
//    ViewPartListUpgrade();
//
//    ViewPartListUpgrade(const vector<ViewPartUpgrade> &mViewPartList);
//
//    const vector<ViewPartUpgrade> &getMViewPartList() const;
//
//    void setMViewPartList(const vector<ViewPartUpgrade> &mViewPartList);
//
//    friend void to_json(json &j, const ViewPartListUpgrade &b) {
//        j = json{
//                {"m_view_part_list", b.m_view_part_list},
//        };
//    }
//
//    friend void from_json(const json &j, ViewPartListUpgrade &b) {
//        j.at("m_view_part_list").get_to(b.m_view_part_list);
//    }
//};

class CleanHistoryUpgrade {
private:
    bool is_complete;
    long launch_time;
    long execute_time;
    long end_time;
    int task_mode;
    string task_id;
    int clean_area;
    int clean_time;
    int error_code;
    string error_code2;
    string error_message;

    // Ewen change begin
    bool m_base_complete;
    int task_type;
    vector<std::string> component_id_list_;
    int skip_path_point_count_;
    vector<int> oper_event;
    int rate_;
    int sweep_status_;
    int mop_status_;
    int vacuum_status_;
    int push_status_;
    int aromatherapy_status_;
    int disinfect_status_;

    int current_step_;
    int current_frquency_;
    int total_step_;
    int total_frequency_;
    int history_state_;
    int current_flow_;
    int urgency_stop_;
    int pause_num_;
    int manual_back_;
    int low_power_back_;
    int force_back_;

    int out_station_;
    int end_sleep_;
    int back_base_retries_;
    int back_base_point_arrived_;
    int station_arrived_;
    int recharge_retries_;
    int close_mechanism_;
    int open_mechanism_;

public:
    CleanHistoryUpgrade();

    CleanHistoryUpgrade(bool isComplete, long launchTime, long executeTime, long endTime, int taskMode,
                        const string &taskId, int cleanArea, int cleanTime,
                        int errorCode, const string errorCode2, const string &errorMessage, bool mBaseComplete,
                        int taskType,
                        const vector<std::string> &componentIdList, int skipPathPointCount,
                        const vector<int> &operEvent, int rate, int sweepStatus, int mopStatus, int vacuumStatus,
                        int pushStatus, int aromatherapyStatus, int disinfectStatus, int currentStep,
                        int currentFrquency, int totalStep, int totalFrequency, int historyState, int currentFlow,
                        int urgencyStop, int pauseNum, int manualBack, int lowPowerBack, int forceBack, int outStation,
                        int endSleep, int backBaseRetries, int backBasePointArrived, int stationArrived,
                        int rechargeRetries, int closeMechanism, int openMechanism);

    bool isBaseComplete() const;

    int getTaskType() const;

    vector<int> getOperEvent() const;

    // Ewen change end
    bool isComplete() const;

    void setIsComplete(bool isComplete);

    long getLaunchTime() const;

    void setLaunchTime(long launchTime);

    long getExecuteTime() const;

    void setExecuteTime(long executeTime);

    long getEndTime() const;

    void setEndTime(long endTime);

    int getTaskMode() const;

    void setTaskMode(int taskMode);

    const string &getTaskId() const;

    void setTaskId(const string &taskId);

    const WorkStatusUpgrade &getWorkStatus() const;

    void setWorkStatus(const WorkStatusUpgrade &workStatus);

    int getCleanArea() const;

    void setCleanArea(int cleanArea);

    int getCleanTime() const;

    void setCleanTime(int cleanTime);

    int getErrorCode() const;

    void setErrorCode(int errorCode);

    const string &getErrorMessage() const;

    void setErrorMessage(const string &errorMessage);

    friend void to_json(json &j, const CleanHistoryUpgrade &b) {
        j = json{
                {"is_complete",             b.is_complete},
                {"launch_time",             b.launch_time},
                {"execute_time",            b.execute_time},
                {"end_time",                b.end_time},
                {"task_mode",               b.task_mode},
                {"task_id",                 b.task_id},
                {"clean_area",              b.clean_area},
                {"clean_time",              b.clean_time},
                {"error_code",              b.error_code},
                {"error_code2",             b.error_code2},
                {"error_message",           b.error_message},

                {"base_complete",           b.m_base_complete},
                {"task_type",               b.task_type},
                {"oper_event",              b.oper_event},
                {"component_id_list",       b.component_id_list_},
                {"skip_path_point_count",   b.skip_path_point_count_},
                {"rate",                    b.rate_},
                {"sweep_status",            b.sweep_status_},
                {"mop_status",              b.mop_status_},
                {"vacuum_status",           b.vacuum_status_},
                {"push_status",             b.push_status_},
                {"aromatherapy_status",     b.aromatherapy_status_},
                {"disinfect_status",        b.disinfect_status_},

                {"current_step",            b.current_step_},
                {"current_frquency",        b.current_frquency_},
                {"total_step",              b.total_step_},
                {"total_frequency",         b.total_frequency_},
                {"history_state",           b.history_state_},
                {"current_flow",            b.current_flow_},
                {"urgency_stop",            b.urgency_stop_},
                {"pause_num",               b.pause_num_},
                {"manual_back",             b.manual_back_},
                {"low_power_back",          b.low_power_back_},
                {"force_back",              b.force_back_},

                {"out_station",             b.out_station_},
                {"end_sleep",               b.end_sleep_},
                {"back_base_retries",       b.back_base_retries_},
                {"back_base_point_arrived", b.back_base_point_arrived_},
                {"station_arrived",         b.station_arrived_},
                {"recharge_retries",        b.recharge_retries_},
                {"close_mechanism",         b.close_mechanism_},
                {"open_mechanism",          b.open_mechanism_}};
    }

    friend void from_json(const json &j, CleanHistoryUpgrade &b) {
        j.at("is_complete").get_to(b.is_complete);
        j.at("launch_time").get_to(b.launch_time);
        j.at("execute_time").get_to(b.execute_time);
        j.at("end_time").get_to(b.end_time);
        j.at("task_mode").get_to(b.task_mode);
        j.at("task_id").get_to(b.task_id);
        j.at("clean_area").get_to(b.clean_area);
        j.at("clean_time").get_to(b.clean_time);
        j.at("error_code").get_to(b.error_code);
        j.at("error_code2").get_to(b.error_code2);
        j.at("error_message").get_to(b.error_message);

        j.at("base_complete").get_to(b.m_base_complete);
        j.at("task_type").get_to(b.task_type);
        j.at("oper_event").get_to(b.oper_event);
        j.at("component_id_list").get_to(b.component_id_list_);
        j.at("skip_path_point_count").get_to(b.skip_path_point_count_);
        j.at("rate").get_to(b.rate_);
        j.at("sweep_status").get_to(b.sweep_status_);
        j.at("mop_status").get_to(b.mop_status_);
        j.at("vacuum_status").get_to(b.vacuum_status_);
        j.at("push_status").get_to(b.push_status_);
        j.at("aromatherapy_status").get_to(b.aromatherapy_status_);
        j.at("disinfect_status").get_to(b.disinfect_status_);

        j.at("current_step").get_to(b.current_step_);
        j.at("current_frquency").get_to(b.current_frquency_);
        j.at("total_step").get_to(b.total_step_);
        j.at("total_frequency").get_to(b.total_frequency_);
        j.at("history_state").get_to(b.history_state_);
        j.at("current_flow").get_to(b.current_flow_);
        j.at("urgency_stop").get_to(b.urgency_stop_);
        j.at("pause_num").get_to(b.pause_num_);
        j.at("manual_back").get_to(b.manual_back_);
        j.at("low_power_back").get_to(b.low_power_back_);
        j.at("force_back").get_to(b.force_back_);

        j.at("out_station").get_to(b.out_station_);
        j.at("end_sleep").get_to(b.end_sleep_);
        j.at("back_base_retries").get_to(b.back_base_retries_);
        j.at("back_base_point_arrived").get_to(b.back_base_point_arrived_);
        j.at("station_arrived").get_to(b.station_arrived_);
        j.at("recharge_retries").get_to(b.recharge_retries_);
        j.at("close_mechanism").get_to(b.close_mechanism_);
        j.at("open_mechanism").get_to(b.open_mechanism_);
    }
};

class CleanHistoryListUpgrade {//清洁历史列表，储存用
private:
    vector<CleanHistoryUpgrade> m_clean_history_list;
public:
    CleanHistoryListUpgrade();

    CleanHistoryListUpgrade(const vector<CleanHistoryUpgrade> &mCleanHistoryList);

    const vector<CleanHistoryUpgrade> &getMCleanHistoryList() const;

    void setMCleanHistoryList(const vector<CleanHistoryUpgrade> &mCleanHistoryList);

    friend void to_json(json &j, const CleanHistoryListUpgrade &b) {
        j = json{
                {"m_clean_history_list", b.m_clean_history_list},
        };
    }

    friend void from_json(const json &j, CleanHistoryListUpgrade &b) {
        j.at("m_clean_history_list").get_to(b.m_clean_history_list);
    }
};

class TaskUpgrade {
private:
    string task_id;
    int mode;
    int rate;
    WorkStatusUpgrade work_status;
    CleanPolygon polygon;
    vector<float> zoned;
    CleanContinuity continuity;
    TeachPathInfo teach_path;
    std::string launch_people;
    long launch_time;
    std::string time_mode;
    bool in_execute;
    Combination combination;
    FullPath full_path;

public:
    TaskUpgrade();

    TaskUpgrade(const string &taskId, int mode, int rate, const WorkStatusUpgrade &workStatus,
                const CleanPolygon &polygon, const vector<float> &zoned, const CleanContinuity &continuity,
                const TeachPathInfo &teachPath, const string &launchPeople, long launchTime, const string &timeMode,
                bool inExecute, const Combination &combination, const FullPath &fullPath);

    const string &getTaskId() const;

    void setTaskId(const string &taskId);

    int getMode() const;

    void setMode(int mode);

    int getRate() const;

    void setRate(int rate);

    const WorkStatusUpgrade &getWorkStatus() const;

    void setWorkStatus(const WorkStatusUpgrade &workStatus);

    const CleanPolygon &getPolygon() const;

    void setPolygon(const CleanPolygon &polygon);

    const vector<float> &getZoned() const;

    void setZoned(const vector<float> &zoned);

    const CleanContinuity &getContinuity() const;

    void setContinuity(const CleanContinuity &continuity);

    const TeachPathInfo &getTeachPath() const;

    void setTeachPath(const TeachPathInfo &teachPath);

    const string &getLaunchPeople() const;

    void setLaunchPeople(const string &launchPeople);

    long getLaunchTime() const;

    void setLaunchTime(long launchTime);

    const string &getTimeMode() const;

    void setTimeMode(const string &timeMode);

    bool isInExecute() const;

    void setInExecute(bool inExecute);

    const Combination &getCombination() const;

    void setCombination(const Combination &combination);

    const FullPath &getFullPath() const;

    void setFullPath(const FullPath &fullPath);

    friend void to_json(json &j, const TaskUpgrade &b) {
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
                {"combination",   b.combination}};
    }

    friend void from_json(const json &j, TaskUpgrade &b) {
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
        j.at("combination").get_to(b.combination);
    }
};

class CombinationDetailUpgrade {
private:
    std::string combination_id;
    std::string name;
    int rate;
    std::vector<ViewPartUpgrade> view_part_list;
public:
    CombinationDetailUpgrade();

    CombinationDetailUpgrade(const string &combinationId, const string &name, int rate,
                             const vector<ViewPartUpgrade> &viewPartList);

    const string &getCombinationId() const;

    void setCombinationId(const string &combinationId);

    const string &getName() const;

    void setName(const string &name);

    int getRate() const;

    void setRate(int rate);

    const vector<ViewPartUpgrade> &getViewPartList() const;

    void setViewPartList(const vector<ViewPartUpgrade> &viewPartList);

    friend void to_json(json &j, const CombinationDetailUpgrade &b) {
        j = json{
                {"combination_id", b.combination_id},
                {"name",           b.name},
                {"rate",           b.rate},
                {"view_part_list", b.view_part_list}
        };
    }

    friend void from_json(const json &j, CombinationDetailUpgrade &b) {
        j.at("combination_id").get_to(b.combination_id);
        j.at("name").get_to(b.name);
        j.at("rate").get_to(b.rate);
        j.at("view_part_list").get_to(b.view_part_list);
    }
};


class CombinationBriefWork {//单个路径组合简表
private:
    std::string combination_id;
    std::string name;
    int rate;
    std::vector<string> part_id_list;
    bool principal{false};

public:

    CombinationBriefWork();

    CombinationBriefWork(const string &combinationId, const string &name, int rate, const vector<string> &partIdList,
                         bool principal);

    const string &getCombinationId() const;

    void setCombinationId(const string &combinationId);

    const string &getName() const;

    void setName(const string &name);

    int getRate() const;

    void setRate(int rate);

    const vector<string> &getPartIdList() const;

    void setPartIdList(const vector<string> &partIdList);

    bool isPrincipal() const;

    void setPrincipal(bool principal);

    friend void to_json(json &j, const CombinationBriefWork &b) {
        j = json{
                {"combination_id", b.combination_id},
                {"name",           b.name},
                {"rate",           b.rate},
                {"part_id_list",   b.part_id_list},
                {"principal",      b.principal}
        };
    }

    friend void from_json(const json &j, CombinationBriefWork &b) {
        j.at("combination_id").get_to(b.combination_id);
        j.at("name").get_to(b.name);
        j.at("rate").get_to(b.rate);
        j.at("part_id_list").get_to(b.part_id_list);
        j.at("principal").get_to(b.principal);
    }
};

class CombinationBriefWorkList {//简化路径组合列表
private:
    std::vector<CombinationBriefWork> m_combination_brief_list;

public:
    CombinationBriefWorkList();

    CombinationBriefWorkList(const vector<CombinationBriefWork> &mCombinationBriefList);

    const vector<CombinationBriefWork> &getMCombinationBriefList() const;

    void setMCombinationBriefList(const vector<CombinationBriefWork> &mCombinationBriefList);

    friend void to_json(json &j, const CombinationBriefWorkList &b) {
        j = json{
                {"m_combination_brief_list", b.m_combination_brief_list},
        };
    }

    friend void from_json(const json &j, CombinationBriefWorkList &b) {
        j.at("m_combination_brief_list").get_to(b.m_combination_brief_list);
    }
};

#endif //APP_COMMUNICATION_UPGRADEMODEL_H
