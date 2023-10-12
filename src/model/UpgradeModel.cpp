//
// Created by mi on 2022/8/1.
//

#include "model/UpgradeModel.h"

WorkStatusUpgrade::WorkStatusUpgrade(int sweepStatus, int dragStatus, int absorbStatus, int pushStatus, int aromStatus)
        : sweep_status(
        sweepStatus), drag_status(dragStatus), absorb_status(absorbStatus), push_status(pushStatus),
          aromatherapy_status(aromStatus) {}

WorkStatusUpgrade::WorkStatusUpgrade() {}

int WorkStatusUpgrade::getSweepStatus() const {
    return sweep_status;
}

void WorkStatusUpgrade::setSweepStatus(int sweepStatus) {
    sweep_status = sweepStatus;
}

int WorkStatusUpgrade::getDragStatus() const {
    return drag_status;
}

void WorkStatusUpgrade::setDragStatus(int dragStatus) {
    drag_status = dragStatus;
}

int WorkStatusUpgrade::getAbsorbStatus() const {
    return absorb_status;
}

void WorkStatusUpgrade::setAbsorbStatus(int absorbStatus) {
    absorb_status = absorbStatus;
}

int WorkStatusUpgrade::getPushStatus() const {
    return push_status;
}

void WorkStatusUpgrade::setPushStatus(int pushStatus) {
    push_status = pushStatus;
}

int WorkStatusUpgrade::getAromatherapyStatus() {
    return aromatherapy_status;
}

void WorkStatusUpgrade::setAromatherapyStatus(int st) {
    aromatherapy_status = st;
}


bool CleanHistoryUpgrade::isBaseComplete() const {
    return m_base_complete;
}

int CleanHistoryUpgrade::getTaskType() const {
    return task_type;
}

std::vector<int> CleanHistoryUpgrade::getOperEvent() const {
    return oper_event;
}

bool CleanHistoryUpgrade::isComplete() const {
    return is_complete;
}

void CleanHistoryUpgrade::setIsComplete(bool isComplete) {
    is_complete = isComplete;
}

long CleanHistoryUpgrade::getLaunchTime() const {
    return launch_time;
}

void CleanHistoryUpgrade::setLaunchTime(long launchTime) {
    launch_time = launchTime;
}

long CleanHistoryUpgrade::getExecuteTime() const {
    return execute_time;
}

void CleanHistoryUpgrade::setExecuteTime(long executeTime) {
    execute_time = executeTime;
}

long CleanHistoryUpgrade::getEndTime() const {
    return end_time;
}

void CleanHistoryUpgrade::setEndTime(long endTime) {
    end_time = endTime;
}

int CleanHistoryUpgrade::getTaskMode() const {
    return task_mode;
}

void CleanHistoryUpgrade::setTaskMode(int taskMode) {
    task_mode = taskMode;
}

const std::string &CleanHistoryUpgrade::getTaskId() const {
    return task_id;
}

void CleanHistoryUpgrade::setTaskId(const std::string &taskId) {
    task_id = taskId;
}

int CleanHistoryUpgrade::getCleanArea() const {
    return clean_area;
}

void CleanHistoryUpgrade::setCleanArea(int cleanArea) {
    clean_area = cleanArea;
}

int CleanHistoryUpgrade::getCleanTime() const {
    return clean_time;
}

void CleanHistoryUpgrade::setCleanTime(int cleanTime) {
    clean_time = cleanTime;
}

int CleanHistoryUpgrade::getErrorCode() const {
    return error_code;
}

void CleanHistoryUpgrade::setErrorCode(int errorCode) {
    error_code = errorCode;
}

const std::string &CleanHistoryUpgrade::getErrorMessage() const {
    return error_message;
}

void CleanHistoryUpgrade::setErrorMessage(const std::string &errorMessage) {
    error_message = errorMessage;
}

CleanHistoryUpgrade::CleanHistoryUpgrade() {}

CleanHistoryUpgrade::CleanHistoryUpgrade(bool isComplete, long launchTime, long executeTime, long endTime, int taskMode,
                                         const std::string &taskId, int cleanArea,
                                         int cleanTime, int errorCode, const std::string errorCode2,
                                         const std::string &errorMessage, bool mBaseComplete,
                                         int taskType, const std::vector<std::string> &componentIdList,
                                         int skipPathPointCount, const std::vector<int> &operEvent, int rate,
                                         int sweepStatus, int mopStatus, int vacuumStatus, int pushStatus,
                                         int aromatherapyStatus, int disinfectStatus, int currentStep,
                                         int currentFrquency, int totalStep, int totalFrequency, int historyState,
                                         int currentFlow, int urgencyStop, int pauseNum, int manualBack,
                                         int lowPowerBack, int forceBack, int outStation, int endSleep,
                                         int backBaseRetries, int backBasePointArrived, int stationArrived,
                                         int rechargeRetries, int closeMechanism, int openMechanism,
                                         bool renew, const std::string &old_task_id, long new_task_id) : is_complete(
        isComplete), launch_time(launchTime), execute_time(executeTime), end_time(endTime), task_mode(taskMode),
                                                                                                         task_id(taskId),
                                                                                                         clean_area(
                                                                                                                 cleanArea),
                                                                                                         clean_time(
                                                                                                                 cleanTime),
                                                                                                         error_code(
                                                                                                                 errorCode),
                                                                                                         error_code2(
                                                                                                                 errorCode2),
                                                                                                         error_message(
                                                                                                                 errorMessage),
                                                                                                         m_base_complete(
                                                                                                                 mBaseComplete),
                                                                                                         task_type(
                                                                                                                 taskType),
                                                                                                         component_id_list_(
                                                                                                                 componentIdList),
                                                                                                         skip_path_point_count_(
                                                                                                                 skipPathPointCount),
                                                                                                         oper_event(
                                                                                                                 operEvent),
                                                                                                         rate_(rate),
                                                                                                         sweep_status_(
                                                                                                                 sweepStatus),
                                                                                                         mop_status_(
                                                                                                                 mopStatus),
                                                                                                         vacuum_status_(
                                                                                                                 vacuumStatus),
                                                                                                         push_status_(
                                                                                                                 pushStatus),
                                                                                                         aromatherapy_status_(
                                                                                                                 aromatherapyStatus),
                                                                                                         disinfect_status_(
                                                                                                                 disinfectStatus),
                                                                                                         current_step_(
                                                                                                                 currentStep),
                                                                                                         current_frquency_(
                                                                                                                 currentFrquency),
                                                                                                         total_step_(
                                                                                                                 totalStep),
                                                                                                         total_frequency_(
                                                                                                                 totalFrequency),
                                                                                                         history_state_(
                                                                                                                 historyState),
                                                                                                         current_flow_(
                                                                                                                 currentFlow),
                                                                                                         urgency_stop_(
                                                                                                                 urgencyStop),
                                                                                                         pause_num_(
                                                                                                                 pauseNum),
                                                                                                         manual_back_(
                                                                                                                 manualBack),
                                                                                                         low_power_back_(
                                                                                                                 lowPowerBack),
                                                                                                         force_back_(
                                                                                                                 forceBack),
                                                                                                         out_station_(
                                                                                                                 outStation),
                                                                                                         end_sleep_(
                                                                                                                 endSleep),
                                                                                                         back_base_retries_(
                                                                                                                 backBaseRetries),
                                                                                                         back_base_point_arrived_(
                                                                                                                 backBasePointArrived),
                                                                                                         station_arrived_(
                                                                                                                 stationArrived),
                                                                                                         recharge_retries_(
                                                                                                                 rechargeRetries),
                                                                                                         close_mechanism_(
                                                                                                                 closeMechanism),
                                                                                                         open_mechanism_(
                                                                                                                 openMechanism),
                                                                                                         renew(
                                                                                                                 renew),
                                                                                                         old_task_id(
                                                                                                                 old_task_id),
                                                                                                         new_task_id(
                                                                                                                 new_task_id) {}
