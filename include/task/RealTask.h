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

    std::vector<DeliveryVo> deliveries;//配送

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

    /**
     * 多地图逻辑
     * do_ 表示执行任务需要的地图
     * pre_ 表示切换前的地图，任务前的地图
     * base_ 表示含基站的地图，任务完成后需要回基站
     */
    bool async_map{false};//非当前地图
    long build_id;//当前任务所在的楼宇

    // 当任务地图没有基站时，需要查找切换前的地图是否有基站，切换前地图没有基站时，需要查找当前楼宇是否有基站。
    std::string do_map_id;
    int do_floor;
    RealPoint do_out_point;
    RealPoint do_in_point;

    std::string pre_map_id;
    int pre_floor;
    RealPoint pre_out_point;
    RealPoint pre_in_point;

    std::string post_map_id;
    int post_floor;
    RealPoint post_out_point;
    RealPoint post_in_point;

    int buildElevatorAddress;

    std::vector<RealBlock> proList;
    std::vector<RealBlock> postList;

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

    const std::vector<DeliveryVo> &getDeliveries() const {
        return deliveries;
    }

    void setDeliveries(const std::vector<DeliveryVo> &deliveries) {
        RealTask::deliveries = deliveries;
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

    bool isAsyncMap() const {
        return async_map;
    }

    void setAsyncMap(bool asyncMap) {
        async_map = asyncMap;
    }

    long getBuildId() const {
        return build_id;
    }

    void setBuildId(long buildId) {
        build_id = buildId;
    }

    const std::string &getDoMapId() const {
        return do_map_id;
    }

    void setDoMapId(const std::string &doMapId) {
        do_map_id = doMapId;
    }

    int getDoFloor() const {
        return do_floor;
    }

    void setDoFloor(int doFloor) {
        do_floor = doFloor;
    }

    const RealPoint &getDoOutPoint() const {
        return do_out_point;
    }

    void setDoOutPoint(const RealPoint &doOutPoint) {
        do_out_point = doOutPoint;
    }

    const std::string &getPreMapId() const {
        return pre_map_id;
    }

    void setPreMapId(const std::string &preMapId) {
        pre_map_id = preMapId;
    }

    int getPreFloor() const {
        return pre_floor;
    }

    void setPreFloor(int preFloor) {
        pre_floor = preFloor;
    }

    const RealPoint &getPreOutPoint() const {
        return pre_out_point;
    }

    void setPreOutPoint(const RealPoint &prePoint) {
        pre_out_point = prePoint;
    }

    const std::string &getPostMapId() const {
        return post_map_id;
    }

    void setPostMapId(const std::string &postMapId) {
        post_map_id = postMapId;
    }

    int getPostFloor() const {
        return post_floor;
    }

    void setPostFloor(int postFloor) {
        post_floor = postFloor;
    }

    const RealPoint &getPostOutPoint() const {
        return post_out_point;
    }

    void setPostOutPoint(const RealPoint &postPoint) {
        post_out_point = postPoint;
    }

    const RealPoint &getDoInPoint() const {
        return do_in_point;
    }

    void setDoInPoint(const RealPoint &doInPoint) {
        do_in_point = doInPoint;
    }

    const RealPoint &getPreInPoint() const {
        return pre_in_point;
    }

    void setPreInPoint(const RealPoint &preInPoint) {
        pre_in_point = preInPoint;
    }

    const RealPoint &getPostInPoint() const {
        return post_in_point;
    }

    void setPostInPoint(const RealPoint &postInPoint) {
        post_in_point = postInPoint;
    }

    const std::vector<RealBlock> &getProList() const {
        return proList;
    }

    void setProList(const std::vector<RealBlock> &proList) {
        RealTask::proList = proList;
    }

    const std::vector<RealBlock> &getPostList() const {
        return postList;
    }

    void setPostList(const std::vector<RealBlock> &postList) {
        RealTask::postList = postList;
    }

    int getBuildElevatorAddress() const {
        return buildElevatorAddress;
    }

    void setBuildElevatorAddress(int buildElevatorAddress) {
        RealTask::buildElevatorAddress = buildElevatorAddress;
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
