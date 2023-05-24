//
// Created by Looper on 2022/11/5.
//

#ifndef APP_COMMUNICATION_REALTASK_H
#define APP_COMMUNICATION_REALTASK_H

#include "string"
#include "model/WorkStatus.h"
#include "model/Task.h"
#include "RealBlock.h"
#include "RealPoint.h"
#include "model/RoomVo.h"
#include "model/task.h"

class RealTask {
private:
    std::string id;

    bool renew;//新旧任务标志位

    std::string map_id;//当前任务所在的地图
    long task_id;//关联的新任务id
    std::string name;//任务名称
    int rate;//任务次数
    int mode;//任何模式，注意新旧的区分

    WorkStatus work_status;//清洁机构状态

    bool principal;//此任务是否为默认任务

    std::vector<ZoneVo> zoned;//新任务划区
    bool partition;//新任务覆盖类型，true表示分区全覆盖，false表示全覆盖
    std::vector<SubregionVo> subregions;//新任务区域
    bool knife{false};//风刀开关

    std::vector<float> zoned0;//app手动划区
    Combination combination;//组合任务
    int combination_type;//全覆盖 or 组合
    std::string time_mode;

    std::string source;//创建任务的源头
    std::string launch_people;//任务创建人
    long launch_time;//任务创建时间

    std::string on_source;//执行任务的源头

    int totalStep;
    int totalFrequency;

    std::vector<RealBlock> planBlocks;
    std::vector<RealBlock> realBlocks;

public:
    RealTask() = default;

    const string &getId() const {
        return id;
    }

    void setId(const string &id) {
        RealTask::id = id;
    }

    bool isRenew() const {
        return renew;
    }

    void setRenew(bool renew) {
        RealTask::renew = renew;
    }

    const string &getMapId() const {
        return map_id;
    }

    void setMapId(const string &mapId) {
        map_id = mapId;
    }

    long getTaskId() const {
        return task_id;
    }

    void setTaskId(long taskId) {
        task_id = taskId;
    }

    const string &getName() const {
        return name;
    }

    void setName(const string &name) {
        RealTask::name = name;
    }

    int getRate() const {
        return rate;
    }

    void setRate(int rate) {
        RealTask::rate = rate;
    }

    int getMode() const {
        return mode;
    }

    void setMode(int mode) {
        RealTask::mode = mode;
    }

    const WorkStatus &getWorkStatus() const {
        return work_status;
    }

    void setWorkStatus(const WorkStatus &workStatus) {
        work_status = workStatus;
    }

    bool isPrincipal() const {
        return principal;
    }

    void setPrincipal(bool principal) {
        RealTask::principal = principal;
    }

    const vector<ZoneVo> &getZoned() const {
        return zoned;
    }

    void setZoned(const vector<ZoneVo> &zoned) {
        RealTask::zoned = zoned;
    }

    bool isPartition() const {
        return partition;
    }

    void setPartition(bool partition) {
        RealTask::partition = partition;
    }

    const vector<SubregionVo> &getSubregions() const {
        return subregions;
    }

    void setSubregions(const vector<SubregionVo> &subregions) {
        RealTask::subregions = subregions;
    }

    bool isKnife() const {
        return knife;
    }

    void setKnife(bool knife) {
        RealTask::knife = knife;
    }

    const vector<float> &getZoned0() const {
        return zoned0;
    }

    void setZoned0(const vector<float> &zoned0) {
        RealTask::zoned0 = zoned0;
    }

    const Combination &getCombination() const {
        return combination;
    }

    void setCombination(const Combination &combination) {
        RealTask::combination = combination;
    }

    int getCombinationType() const {
        return combination_type;
    }

    void setCombinationType(int combinationType) {
        combination_type = combinationType;
    }

    const string &getTimeMode() const {
        return time_mode;
    }

    void setTimeMode(const string &timeMode) {
        time_mode = timeMode;
    }

    const string &getSource() const {
        return source;
    }

    void setSource(const string &source) {
        RealTask::source = source;
    }

    const string &getLaunchPeople() const {
        return launch_people;
    }

    void setLaunchPeople(const string &launchPeople) {
        launch_people = launchPeople;
    }

    long getLaunchTime() const {
        return launch_time;
    }

    void setLaunchTime(long launchTime) {
        launch_time = launchTime;
    }

    const string &getOnSource() const {
        return on_source;
    }

    void setOnSource(const string &onSource) {
        on_source = onSource;
    }

    int getTotalStep() const {
        return totalStep;
    }

    void setTotalStep(int totalStep) {
        RealTask::totalStep = totalStep;
    }

    int getTotalFrequency() const {
        return totalFrequency;
    }

    void setTotalFrequency(int totalFrequency) {
        RealTask::totalFrequency = totalFrequency;
    }

    const vector<RealBlock> &getPlanPoints() const {
        return planBlocks;
    }

    void setPlanPoints(const vector<RealBlock> &planPoints) {
        RealTask::planBlocks = planPoints;
    }

    const vector<RealBlock> &getRealPoints() const {
        return realBlocks;
    }

    void setRealPoints(const vector<RealBlock> &realPoints) {
        RealTask::realBlocks = realPoints;
    }

    void assignmentPoint(RealBlock &block, int pointId) const {
        block.id = pointId;
        if (isRenew()) {
            block.newTaskId = getTaskId();
        } else {
            block.oldTaskId = getId();
        }
    }

    void changeArrivalStatus(const RealBlock &block) {
        bool isContains;
        for (auto realPoint: realBlocks) {
            if (realPoint.id == block.id) {
                isContains = true;
                if (realPoint.arrive) {
                    break;
                } else {
                    if (block.arrive) {
                        realPoint.arrive = block.arrive;
                    }
                }
            }
        }
        if (!isContains) {
            realBlocks.push_back(block);
        }

    }

};

#endif //APP_COMMUNICATION_REALTASK_H
