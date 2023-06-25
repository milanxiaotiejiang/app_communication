//
// Created by mi on 2022/8/1.
//

#include "model/UpgradeModel.h"

CombinationBriefUpgrade::CombinationBriefUpgrade(const std::string &combinationId, const std::string &name, int rate,
                                                 const std::vector<std::string> &partIdList) : combination_id(combinationId),
                                                                                     name(name), rate(rate),
                                                                                     part_id_list(partIdList) {}

const std::string &CombinationBriefUpgrade::getCombinationId() const {
    return combination_id;
}

void CombinationBriefUpgrade::setCombinationId(const std::string &combinationId) {
    combination_id = combinationId;
}

const std::string &CombinationBriefUpgrade::getName() const {
    return name;
}

void CombinationBriefUpgrade::setName(const std::string &name) {
    CombinationBriefUpgrade::name = name;
}

int CombinationBriefUpgrade::getRate() const {
    return rate;
}

void CombinationBriefUpgrade::setRate(int rate) {
    CombinationBriefUpgrade::rate = rate;
}

const int &CombinationBriefUpgrade::getCombinationType() const {
    return combination_type;
}

void CombinationBriefUpgrade::setCombinationType(int type) {
    combination_type = type;
}

const std::vector<std::string> &CombinationBriefUpgrade::getPartIdList() const {
    return part_id_list;
}

void CombinationBriefUpgrade::setPartIdList(const std::vector<std::string> &partIdList) {
    part_id_list = partIdList;
}

CombinationBriefUpgrade::CombinationBriefUpgrade() {}

//CombinationBriefUpgradeList::CombinationBriefUpgradeList(const vector<CombinationBriefUpgrade> &mCombinationBriefList)
//        : m_combination_brief_list(mCombinationBriefList) {}
//
//const vector<CombinationBriefUpgrade> &CombinationBriefUpgradeList::getMCombinationBriefList() const {
//    return m_combination_brief_list;
//}
//
//void
//CombinationBriefUpgradeList::setMCombinationBriefList(const vector<CombinationBriefUpgrade> &mCombinationBriefList) {
//    m_combination_brief_list = mCombinationBriefList;
//}
//
//CombinationBriefUpgradeList::CombinationBriefUpgradeList() {}

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

ViewPartUpgrade::ViewPartUpgrade() {}

ViewPartUpgrade::ViewPartUpgrade(int mode, const std::string &name, const std::string &partId, const std::vector<Point> &polygon,
                                 int rate, const std::vector<Point> &teach, const std::string &pathId,
                                 const WorkStatusUpgrade &workStatus, const std::vector<Point> &zoned) : mode(mode),
                                                                                                    name(name),
                                                                                                    part_id(partId),
                                                                                                    polygon(polygon),
                                                                                                    rate(rate),
                                                                                                    teach(teach),
                                                                                                    path_id(pathId),
                                                                                                    work_status(
                                                                                                            workStatus),
                                                                                                    zoned(zoned) {}

int ViewPartUpgrade::getMode() const {
    return mode;
}

void ViewPartUpgrade::setMode(int mode) {
    ViewPartUpgrade::mode = mode;
}

const std::string &ViewPartUpgrade::getName() const {
    return name;
}

void ViewPartUpgrade::setName(const std::string &name) {
    ViewPartUpgrade::name = name;
}

const std::string &ViewPartUpgrade::getPartId() const {
    return part_id;
}

void ViewPartUpgrade::setPartId(const std::string &partId) {
    part_id = partId;
}

const std::vector<Point> &ViewPartUpgrade::getPolygon() const {
    return polygon;
}

void ViewPartUpgrade::setPolygon(const std::vector<Point> &polygon) {
    ViewPartUpgrade::polygon = polygon;
}

int ViewPartUpgrade::getRate() const {
    return rate;
}

void ViewPartUpgrade::setRate(int rate) {
    ViewPartUpgrade::rate = rate;
}

const std::vector<Point> &ViewPartUpgrade::getTeach() const {
    return teach;
}

void ViewPartUpgrade::setTeach(const std::vector<Point> &teach) {
    ViewPartUpgrade::teach = teach;
}

const std::string &ViewPartUpgrade::getPathId() const {
    return path_id;
}

void ViewPartUpgrade::setPathId(const std::string &pathId) {
    path_id = pathId;
}

const WorkStatusUpgrade &ViewPartUpgrade::getWorkStatus() const {
    return work_status;
}

void ViewPartUpgrade::setWorkStatus(const WorkStatusUpgrade &workStatus) {
    work_status = workStatus;
}

const std::vector<Point> &ViewPartUpgrade::getZoned() const {
    return zoned;
}

void ViewPartUpgrade::setZoned(const std::vector<Point> &zoned) {
    ViewPartUpgrade::zoned = zoned;
}

//ViewPartListUpgrade::ViewPartListUpgrade() {}
//
//ViewPartListUpgrade::ViewPartListUpgrade(const vector<ViewPartUpgrade> &mViewPartList) : m_view_part_list(
//        mViewPartList) {}
//
//const vector<ViewPartUpgrade> &ViewPartListUpgrade::getMViewPartList() const {
//    return m_view_part_list;
//}
//
//void ViewPartListUpgrade::setMViewPartList(const vector<ViewPartUpgrade> &mViewPartList) {
//    m_view_part_list = mViewPartList;
//}

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
                                         int rechargeRetries, int closeMechanism, int openMechanism) : is_complete(
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
                                                                                                               openMechanism) {}

CleanHistoryListUpgrade::CleanHistoryListUpgrade(const std::vector<CleanHistoryUpgrade> &mCleanHistoryList)
        : m_clean_history_list(mCleanHistoryList) {}

const std::vector<CleanHistoryUpgrade> &CleanHistoryListUpgrade::getMCleanHistoryList() const {
    return m_clean_history_list;
}

void CleanHistoryListUpgrade::setMCleanHistoryList(const std::vector<CleanHistoryUpgrade> &mCleanHistoryList) {
    m_clean_history_list = mCleanHistoryList;
}

CleanHistoryListUpgrade::CleanHistoryListUpgrade() {}

const std::string &TaskUpgrade::getTaskId() const {
    return task_id;
}

void TaskUpgrade::setTaskId(const std::string &taskId) {
    task_id = taskId;
}

int TaskUpgrade::getMode() const {
    return mode;
}

void TaskUpgrade::setMode(int mode) {
    TaskUpgrade::mode = mode;
}

int TaskUpgrade::getRate() const {
    return rate;
}

void TaskUpgrade::setRate(int rate) {
    TaskUpgrade::rate = rate;
}

const WorkStatusUpgrade &TaskUpgrade::getWorkStatus() const {
    return work_status;
}

void TaskUpgrade::setWorkStatus(const WorkStatusUpgrade &workStatus) {
    work_status = workStatus;
}

const CleanPolygon &TaskUpgrade::getPolygon() const {
    return polygon;
}

void TaskUpgrade::setPolygon(const CleanPolygon &polygon) {
    TaskUpgrade::polygon = polygon;
}

const std::vector<float> &TaskUpgrade::getZoned() const {
    return zoned;
}

void TaskUpgrade::setZoned(const std::vector<float> &zoned) {
    TaskUpgrade::zoned = zoned;
}

const CleanContinuity &TaskUpgrade::getContinuity() const {
    return continuity;
}

void TaskUpgrade::setContinuity(const CleanContinuity &continuity) {
    TaskUpgrade::continuity = continuity;
}

const TeachPathInfo &TaskUpgrade::getTeachPath() const {
    return teach_path;
}

void TaskUpgrade::setTeachPath(const TeachPathInfo &teachPath) {
    teach_path = teachPath;
}

const std::string &TaskUpgrade::getLaunchPeople() const {
    return launch_people;
}

void TaskUpgrade::setLaunchPeople(const std::string &launchPeople) {
    launch_people = launchPeople;
}

long TaskUpgrade::getLaunchTime() const {
    return launch_time;
}

void TaskUpgrade::setLaunchTime(long launchTime) {
    launch_time = launchTime;
}

const std::string &TaskUpgrade::getTimeMode() const {
    return time_mode;
}

void TaskUpgrade::setTimeMode(const std::string &timeMode) {
    time_mode = timeMode;
}

bool TaskUpgrade::isInExecute() const {
    return in_execute;
}

void TaskUpgrade::setInExecute(bool inExecute) {
    in_execute = inExecute;
}

const FullPath &TaskUpgrade::getFullPath() const {
    return full_path;
}

void TaskUpgrade::setFullPath(const FullPath &fullPath) {
    full_path = fullPath;
}

TaskUpgrade::TaskUpgrade() {}

TaskUpgrade::TaskUpgrade(const std::string &taskId, int mode, int rate, const WorkStatusUpgrade &workStatus,
                         const CleanPolygon &polygon, const std::vector<float> &zoned, const CleanContinuity &continuity,
                         const TeachPathInfo &teachPath, const std::string &launchPeople, long launchTime,
                         const std::string &timeMode, bool inExecute, const Combination &combination,
                         const FullPath &fullPath) : task_id(taskId), mode(mode), rate(rate), work_status(workStatus),
                                                     polygon(polygon), zoned(zoned), continuity(continuity),
                                                     teach_path(teachPath), launch_people(launchPeople),
                                                     launch_time(launchTime), time_mode(timeMode),
                                                     in_execute(inExecute), combination(combination),
                                                     full_path(fullPath) {}

const Combination &TaskUpgrade::getCombination() const {
    return combination;
}

void TaskUpgrade::setCombination(const Combination &combination) {
    TaskUpgrade::combination = combination;
}

CombinationDetailUpgrade::CombinationDetailUpgrade() {}

CombinationDetailUpgrade::CombinationDetailUpgrade(const std::string &combinationId, const std::string &name, int rate,
                                                   const std::vector<ViewPartUpgrade> &viewPartList) : combination_id(
        combinationId), name(name), rate(rate), view_part_list(viewPartList) {}

const std::string &CombinationDetailUpgrade::getCombinationId() const {
    return combination_id;
}

void CombinationDetailUpgrade::setCombinationId(const std::string &combinationId) {
    combination_id = combinationId;
}

const std::string &CombinationDetailUpgrade::getName() const {
    return name;
}

void CombinationDetailUpgrade::setName(const std::string &name) {
    CombinationDetailUpgrade::name = name;
}

int CombinationDetailUpgrade::getRate() const {
    return rate;
}

void CombinationDetailUpgrade::setRate(int rate) {
    CombinationDetailUpgrade::rate = rate;
}

const std::vector<ViewPartUpgrade> &CombinationDetailUpgrade::getViewPartList() const {
    return view_part_list;
}

void CombinationDetailUpgrade::setViewPartList(const std::vector<ViewPartUpgrade> &viewPartList) {
    view_part_list = viewPartList;
}

CombinationBriefWork::CombinationBriefWork() {}

CombinationBriefWork::CombinationBriefWork(const std::string &combinationId, const std::string &name, int rate,
                                           const std::vector<std::string> &partIdList, bool principal) : combination_id(
        combinationId), name(name), rate(rate), part_id_list(partIdList), principal(principal) {}

const std::string &CombinationBriefWork::getCombinationId() const {
    return combination_id;
}

void CombinationBriefWork::setCombinationId(const std::string &combinationId) {
    combination_id = combinationId;
}

const std::string &CombinationBriefWork::getName() const {
    return name;
}

void CombinationBriefWork::setName(const std::string &name) {
    CombinationBriefWork::name = name;
}

int CombinationBriefWork::getRate() const {
    return rate;
}

void CombinationBriefWork::setRate(int rate) {
    CombinationBriefWork::rate = rate;
}

const std::vector<std::string> &CombinationBriefWork::getPartIdList() const {
    return part_id_list;
}

void CombinationBriefWork::setPartIdList(const std::vector<std::string> &partIdList) {
    part_id_list = partIdList;
}

bool CombinationBriefWork::isPrincipal() const {
    return principal;
}

void CombinationBriefWork::setPrincipal(bool principal) {
    CombinationBriefWork::principal = principal;
}

CombinationBriefWorkList::CombinationBriefWorkList() {}

CombinationBriefWorkList::CombinationBriefWorkList(const std::vector<CombinationBriefWork> &mCombinationBriefList)
        : m_combination_brief_list(mCombinationBriefList) {}

const std::vector<CombinationBriefWork> &CombinationBriefWorkList::getMCombinationBriefList() const {
    return m_combination_brief_list;
}

void CombinationBriefWorkList::setMCombinationBriefList(const std::vector<CombinationBriefWork> &mCombinationBriefList) {
    m_combination_brief_list = mCombinationBriefList;
}
