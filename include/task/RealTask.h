//
// Created by Looper on 2022/11/5.
//

#ifndef APP_COMMUNICATION_REALTASK_H
#define APP_COMMUNICATION_REALTASK_H

#include "string"
#include "model/WorkStatus.h"
#include "RealBlock.h"
#include "RealPoint.h"
#include "model/RoomVo.h"
#include "model/task.h"

class RealTask {
private:
    std::string id;

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

    std::string time_mode;

    std::string source;//创建任务的源头
    std::string launch_people;//任务创建人
    long launch_time;//任务创建时间

    std::string on_source;//执行任务的源头

    int totalStep;
    int totalFrequency;

    std::vector<RealBlock> planBlocks;
    std::vector<RealBlock> realBlocks;

    bool verify_mode = false;

public:
    RealTask() = default;

    const std::string &getId() const {
        return id;
    }

    void setId(const std::string &id) {
        RealTask::id = id;
    }

    const std::string &getMapId() const {
        return map_id;
    }

    void setMapId(const std::string &mapId) {
        map_id = mapId;
    }

    long getTaskId() const {
        return task_id;
    }

    void setTaskId(long taskId) {
        task_id = taskId;
    }

    const std::string &getName() const {
        return name;
    }

    void setName(const std::string &name) {
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

    const std::vector<ZoneVo> &getZoned() const {
        return zoned;
    }

    void setZoned(const std::vector<ZoneVo> &zoned) {
        RealTask::zoned = zoned;
    }

    bool isPartition() const {
        return partition;
    }

    void setPartition(bool partition) {
        RealTask::partition = partition;
    }

    const std::vector<SubregionVo> &getSubregions() const {
        return subregions;
    }

    void setSubregions(const std::vector<SubregionVo> &subregions) {
        RealTask::subregions = subregions;
    }

    bool isKnife() const {
        return knife;
    }

    void setKnife(bool knife) {
        RealTask::knife = knife;
    }

    const std::string &getTimeMode() const {
        return time_mode;
    }

    void setTimeMode(const std::string &timeMode) {
        time_mode = timeMode;
    }

    const std::string &getSource() const {
        return source;
    }

    void setSource(const std::string &source) {
        RealTask::source = source;
    }

    const std::string &getLaunchPeople() const {
        return launch_people;
    }

    void setLaunchPeople(const std::string &launchPeople) {
        launch_people = launchPeople;
    }

    long getLaunchTime() const {
        return launch_time;
    }

    void setLaunchTime(long launchTime) {
        launch_time = launchTime;
    }

    const std::string &getOnSource() const {
        return on_source;
    }

    void setOnSource(const std::string &onSource) {
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

    const std::vector<RealBlock> &getPlanBlocks() const {
        return planBlocks;
    }

    void setPlanPoints(const std::vector<RealBlock> &planBlocks) {
        RealTask::planBlocks = planBlocks;
    }

    const std::vector<RealBlock> &getRealBlocks() const {
        return realBlocks;
    }

    void setRealPoints(const std::vector<RealBlock> &realBlocks) {
        RealTask::realBlocks = realBlocks;
    }

    bool isVerifyMode() const {
        return verify_mode;
    }

    void setVerifyMode(bool verifyMode) {
        verify_mode = verifyMode;
    }

    void assignmentPoint(RealBlock &block, int blockId) const {
        block.id = blockId;
        block.newTaskId = getTaskId();
    }

    void changeArrivalStatus(const RealBlock &block) {
        bool isContains;
        for (auto &realBlock: realBlocks) {
            if (realBlock.id == block.id) {
                realBlock.timely_step = block.timely_step;
                realBlock.already_step = block.already_step;
                isContains = true;
                if (realBlock.arrive) {
                    break;
                } else {
                    if (block.arrive) {
                        realBlock.arrive = block.arrive;
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
