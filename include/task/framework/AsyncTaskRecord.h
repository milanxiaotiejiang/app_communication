//
// Created by Looper on 2023/2/6.
//

#ifndef APP_COMMUNICATION_ASYNCTASKRECORD_H
#define APP_COMMUNICATION_ASYNCTASKRECORD_H

#include "AsyncTaskFramework.h"
#include "task/model/TaskStack.h"

const int MAX_RECORD_TASK_STACK_SIZE = 3;

class AsyncTaskRecord : public AsyncTaskFramework {
protected:

    std::deque<TaskStack> stopStack;

    std::deque<RealTask> waitTaskQueue;
    std::deque<RealBlock> plannerQueue;
    std::vector<RealBlock> preConditions;
    std::vector<RealBlock> postConditions;

    RealTask runTask;

    RealBlock flowSeizeSeatPoint;
    RealBlock flowOpenMechanismPoint;
    RealBlock flowCloseMechanismPoint;
    RealBlock flowOutStationPoint;

    RealBlock flowEndSleepPoint;
    RealBlock flowInBasePoint;
    RealBlock flowInStationPoint;

    RealBlock flowElevatorPrePoint;
    RealBlock flowElevatorPostPoint;

    RealBlock flowReadyBackPoint;

    std::atomic<bool> mElevatorInside;

    std::string runId() const {
        return runTask.getId();
    }

    long runTaskId() const {
        return runTask.getTaskId();
    }

    WorkStatus baseWorkStatus() const {
        return runTask.getWorkStatus();
    }

    bool isKnife() const {
        return runTask.isKnife();
    }

    int baseTaskMode() const {
        return runTask.getMode();
    }

    std::vector<RealBlock> realPoints() const {
        return runTask.getRealBlocks();
    }

    std::vector<RealBlock> planBlocks() const {
        return runTask.getPlanBlocks();
    }

    std::vector<RealBlock> preBlocks() const {
        return runTask.getProList();
    }

    int getBuildElevatorAddress() const {
        return runTask.getBuildElevatorAddress();
    }

    bool asyncMap() const {
        return runTask.isAsyncMap();
    }

    std::vector<RealBlock> postBlocks() const {
        return runTask.getPostList();
    }

    bool isWaitTask(event::flow flow);

    bool isPause();

    bool isPreparation(event::flow flow);

    bool isPreCompleted(event::flow flow);

    bool isMechanismReady(event::flow flow);

    bool isFlowingWater(event::flow flow);

    bool isReturningBase(event::flow flow);

    bool isContinueWork(event::flow flow, bool suspend, bool skipManual = false);

    bool isRegularTask(event::flow flow);

    bool isManualTask(const RealTask &realTask);

    bool isRechargeFLow(event::flow flow);

    bool isPlannerEmpty(event::flow flow);

    bool isPreConditions(event::flow flow);

    bool isPostConditions(event::flow flow);

    void recordEmergencyStop(event::flow event_flow, const RealBlock &realBlock);

    bool recoverableEmergencyStop();

    bool recoverableSuspend();

    TaskStack lastEmergencyStop();

    void makeSurePause(event::flow flow);

    void release() override;

    static std::string output_interpolation_block(int id) {
        switch (id) {
            case FLOW_SEIZE_SEAT:
                return "启动节点";
            case FLOW_OPEN_MECHANISM:
                return "打开清洁机构节点";
            case FLOW_CLOSE_MECHANISM:
                return "关闭清洁机构节点";
            case FLOW_OUT_STATION:
                return "出站节点";
            case FLOW_END_SLEEP:
                return "退出睡眠模式节点";
            case FLOW_IN_BASE_POINT:
                return "摆渡点节点";
            case FLOW_IN_STATION:
                return "进站节点";
            case FLOW_ELEVATOR_PRE:
                return "梯控前期节点";
            case FLOW_ELEVATOR_POST:
                return "梯控后期节点";
            default:
                return "流程点 " + std::to_string(id) + " ";
        }
    }

};


#endif //APP_COMMUNICATION_ASYNCTASKRECORD_H
