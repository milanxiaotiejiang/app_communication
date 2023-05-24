//
// Created by Looper on 2023/2/6.
//

#ifndef APP_COMMUNICATION_ASYNCTASKRECORD_H
#define APP_COMMUNICATION_ASYNCTASKRECORD_H

#include "AsyncTaskFramework.h"
#include "task/model/TaskStack.h"

const int FLOW_SEIZE_SEAT = -10;
const int FLOW_OPEN_MECHANISM = -11;
const int FLOW_CLOSE_MECHANISM = -12;
const int FLOW_OUT_STATION = -13;
const int FLOW_END_SLEEP = -14;
const int FLOW_IN_BASE_POINT = -15;
const int FLOW_IN_STATION = -16;

const int FLOW_ERROR_UNRECOVERABLE = -20;
const int FLOW_ERROR_LIFT = -21;

const int MAX_RECORD_TASK_STACK_SIZE = 3;

class AsyncTaskRecord : public AsyncTaskFramework {
protected:

    std::deque<TaskStack> stopStack;

    std::deque<RealTask> waitTaskQueue;
    std::deque<RealBlock> plannerQueue;

    RealTask runTask;

    RealBlock flowSeizeSeatPoint;
    RealBlock flowOpenMechanismPoint;
    RealBlock flowCloseMechanismPoint;
    RealBlock flowOutStationPoint;

    RealBlock flowEndSleepPoint;
    RealBlock flowInBasePoint;
    RealBlock flowInStationPoint;

    std::string runTaskId() const {
        return runTask.getId();
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
        return runTask.getRealPoints();
    }

    std::vector<RealBlock> planPoints() const {
        return runTask.getPlanPoints();
    }

    bool isWaitTask(event::flow flow);

    bool isPause();

    bool isPreparation(event::flow flow);

    bool isFlowingWater(event::flow flow);

    bool isReturningBase(event::flow flow);

    bool isContinueWork(event::flow flow, bool suspend);

    bool isRegularTask(event::flow flow);

    bool isManualTask(const RealTask &realTask);

    bool isRechargeFLow(event::flow flow);

    bool isPlannerEmpty(event::flow flow);

    void recordEmergencyStop(event::flow event_flow, const RealBlock &realPoint);

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
            default:
                return "流程点 " + to_string(id) + " ";
        }
    }

};


#endif //APP_COMMUNICATION_ASYNCTASKRECORD_H
