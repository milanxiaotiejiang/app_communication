//
// Created by Looper on 2022/11/5.
//

#ifndef APP_COMMUNICATION_ASYNC_TASK_CALL_H
#define APP_COMMUNICATION_ASYNC_TASK_CALL_H

#include <utility>

#include "task/framework/AsyncTaskRecord.h"

#include "geometry_msgs/PoseStamped.h"
#include "task/status/state_machine.h"
#include "task/subscribe/async_machine.h"
#include "task/model/PointProgressVo.h"
#include "task/callback/EventNotifier.h"
#include "model/ManualModel.h"

const int MAX_FIRST_RETRY_COUNT = 2;
const int MAX_BASE_POINT_RETRY_COUNT = 3;
const int MAX_RECHARGE_RETRY_COUNT = 5;

/**
 * 任务执行线程
 * 状态机相关信息从此处提取
 */
class AsyncTaskCall : public AsyncTaskRecord {
private:

    std::atomic<event::flow> event_flow;
    std::mutex event_flow_mtx;  // 互斥量用于保护写操作

    std::shared_ptr<TaskFeedback> feedback;

protected:

    std::atomic<int> firstRetryCount;
    std::atomic<int> backBaseRetryCount;
    std::atomic<int> rechargeRetryCount;

    void setFlow(event::flow flow) {
        std::lock_guard<std::mutex> lock(event_flow_mtx);  // 自动加锁并在离开作用域时自动解锁
        event_flow.store(flow);
        AsyncMachine::instance().setFlow(flow);
    }

    event::flow currentFlow() {
        return event_flow.load();
    }

    std::atomic<bool> isCarpetAndPack;

    std::deque<PointProgressVo> finishedPoints;

    TaskEventNotifier notifier;

protected:

    void handleManualOperation() override;

    void handleSpecialOperation() override;

    void handleErrorOperation() override;

    void handleStop() override;

    void handleTask(const RealTask &task) override;

    void handleBlock(const RealBlock &block) override;


    virtual void handleExecuteTask(const RealTask &task);

    void handleAutoBlock(const RealBlock &block);

    void handleBlockManualControl(const RealBlock &block);

    void handleBlockSpecialDevice(const RealBlock &block);

    void initTaskBlock(const RealTask &realTask);

    virtual void goodGame(event::GG gg);

    virtual void garbage(event::SB sb);

    void reset();

    virtual void handleFlowBlock(const RealBlock &block) = 0;

    virtual void processControl(const RealBlock &block) = 0;

    virtual void handlePlannerBlock(const RealBlock &block);

    virtual void feedBackPose(const geometry_msgs::Pose &pose) = 0;

    RealBlock findFrontBlock();

    RealBlock findFrontNextBlock();

    bool isBasePointReached(float disAccuracy, float angleAccuracy);


    void callGoNextBlock(const RealBlock &realBlock);

    void callBlockComplete(const std::function<void()> &f);

    void callManualCleanStart();

    void callManualCleanEnd();

    void callSubsequentSelfClean(const WorkStatus &status);

    void callSelfCleanClose();

    void callSubsequentMode(int mode, double cleanedRatio);

    void callUrgencyStop();

    void callReleaseStop();

    void callRecoveryStop();

    void callResume();

    void callPause();

    void callManualPause();

    void cancelTaskAndBack();

    void cancelTask();

    virtual void forceInterruptTask(event::SB sb);

public:
    AsyncTaskCall();

    void executeUnrecoverableError();

    void executeOneTask(const RealTask &task);

    void executeOnPathDone(event::error error);

    void executeOnPathFeedBack(int current_step, int goal_step, int current_goal, const geometry_msgs::Pose &pose);

    void executeOutStation(bool result);

    void executeInStation(bool result);

    void executeCover();

    void manualBackToBase(bool force);

    void manualResume();

    void manualPause();


    void enterManual();

    ManualModel quitManual();


    void executeUrgencyStop(bool isUrgencyStop);

    void urgencyStopAndCharge();


    void forceBackToBase(loop::special_epoll operation);


    void executeCarpet(bool carpet);

    void carpetStop();

    void executeLift(bool lift);


    RealTask runningTask() const;

    std::vector<RealTask> runTaskList();

    std::vector<PointProgressVo> runTaskPointList();

    void restore();

    TaskEventNotifier getNotifier() {
        return notifier;
    }

};

#endif //APP_COMMUNICATION_ASYNC_TASK_CALL_H
