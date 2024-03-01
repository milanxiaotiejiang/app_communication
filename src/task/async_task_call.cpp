//
// Created by Looper on 2022/11/5.
//

#include "task/async_task_call.h"
#include "task/point_planner.h"
#include "future/timer_call.h"
#include "future/thread_pool.h"
#include "segmentation/map_attribute.h"
#include "geometry_msgs/Pose2D.h"
#include "simulation.h"
#include "task/model/PointProgressVo.h"
#include "task/manager/PointProgressPublish.h"
#include "task/manager/MechanismManager.h"
#include "leave/ParamManager.h"
#include "task/feedback.h"
#include "task/manager/SwitchModePublish.h"
#include "manager/PublishInnerManager.h"
#include "leave/cartographer_node.h"
#include "leave/HotWindNote.h"
#include "exploration/ExplorationCenter.h"
#include "future/node/mode_validate.h"
#include "model/ManualModel.h"
#include "task/manager/NodeWorkModeManager.h"
#include "segmentation/GateComprehensive.h"
#include "manager/elevator_control.h"
#include "manager/PublishOutManager.h"

/*
 * 初始化函数将当墙状态设置为等待任务（状态机起始）
 */
AsyncTaskCall::AsyncTaskCall() : feedback(std::make_shared<TaskFeedback>()),
                                 mGateDistribution(std::make_shared<AsyncGateDistribution>()) {
    notifier.setOnTaskCallback(std::dynamic_pointer_cast<ITaskCallback>(feedback));
    feedback->run();

    setFlow(event::flow::waiting_for_task);
    setEpollManual(loop::manual_epoll::manual_normal);
    setEpollSpecial(loop::special_epoll::special_normal);
    setEpollError(loop::error_epoll::error_normal);
    setUrgencyStop(loop::urgency_stop::trigger_urgency_stop);

    initTaskBlock(runTask);

    mGateDistribution->setCallbackDistributionStart([]() {
        AsyncMachine::instance().setGateMachine(true);
    });
    mGateDistribution->setCallbackDistribution([this](bool success, int progress) {
        AsyncMachine::instance().setGateMachine(false);
        //int blockId, event::error error, const std::string &message
        if (success) {
            LOG_IF(INFO, DEBUG_GATE) << "AsyncGateImplement  假设通过闸机了 ";
            executeOnPathDone(0, event::error::SUCCEEDED, "闸机");
        } else {
            if (progress == -1) {
                PointPlanner::instance().cancelPoint();
                LOG_IF(INFO, DEBUG_GATE) << "AsyncGateImplement  执行取消 ";
                executeOnPathDone(0, event::error::SUCCEEDED, "闸机");
            } else {
                LOG_IF(INFO, DEBUG_GATE) << "AsyncGateImplement  执行错误 ";
                setFlow(event::flow::software_interrupt_task);
                executeOnPathDone(0, event::error::FAIL, "闸机");
            }
        }
    });


    ElevatorControlManager::instance().setElevatorCallback(
            [](int floor, int doorState, int lastDirection, int availability, int nextDirection) {
                PublishOutManager::instance().publishElevatorStatus(
                        ElevatorModel(floor, doorState, lastDirection, availability, nextDirection));
            });

    ElevatorControlManager::instance().setCallbackElevatorPre([this](bool result) {
        notify_one([this, &result]() {

            if (!Environment::instance().isRealEnvironment) {
                async::TimerCall::instance().baseLoop()->scheduleLater(std::chrono::seconds(10), [this]() {
                    try {
                        manualBackToBase(true);
                    } catch (...) {
                        LOG_IF(INFO, DEBUG_TASK) << "AsyncTaskCall : notify pre elevator error ...";
                    }
                });
            }

            LOG_IF(INFO, DEBUG_TASK) << "AsyncTaskCall : notify pre elevator finish ...";
            callOpenMechanism(baseWorkStatus(), isKnife(), []() {});
            preConditions.clear();
            flowElevatorPrePoint.arrive = result;
            pushBlock(flowElevatorPrePoint);
        });
    });

    ElevatorControlManager::instance().setCallbackElevatorPost([this](bool result) {
        notify_one([this, &result]() {
            LOG_IF(INFO, DEBUG_TASK) << "AsyncTaskCall : notify post elevator finish ...";
            postConditions.clear();
            flowElevatorPostPoint.arrive = result;
            pushBlock(flowElevatorPostPoint);
        });
    });
}

void AsyncTaskCall::handleManualOperation() {
    switch (epoll_manual) {
        case loop::manual_epoll::manual_resume:
            LOG_IF(INFO, DEBUG_TASK) << "AsyncTaskCall : 手动继续任务，撤销暂停拦截 ...";
            callResume();
            break;
        case loop::manual_epoll::manual_pause:
            LOG_IF(INFO, DEBUG_TASK) << "AsyncTaskCall : 手动暂停任务，增加暂停拦截 ...";
            callPause(false);
            break;
        case loop::manual_epoll::manual_back:
            LOG_IF(INFO, DEBUG_TASK) << "AsyncTaskCall : 手动取消任务，进入手动接管模式，手动需要返回基站点 ...";
            cancelTaskAndBack(false);
            break;
        case loop::manual_epoll::manual_force_back:
            LOG_IF(INFO, DEBUG_TASK) << "AsyncTaskCall : 强制返回，进入强制接管模式，强制返回基站点 ...";
            cancelTaskAndBack(true);
            break;
        case loop::manual_epoll::manual_task_over:
            LOG_IF(INFO, DEBUG_TASK) << "AsyncTaskCall : 有 App 或 Pad 下发任务，停止当前任务 ...";
            mGateDistribution->cancelDistribution();
            cancelAny();
            goodGame(event::GG::gg_task_over);
            break;
        default:
            LOG_IF(INFO, DEBUG_TASK) << "AsyncTaskCall handleManualOperation : " << epoll_manual << " ...";
            break;
    }
}

void AsyncTaskCall::handleSpecialOperation() {
    switch (epoll_special) {
        case loop::special_epoll::special_low_battery: {
            LOG_IF(INFO, DEBUG_TASK) << "AsyncTaskCall : 低电量，低电量导致需要强制返回基站点 ...";
            break;
        }
        case loop::special_epoll::special_branch_water: {
            LOG_IF(INFO, DEBUG_TASK) << "AsyncTaskCall : 清水箱空，清水箱空导致需要强制返回基站点 ...";
            MechanismManager::instance().resetWorkStatus();
            break;
        }
        case loop::special_epoll::special_sewage_water: {
            LOG_IF(INFO, DEBUG_TASK) << "AsyncTaskCall : 污水箱满，污水箱满导致需要强制返回基站点 ...";
            break;
        }
        case loop::special_epoll::special_branch_sewage_water: {
            LOG_IF(INFO, DEBUG_TASK) << "AsyncTaskCall : 污水箱满/清水箱空，污水箱满/清水箱空导致需要强制返回基站点 ...";
            break;
        }
        case loop::special_epoll::special_dust_push_anomaly: {
            LOG_IF(INFO, DEBUG_TASK) << "AsyncTaskCall : 尘推电机堵转，导致需要强制返回基站点 ...";
            break;
        }
        case loop::special_epoll::special_wet_tow_anomaly: {
            LOG_IF(INFO, DEBUG_TASK) << "AsyncTaskCall : 湿拖电机堵转，导致需要强制返回基站点 ...";
            break;
        }
        default:
            LOG_IF(INFO, DEBUG_TASK) << "AsyncTaskCall handleSpecialOperation : " << epoll_special << " ...";
            break;
    }
    if (!isWaitTask(currentFlow())) {
        if (!isManualMode()) {
            cancelTaskAndBack(false);
        } else {
            LOG_IF(INFO, DEBUG_TASK) << "AsyncTaskCall  手动模式无需返回 ...";
        }
    } else {
        LOG_IF(INFO, DEBUG_TASK) << "AsyncTaskCall  无任务无需返回 ...";
    }
}

void AsyncTaskCall::handleErrorOperation() {
    switch (epoll_error) {
        case loop::error_epoll::error_manual_clean_start:
            LOG_IF(INFO, DEBUG_TASK) << "AsyncTaskCall : 进入手动模式";
            callManualCleanStart();
            break;
        case loop::error_epoll::error_manual_clean_end:
            LOG_IF(INFO, DEBUG_TASK) << "AsyncTaskCall : 退出手动模式";
            callManualCleanEnd();
            break;
        case loop::error_epoll::error_lift:
            LOG_IF(INFO, DEBUG_TASK) << "AsyncTaskCall : 走到电梯上了 ... ";
            forceInterruptTask(event::SB::sb_lift);
            break;
        case loop::error_epoll::error_electric_move:
            LOG_IF(INFO, DEBUG_TASK) << "AsyncTaskCall : 电机失能了 ... ";
            forceInterruptTask(event::SB::sb_electric_move);
            break;
        case loop::error_epoll::error_unrecoverable:
            LOG_IF(INFO, DEBUG_TASK) << "AsyncTaskCall : 出现不可恢复的错误 ... ";
            forceInterruptTask(event::SB::sb_unrecoverable);
            break;
        default:
            LOG_IF(INFO, DEBUG_TASK) << "AsyncTaskCall handleErrorOperation : " << epoll_error << " ...";
            break;
    }
}

void AsyncTaskCall::handleStop() {
    switch (urgency_stop) {
        case loop::urgency_stop::trigger_urgency_stop:
            LOG_IF(INFO, DEBUG_TASK) << "AsyncTaskCall : 急停了 ... ";
            if (!isWaitTask(currentFlow()))
                callUrgencyStop();
            break;
        case loop::urgency_stop::recovery_urgency_stop:
            LOG_IF(INFO, DEBUG_TASK) << "AsyncTaskCall : 急停后推回基站，任务结束 ... ";
            if (!isWaitTask(currentFlow()))
                callRecoveryStop();
            break;
        case loop::urgency_stop::release_urgency_stop:
            LOG_IF(INFO, DEBUG_TASK) << "AsyncTaskCall : 解除急停了 ... ";
            if (!isWaitTask(currentFlow()))
                callReleaseStop();
            break;
        default:
            break;
    }
}

void AsyncTaskCall::handleTask(const RealTask &realTask) {
    if (isUnrecoverableError()) {
        LOG_IF(INFO, DEBUG_TASK) << "AsyncTaskCall : 程序达到不可恢复状态，不能接受任何数据，当前状态 "
                                 << "epoll_manual " << epoll_manual << " "
                                 << "epoll_special " << epoll_special << " "
                                 << "epoll_error " << epoll_error << " "
                                 << "urgency_stop " << urgency_stop << " ";
        return;
    }
    if (isUrgencyStop()) {
        LOG_IF(INFO, DEBUG_TASK) << "AsyncTaskCall : 急停拦截，不能接受 task 了 " << realTask.getId() << " "
                                 << "urgency_stop " << urgency_stop << " ";
        return;
    }
    if (isManualMode()) {
        LOG_IF(INFO, DEBUG_TASK) << "AsyncTaskCall : 手动模式开启，暂不接受 task " << realTask.getId() << " "
                                 << "epoll_error " << epoll_error << " ";
        return;
    }

    SwitchModePublish::instance().cancel();

    if (isWaitTask(currentFlow())) {

        handleExecuteTask(realTask);
    } else {
        if (isManualTask(realTask) && isFlowingWater(currentFlow())) {
            notify_one([this, &realTask]() {
                waitTaskQueue.push_back(realTask);
                pushManual(loop::manual_epoll::manual_task_over);
            });
        } else {
            LOG_IF(INFO, DEBUG_TASK)
                            << "AsyncTaskCall : 不支持前期出站阶段及后期回充阶段添加任务 event_flow : "
                            << currentFlow();
        }
    }
}

void AsyncTaskCall::handleBlock(const RealBlock &block) {
    if (isUnrecoverableError()) {
        LOG_IF(INFO, DEBUG_TASK)
                        << "AsyncTaskCall : 程序运行异常，抛弃 " << output_interpolation_block(block.id) << " ...";
        return;
    }
    if (isUrgencyStop()) {
        LOG_IF(INFO, DEBUG_TASK) << "AsyncTaskCall : 急停了，抛弃 " << output_interpolation_block(block.id) << " ...";
        return;
    }
    if (isManualMode()) {
        LOG_IF(INFO, DEBUG_TASK) << "AsyncTaskCall : 手动模式抛弃 " << output_interpolation_block(block.id) << " ...";
        return;
    }
    if (isPause()) {
        LOG_IF(INFO, DEBUG_TASK) << "AsyncTaskCall : 暂停了，抛弃 " << output_interpolation_block(block.id) << " ...";
        return;
    }
    if (isExchangeTask()) {
        LOG_IF(INFO, DEBUG_TASK)
                        << "AsyncTaskCall : 切换新的任务中，抛弃 " << output_interpolation_block(block.id) << " ...";
        return;
    }
    recordEmergencyStop(currentFlow(), block);
    if (isManualControl()) {
        handleBlockManualControl(block);
    } else if (isSpecialDevice()) {
        handleBlockSpecialDevice(block);
    } else {
        handleAutoBlock(block);
    }
}

void AsyncTaskCall::handleExecuteTask(const RealTask &task) {
    runTask = task;

    initTaskBlock(runTask);

    //清扫队列中的正常点全部加入
    preConditions.clear();
    for (const auto &block: preBlocks()) {
        preConditions.push_back(block);
    }
    plannerQueue.clear();
    for (const auto &block: planBlocks()) {
        plannerQueue.push_back(block);
    }
    postConditions.clear();
    for (const auto &block: postBlocks()) {
        postConditions.push_back(block);
    }

    firstRetryCount = 0;
    backBaseRetryCount = 0;
    rechargeRetryCount = 0;

    finishedPoints.clear();

    HotWindNoteSingleton::instance().closeHotWind();

    auto gateList = SegmentationDataBase::instance().loadGate(SegmentationDataBase::instance().getDbMap().id);
    mGateComprehensive = std::make_shared<GateComprehensive>(gateList);

    //预埋点，执行当期任务的第一个点，触发 handlePoint 流程
    notify_one([this]() {
        pushBlock(flowSeizeSeatPoint);
    });
}

void AsyncTaskCall::handleAutoBlock(const RealBlock &block) {
    switch (block.id) {
        //中断，进基站，返回摆渡点，结束睡眠模式，出站，开关清洁机构，抢占，为流程点位，执行流程工作
        case FLOW_IN_STATION:
        case FLOW_IN_BASE_POINT:
        case FLOW_END_SLEEP:
        case FLOW_OUT_STATION:
        case FLOW_CLOSE_MECHANISM:
        case FLOW_OPEN_MECHANISM:
        case FLOW_SEIZE_SEAT:
        case FLOW_ELEVATOR_PRE:
        case FLOW_ELEVATOR_POST:
        case FLOW_READY_BACK:
            handleFlowBlock(block); //随后进行处理
            break;
            //正常规划出的点位
        default:
            handlePlannerBlock(block);
            break;
    }
    //流程控制
    processControl(block);
}

void AsyncTaskCall::handleBlockManualControl(const RealBlock &block) {
    switch (block.id) {
        case FLOW_IN_BASE_POINT:
        case FLOW_CLOSE_MECHANISM:
        case FLOW_IN_STATION:
        case FLOW_ELEVATOR_PRE:
        case FLOW_ELEVATOR_POST:
        case FLOW_READY_BACK:
            handleFlowBlock(block);
            processControl(block);
            break;
        default:
            LOG_IF(INFO, DEBUG_TASK)
                            << "AsyncTaskCall : 手动接管期间不必要接受 " << output_interpolation_block(block.id)
                            << " ...";
            break;
    }
}

void AsyncTaskCall::handleBlockSpecialDevice(const RealBlock &block) {
    switch (block.id) {
        case FLOW_IN_BASE_POINT:
        case FLOW_CLOSE_MECHANISM:
        case FLOW_IN_STATION:
        case FLOW_ELEVATOR_PRE:
        case FLOW_ELEVATOR_POST:
        case FLOW_READY_BACK:
            handleFlowBlock(block);
            processControl(block);
            break;
        default:
            LOG_IF(INFO, DEBUG_TASK)
                            << "AsyncTaskCall : 强制模式下不必要接受 " << output_interpolation_block(block.id)
                            << " ...";
            break;
    }
}

void AsyncTaskCall::initTaskBlock(const RealTask &realTask) {
    //预埋点流转循环，打开清洁机构，关闭清洁机构，出站
    realTask.assignmentPoint(flowSeizeSeatPoint, FLOW_SEIZE_SEAT);
    realTask.assignmentPoint(flowOpenMechanismPoint, FLOW_OPEN_MECHANISM);
    realTask.assignmentPoint(flowCloseMechanismPoint, FLOW_CLOSE_MECHANISM);
    realTask.assignmentPoint(flowOutStationPoint, FLOW_OUT_STATION);
    //结束睡眠模式，抵达摆渡点，进站
    realTask.assignmentPoint(flowEndSleepPoint, FLOW_END_SLEEP);
    realTask.assignmentPoint(flowInBasePoint, FLOW_IN_BASE_POINT);
    realTask.assignmentPoint(flowInStationPoint, FLOW_IN_STATION);
    //梯控前后
    realTask.assignmentPoint(flowElevatorPrePoint, FLOW_ELEVATOR_PRE);
    realTask.assignmentPoint(flowElevatorPostPoint, FLOW_ELEVATOR_POST);

    realTask.assignmentPoint(flowReadyBackPoint, FLOW_READY_BACK);
}


void AsyncTaskCall::goodGame(event::GG gg) {
    LOG(WARNING) << "AsyncTaskCall : goodGame " << gg;
    finishedPoints;

    double cleanedRatio = 0.0;
    if (!finishedPoints.empty()) {
        PointProgressVo &back = finishedPoints.back();
        if (back.isRenew()) {
            cleanedRatio = back.getValue() * back.getTotalFrequency();
        }
    }
    if (gg != event::GG::gg_normal_flow) {
        cleanedRatio = 0.0;
    }
    if (cancelTaskUpdateMap) {
        cleanedRatio = 0.0;
    }

    setEpollManual(loop::manual_epoll::manual_normal);
    setEpollSpecial(loop::special_epoll::special_normal);
    if (!isManualMode()) {
        setEpollError(loop::error_epoll::error_normal);
    }

    setFlow(event::flow::waiting_for_task);
    reset();

    MechanismManager::instance().resetWorkStatus();

    if (!waitTaskQueue.empty()) {
        notify_one([this]() {
            auto task = waitTaskQueue.front();
            pushTask(task);
            waitTaskQueue.pop_front();
        });
    } else {
        if (gg == event::GG::gg_normal_flow) {
            callSubsequentSelfClean(baseWorkStatus());
        }
        callSubsequentMode(baseTaskMode(), cleanedRatio);
    }
}


void AsyncTaskCall::garbage(event::SB sb) {
    LOG_IF(INFO, DEBUG_TASK) << "AsyncTaskCall : 程序出现严重错误，不可恢复，以下是现场可保存的信息 " << sb;
    LOG_IF(INFO, DEBUG_TASK) << " start ————————————————————————————————————————————————————";

    CartographerServiceClient::instance().callSensorStatus();

    for (const auto &item: stopStack) {
        LOG_IF(INFO, DEBUG_TASK) << "AsyncTaskCall stopStack : " << item;
    }

    LOG_IF(INFO, DEBUG_TASK) << "AsyncTaskCall epoll_manual : " << epoll_manual;
    LOG_IF(INFO, DEBUG_TASK) << "AsyncTaskCall epoll_special : " << epoll_special;
    LOG_IF(INFO, DEBUG_TASK) << "AsyncTaskCall epoll_error : " << epoll_error;
    LOG_IF(INFO, DEBUG_TASK) << "AsyncTaskCall urgency_stop : " << urgency_stop;

    LOG_IF(INFO, DEBUG_TASK) << "AsyncTaskCall event_flow: " << currentFlow();

    LOG_IF(INFO, DEBUG_TASK) << "AsyncTaskCall plannerQueue: " << plannerQueue.size();

    LOG_IF(INFO, DEBUG_TASK) << "AsyncTaskCall firstRetryCount: " << firstRetryCount
                             << " , backBaseRetryCount : " << backBaseRetryCount
                             << " , rechargeRetryCount : " << rechargeRetryCount;

    LOG_IF(INFO, DEBUG_TASK) << "AsyncTaskCall OpenMechanism: " << flowOpenMechanismPoint.arrive
                             << " , CloseMechanism : " << flowCloseMechanismPoint.arrive
                             << " , OutStation : " << flowOutStationPoint.arrive
                             << " , EndSleep : " << flowEndSleepPoint.arrive
                             << " , InBase : " << flowInBasePoint.arrive
                             << " , InStation : " << flowInStationPoint.arrive
                             << " , ElevatorPre : " << flowElevatorPrePoint.arrive
                             << " , ElevatorPost : " << flowElevatorPostPoint.arrive
                             << " , ReadyBack : " << flowReadyBackPoint.arrive;

    MechanismManager::instance().resetWorkStatus();

    reset();

    if (!isManualMode()) {
        setEpollError(loop::error_epoll::error_unrecoverable);
    }

    LOG_IF(INFO, DEBUG_TASK) << " end ————————————————————————————————————————————————————";
}

void AsyncTaskCall::reset() {

    AsyncTaskRecord::release();

    runTask.setId("");

    plannerQueue.clear();
    firstRetryCount = 0;
    backBaseRetryCount = 0;
    rechargeRetryCount = 0;
    flowSeizeSeatPoint.arrive = false;
    flowOpenMechanismPoint.arrive = false;
    flowCloseMechanismPoint.arrive = false;
    flowOutStationPoint.arrive = false;
    flowEndSleepPoint.arrive = false;
    flowInBasePoint.arrive = false;
    flowInStationPoint.arrive = false;
    flowElevatorPrePoint.arrive = false;
    flowElevatorPostPoint.arrive = false;
    flowReadyBackPoint.arrive = false;

    isCarpetAndPack = false;

    cancelTaskUpdateMap = false;

    finishedPoints.clear();
}

void AsyncTaskCall::handlePlannerBlock(const RealBlock &block) {
//    LOG_IF(INFO, DEBUG_TASK) << "AsyncTaskCall : handlePlannerPoint "
//              << " taskId: " << block.taskId << " Id: " << block.id
//              << " arrive : " << block.arrive << " "
//              << " currentFrequency : " << block.currentFrequency << " "
//              << " inClean : " << block.inClean << " ";

    if (block.id < 0) {
        return;
    }
    if (block.taskId.empty()) {
        return;
    }
    auto plannerPoints = block.plannerPoints;
    if (plannerPoints.empty()) {
        return;
    }
    int current_step = block.already_step + block.timely_step;
    if (current_step > plannerPoints.size()) {
        return;
    }
    auto point = plannerPoints[current_step];

    PointProgressVo pointProgressVo(
            point.realPosition.x, point.realPosition.y,
            point.currentStep, block.totalStep,
            block.currentFrequency, block.totalFrequency,
            block.work_status, block.mode, block.inClean,
            runTaskId(), block.newTaskId,
            (double((double) point.id / block.totalStep))
    );

    LOG_IF(INFO, DEBUG_TASK)
                    << "blockId : " << block.id << " , " << pointProgressVo << " " << finishedPoints.size() << " "
                    << point.timeout;
    finishedPoints.push_back(pointProgressVo);

//    std::vector<geometry_msgs::Pose2D> exploration_path;
//    for (const auto &item: finishedPoints) {
//        geometry_msgs::Pose2D pose;
//        pose.x = item.getY();
//        pose.y = item.getX();
//        exploration_path.push_back(pose);
//    }
//    ExplorationCenter::instance().pathPublish(exploration_path);

    PointProgressPublish::instance().publishProgressPoint(pointProgressVo);

    runTask.changeArrivalStatus(block);
}

RealBlock AsyncTaskCall::findFrontBlock() {
    return plannerQueue.front();
}

std::pair<bool, RealBlock> AsyncTaskCall::findFrontNextBlock() {
    if (plannerQueue.empty()) {
        LOG_IF(INFO, DEBUG_TASK) << "findFrontNextBlock : empty ...";
        return std::make_pair(false, RealBlock());
    }
    if (plannerQueue.size() == 1) {
        LOG_IF(INFO, DEBUG_TASK) << "findFrontNextBlock : size == 1 ...";
        return std::make_pair(false, RealBlock());
    }
    plannerQueue.pop_front();
    return std::make_pair(true, plannerQueue.front());
}

bool AsyncTaskCall::isBasePointReached(float disAccuracy, float angleAccuracy) {
    geometry_msgs::Pose2D current_point = MapAttributeSingleton::instance().getRobotPositionPose();
    float robot_yaw = current_point.theta;
    float robot_x = current_point.x;
    float robot_y = current_point.y;
    float dist_error = sqrt(robot_x * robot_x + robot_y * robot_y);
    float angle_error = abs(robot_yaw);
    return (abs(dist_error) < disAccuracy) && (abs(angle_error) < angleAccuracy);
}

void AsyncTaskCall::callBackBasePoint() {

    if (postConditions.empty()) {
        auto backBasePoint = PointPlanner::createBackBasePoint();

        bool use_re_plan = true;
        std::vector<RealPoint> points;

        std::vector<int> stacks;
        mGateComprehensive->AStarPlannerPoint(backBasePoint, stacks);

        if (!stacks.empty()) {
            if (stacks.size() > 1) {

                mGateComprehensive->generateGatePointList(stacks, backBasePoint, points);
                if (!points.empty()) {
                    use_re_plan = false;
                }
            }
        }
        if (use_re_plan) {
            AsyncTaskFramework::callBackBasePoint();
        } else {
            mGateDistribution->onDistributionStart(points);
        }
    } else {
        LOG_IF(INFO, DEBUG_ELEVATOR) << "HeadTailPointCall : 梯控后期逻辑开始 ...";
        setFlow(event::flow::trigger_special_post_conditions);
        MechanismManager::instance().resetWorkStatus();
        ElevatorControlManager::instance().setBuildElevatorAddress(getBuildElevatorAddress());
        ElevatorControlManager::instance().handlePostFlow(postBlocks());
    }

}

void AsyncTaskCall::callGoNextBlock(const RealBlock &nextBlock, bool first) {
    if (first) PointPlanner::instance().setPathFirst();

    bool use_re_plan = true;
    std::vector<RealPoint> points;
    if (nextBlock.plannerPoints.size() == 1 && mGateComprehensive->isHasGate()) {

        RealPoint realPoint = nextBlock.plannerPoints[0];
        std::vector<int> stacks;
        try {
            mGateComprehensive->AStarPlannerPoint(realPoint, stacks);
        } catch (...) {
            LOG_IF(INFO, DEBUG_GATE)
                            << "AsyncGateImplement  AStarPlannerPoint catch " << "... ";
        }

        if (!stacks.empty()) {
            if (stacks.size() > 1) {

                mGateComprehensive->generateGatePointList(stacks, realPoint, points);
                if (points.size() > 0) {
                    use_re_plan = false;
                }
            }
        }
    }
    if (use_re_plan) {
        auto plannerPoints = nextBlock.plannerPoints;
        if (!plannerPoints.empty()) {
            auto cmcMode = plannerPoints[0].cmcMode;
            LOG_IF(INFO, DEBUG_CLEAN_MECHANISM)
                            << "DEBUG_CLEAN_MECHANISM 取未执行的点列队首，清洁机构操控 mode 为 "
                            << static_cast<int>(cmcMode) << " ... ";
            if (cmcMode == CmcMode::Open) {
                MechanismManager::instance().controlWorkStatus(nextBlock.work_status, nextBlock.knife);
            } else if (cmcMode == CmcMode::Close) {
                //
            }
        }

        PointPlanner::instance().goToPath(nextBlock);
        int id = nextBlock.id;
        int timeout = nextBlock.timeout;
        if (timeout > 0) {
            async::TimerCall::instance().baseLoop()
                    ->scheduleLater(std::chrono::seconds(timeout), [this, id]() {
                        auto currentPoint = findFrontBlock();
                        if (currentPoint.id == id) {
                            executeOnPathDone(id, event::error::TIMEOUT, "timeout");
                        }
                    });
        }
    } else {
        mGateDistribution->onDistributionStart(points);
    }
}

void AsyncTaskCall::callBlockComplete(const std::function<void()> &f) {
    plannerQueue.clear();
    f();
}

void AsyncTaskCall::callManualCleanStart() {
    HotWindNoteSingleton::instance().closeHotWind();
    if (!isWaitTask(currentFlow()))
        callManualPause();
    //电机失能
    MechanismManager::instance().enterManualControl();
    cancelTaskUpdateMap = true;
}

void AsyncTaskCall::callManualCleanEnd() {//退出手动模式
    setEpollError(loop::error_epoll::error_normal);
    //电机使能
    MechanismManager::instance().quitManualControl();

    if (isCharging()) {
        if (!isWaitTask(currentFlow())) {
            cancelTask();
            goodGame(event::GG::gg_manual_mode);
        } else {
            callNeedPublishSleep();
        }
    } else {
        if (isContinueWork(currentFlow(), true, true)) {
            LOG(INFO) << "CAN CONTINUE WORKING ... ";
        } else {
            //睡眠模式标志设置
            callNeedPublishSleep();
        }
    }
}

void AsyncTaskCall::callSubsequentSelfClean(const WorkStatus &status) {
    LOG_IF(INFO, DEBUG_TASK) << "AsyncTaskCall : 处理 WorkStatus " << status << " ...";
    if (!isCharging()) {
        return;
    }
    if (status.getMopStatus() > 0) {
        LOG_IF(INFO, DEBUG_TASK) << "Dry: " << ParamManager::instance().getDry();
        if (ParamManager::instance().getDry() == -1) {
            return;
        }
        time_t now = std::time(0);
        tm *ltm = localtime(&now);
        LOG_IF(INFO, DEBUG_TASK) << "年: " << 1900 + ltm->tm_year;
        LOG_IF(INFO, DEBUG_TASK) << "月: " << 1 + ltm->tm_mon;
        LOG_IF(INFO, DEBUG_TASK) << "日: " << ltm->tm_mday;
        LOG_IF(INFO, DEBUG_TASK) << "时间: " << ltm->tm_hour << ":" << ltm->tm_min << ":" << ltm->tm_sec;
        LOG_IF(INFO, DEBUG_TASK) << "dry_accumulation: " << Environment::instance().dry_accumulation;
        if (ParamManager::instance().getDry() == 0) {
            // 0-7 点执行
            if (ltm->tm_hour >= 0 + Environment::instance().dry_accumulation
                && ltm->tm_hour < 7 + Environment::instance().dry_accumulation) {
                HotWindNoteSingleton::instance().openHotWind();
            }
        } else if (ParamManager::instance().getDry() == 1) {
            HotWindNoteSingleton::instance().openHotWind();
        }
    }
}

void AsyncTaskCall::callSelfCleanClose() {

}

void AsyncTaskCall::callSubsequentMode(int mode, double cleanedRatio) {
    LOG_IF(INFO, DEBUG_TASK) << "AsyncTaskCall : 处理 mode " << mode << " ...";
    LOG_IF(INFO, DEBUG_TASK) << "AsyncTaskCall : 处理 cleanedRatio " << cleanedRatio << " ...";

    if (mode == static_cast<int>(TaskMode::Cover) && cleanedRatio > 0.8 && Environment::instance().update_map) {
        LOG_IF(INFO, DEBUG_TASK) << "AsyncTaskCall : 全覆盖清洁后需要更新地图信息 ...";
        CartographerPublisher::instance().publishUpdateMap();
    } else {
        callNeedPublishSleep();
        LOG(ERROR) << "AsyncTaskCall : gg";
    }
}

void AsyncTaskCall::callUrgencyStop() {
    if (!isPause()) {
        if (isPreCompleted(currentFlow())) {
            LOG_IF(INFO, DEBUG_TASK)
                            << "AsyncTaskCall : 前期准备工作完成，此处改变 event_flow 状态，变更为下一个步骤 ...";
            setFlow(event::flow::cleaning_mechanism_ready);
        }
        if (isContinueWork(currentFlow(), true)) {
            LOG_IF(INFO, DEBUG_TASK) << "AsyncTaskCall : 手动暂停任务，增加暂停拦截 ...";
            LOG_IF(INFO, DEBUG_TASK)
                            << "AsyncTaskCall : event_flow : " << currentFlow() << "   " << recoverableEmergencyStop();
            setEpollManual(loop::manual_epoll::manual_pause);
            if (isRechargeFLow(currentFlow())) {
                LOG_IF(INFO, DEBUG_TASK)
                                << "AsyncTaskCall : 回充中触发急停，为保证清洁机构确保收起，将回充重试次数设置为 0 ...";
                callCancelBackStation();
                rechargeRetryCount = 0;
                recordEmergencyStop(event::flow::formally_return_to_the_base_station, flowReadyBackPoint);
            }
            if (isMechanismReady(currentFlow())) {
                recordEmergencyStop(event::flow::cleaning_mechanism_ready, flowOpenMechanismPoint);
            }
            callPause(false);
        }
    }
    cancelTaskUpdateMap = true;
}

void AsyncTaskCall::callManualPause() {
    if (!isPause()) {
        if (isPreCompleted(currentFlow())) {
            LOG_IF(INFO, DEBUG_TASK)
                            << "AsyncTaskCall : 前期准备工作完成，此处改变 event_flow 状态，变更为下一个步骤 ...";
            setFlow(event::flow::cleaning_mechanism_ready);
        }
        if (isContinueWork(currentFlow(), true, true)) {
            LOG_IF(INFO, DEBUG_TASK) << "AsyncTaskCall : 手动暂停任务，增加暂停拦截 ...";
            LOG_IF(INFO, DEBUG_TASK)
                            << "AsyncTaskCall : event_flow : " << currentFlow() << "   " << recoverableEmergencyStop();
            setEpollManual(loop::manual_epoll::manual_pause);
            if (isRechargeFLow(currentFlow())) {
                LOG_IF(INFO, DEBUG_TASK)
                                << "AsyncTaskCall : 回充中触发手动模式为保证清洁机构确保收起，将回充重试次数设置为 0 ...";
                callCancelBackStation();
                rechargeRetryCount = 0;
                recordEmergencyStop(event::flow::formally_return_to_the_base_station, flowReadyBackPoint);
            }
            if (isMechanismReady(currentFlow())) {
                recordEmergencyStop(event::flow::cleaning_mechanism_ready, flowOpenMechanismPoint);
            }
            callPause(true);
        }
    }
}

void AsyncTaskCall::callReleaseStop() {
    if (recoverableEmergencyStop()) {
        if (isPause()) {
            if (recoverableSuspend()) {
                LOG_IF(INFO, DEBUG_TASK) << "AsyncTaskCall : 急停可恢复暂停状态 ... ";
                MechanismManager::instance().forceControlWorkStatus(baseWorkStatus(), isKnife());
            }
        }
    }
}

void AsyncTaskCall::callRecoveryStop() {
    setEpollManual(loop::manual_epoll::manual_normal);
    MechanismManager::instance().resetWorkStatus();
    cancelTask();
    goodGame(event::GG::gg_urgency_stop);
}

void AsyncTaskCall::callResume() {
    setEpollManual(loop::manual_epoll::manual_normal);
    if (recoverableSuspend()) {
        LOG_IF(INFO, DEBUG_TASK) << "AsyncTaskCall : 可继续执行任务 ...";
        MechanismManager::instance().forceControlWorkStatus(baseWorkStatus(), isKnife());
        auto lastStack = lastEmergencyStop();
        LOG_IF(INFO, DEBUG_TASK) << "AsyncTaskCall : 继续 lastStack : " << lastStack << " ...";

        if (isPlannerEmpty(lastStack.flow)) {
            LOG_IF(INFO, DEBUG_TASK) << "AsyncTaskCall : 流水点的最后，点位规划队列为空，需要直接返回基站 ...";
            pushBlock(flowReadyBackPoint);
        } else {
            notify_one([this, &lastStack]() {
                setFlow(lastStack.flow);
                pushBlock(lastStack.suspendBlock);
            });
        }
    }
}

void AsyncTaskCall::callPause(bool skipManual) {
    MechanismManager::instance().resetWorkStatus();
    if (isContinueWork(currentFlow(), true, skipManual)) {
        makeSurePause(currentFlow());
        cancelAny();
        if (!plannerQueue.empty()) {
            auto currentPoint = findFrontBlock();
            if (currentPoint.goal_step == INT_MAX) {
                auto nextPointPair = findFrontNextBlock();
                if (nextPointPair.first) {
                    plannerQueue.push_front(nextPointPair.second);
                }
            } else {
                plannerQueue.push_front(currentPoint);
            }
        }
    }
}

void AsyncTaskCall::cancelTaskAndBack(bool force) {
    if (!isReturningBase(currentFlow())) {
        if (isRegularTask(currentFlow())) {
            cancelAny();
            setEpollManual(loop::manual_epoll::manual_normal);
        }
        pushBlock(flowReadyBackPoint);
    } else {
        if (force) {
            LOG_IF(INFO, DEBUG_TASK) << "AsyncTaskCall : 强制返回 force ...";
            cancelAny();
            setEpollManual(loop::manual_epoll::manual_normal);
            pushBlock(flowReadyBackPoint);
        } else
            LOG_IF(INFO, DEBUG_TASK) << "AsyncTaskCall : 已经触发返回基站的动作了 ...";
    }
}

void AsyncTaskCall::cancelTask() {
    if (isRegularTask(currentFlow())) {
        cancelAny();
        waitTaskQueue.clear();
    }
}

void AsyncTaskCall::cancelAny() {
    PointPlanner::instance().cancelPath();
    async::TimerCall::instance().baseLoop()->cancelAny();
    mGateDistribution->cancelDistribution();
}

void AsyncTaskCall::forceInterruptTask(event::SB sb) {
    if (currentFlow() != event::flow::waiting_for_task &&
        currentFlow() != event::flow::hardware_interrupt_task &&
        currentFlow() != event::flow::software_interrupt_task) {
        LOG_IF(INFO, DEBUG_TASK) << "AsyncTaskCall : 当前有任务取消任务 ... ";
        cancelAny();
        waitTaskQueue.clear();
    }
}


void AsyncTaskCall::executeUnrecoverableError() {
    if (epoll_error != loop::error_epoll::error_normal) {
        return;
    }
    notify_one([this]() {
        pushError(loop::error_epoll::error_unrecoverable);
    });
}

void AsyncTaskCall::executeOneTask(const RealTask &task) {
    if (isUnrecoverableError()) {
        throw app::exception(make_error_code(error::operation_failure_please_restart_the_machine));
    }
    if (isManualMode()) {
        throw app::exception(make_error_code(error::machine_is_in_manual_mode_command_not_supported));
    }
    if (isUrgencyStop()) {
        throw app::exception(make_error_code(error::machine_is_in_emergency_stop_command_not_supported));
    }
    if (task.isVerifyMode()) {
        return;
    }
    notify_one([this, &task]() {
        pushTask(task);
    });
}

void AsyncTaskCall::executeOnPointDone(event::error error) {
    if (isCharging())
        return;
    if (isWaitTask(currentFlow()))
        return;
    if (isPreparation(currentFlow()))
        return;
    if (isUnrecoverableError())
        return;
    if (isUrgencyStop())
        return;
    if (isManualMode())
        return;

    if (error != event::error::TIMEOUT) {
        async::TimerCall::instance().baseLoop()->cancelAny();
    }

    if (mGateDistribution->isImplement()) {
        mGateDistribution->executeOnPointDone(error);
    } else
        notify_one([this, &error]() {
            if (!plannerQueue.empty()) {
                if (error == event::error::LOST) {
                    auto currentPoint = findFrontBlock();
                    currentPoint.arrive = error == event::error::SUCCEEDED;
                    currentPoint.retry = true;
                    pushBlock(currentPoint);
                } else {
                    auto currentPoint = findFrontBlock();
                    currentPoint.arrive = error == event::error::SUCCEEDED;
                    pushBlock(currentPoint);
                }
            } else {
                flowInBasePoint.arrive = error == event::error::SUCCEEDED;
                pushBlock(flowInBasePoint);
            }

        });
}

void AsyncTaskCall::executeOnPathDone(int blockId, event::error error, const std::string &message) {
    if (isCharging())
        return;
    if (isWaitTask(currentFlow()))
        return;
    if (isPreparation(currentFlow()))
        return;
    if (isUnrecoverableError())
        return;
    if (isUrgencyStop())
        return;
    if (isManualMode())
        return;

    if (error != event::error::TIMEOUT) {
        async::TimerCall::instance().baseLoop()->cancelAny();
    }

    if (mGateDistribution->isImplement()) {
        LOG(WARNING) << "PointPlanner moveBase  blockId : " << blockId << " , result : " << message;
        mGateDistribution->executeOnPathDone(error);
    } else
        notify_one([this, &blockId, &error, &message]() {
//            LOG(WARNING) << "PointPlanner moveBase  blockId : " << blockId << " , result : " << message;
            if (!plannerQueue.empty()) {
                if (error == event::error::LOST) {
                    auto currentPoint = findFrontBlock();
                    currentPoint.arrive = error == event::error::SUCCEEDED;
                    currentPoint.retry = true;
                    pushBlock(currentPoint);
                } else {
                    auto currentPoint = findFrontBlock();
                    currentPoint.arrive = error == event::error::SUCCEEDED;
                    pushBlock(currentPoint);
                }
            } else {
                flowInBasePoint.arrive = error == event::error::SUCCEEDED;
                pushBlock(flowInBasePoint);
            }
        });
}

void AsyncTaskCall::executeOnPathFeedBack(int blockId, int current_step, int goal_step, int current_goal,
                                          const geometry_msgs::Pose &pose) {
    lock([this, &blockId, &current_step, &goal_step, &current_goal]() {
        if (!plannerQueue.empty()) {
            RealBlock &block = plannerQueue.front();
            if (block.id == blockId) {
                if (current_step > block.timely_step) {
//                    LOG(ERROR) << "AsyncTaskCall : executeOnPathFeedBack : "
//                               << "  blockId " << blockId
//                               << "  plannerQueue.size " << plannerQueue.size()
//                               << "  point.id " << block.id
//                               << "  point.timely_step " << block.timely_step
//                               << "  point.already_step " << block.already_step
//                               << "  block.plannerPoints.size " << block.plannerPoints.size();
                    handlePlannerBlock(block);
                }
                block.timely_step = current_step;
                block.current_step = current_step;
                block.goal_step = goal_step;
                block.current_goal = current_goal;
            }
        }
    });
    feedBackPose(pose);
}

void AsyncTaskCall::executeOutStation(bool result) {
    notify_one([this, &result]() {
        flowOutStationPoint.arrive = result;
        pushBlock(flowOutStationPoint);
    });
}

void AsyncTaskCall::executeInStation(bool result) {
    notify_one([this, &result]() {
        flowInStationPoint.arrive = result;
        pushBlock(flowInStationPoint);
    });
}

void AsyncTaskCall::executeCover() {
    LOG_IF(INFO, DEBUG_TASK) << "AsyncTaskCall : 地图信息已更新完成，准备发布睡眠模式 ...";
    callNeedPublishSleep();
    LOG(ERROR) << "AsyncTaskCall : gg";
}


void AsyncTaskCall::manualBackToBase(bool force) {
    if (isWaitTask(currentFlow())) {
        throw app::exception(make_error_code(error::no_task_for_return_base_station));
    }
    if (isCharging()) {
        throw app::exception(make_error_code(error::already_in_the_base_station));
    }
    if (isUnrecoverableError()) {
        throw app::exception(make_error_code(error::operation_failure_please_restart_the_machine));
    }
    if (isUrgencyStop()) {
        throw app::exception(make_error_code(error::machine_is_in_emergency_stop_command_not_supported));
    }
    if (isManualMode()) {
        throw app::exception(make_error_code(error::machine_is_in_manual_mode_command_not_supported));
    }
    if (isPreparation(currentFlow())) {
        throw app::exception(make_error_code(error::operation_not_allowed_in_outbound));
    }
//    if (isReturningBase(currentFlow())) {
//        throw app::exception(make_error_code(error::already_returning_to_the_base_station));
//    }
    if (force) {
        notify_one([this]() {
            pushManual(loop::manual_epoll::manual_force_back);
        });
    } else {
        notify_one([this]() {
            pushManual(loop::manual_epoll::manual_back);
        });
    }

}

//队列前方加入恢复运行的点
void AsyncTaskCall::manualResume() {
    if (isUnrecoverableError()) {
        throw app::exception(make_error_code(error::the_current_state_is_uncontrollable));
    }
    if (isUrgencyStop()) {
        throw app::exception(make_error_code(error::machine_is_in_emergency_stop_command_not_supported));
    }
    if (isManualMode()) {
        throw app::exception(make_error_code(error::the_current_state_is_uncontrollable));
    }
    if (!isPause()) {
        throw app::exception(make_error_code(error::not_paused_status));
    }
    if (mGateDistribution->isImplement()) {
        throw app::exception(make_error_code(error::passing_through_the_gate_manual_control_is_not_supported));
    }
    notify_one([this]() {
        pushManual(loop::manual_epoll::manual_resume);
    });
}

//手动暂停，队列中加入一个暂停点
void AsyncTaskCall::manualPause() {
    if (isUnrecoverableError()) {
        throw app::exception(make_error_code(error::the_current_state_is_uncontrollable));
    }
    if (isUrgencyStop()) {
        throw app::exception(make_error_code(error::machine_is_in_emergency_stop_command_not_supported));
    }
    if (isManualMode()) {
        throw app::exception(make_error_code(error::the_current_state_is_uncontrollable));
    }
    if (isPause()) {
        throw app::exception(make_error_code(error::it_is_currently_suspended));
    }
    if (!isContinueWork(currentFlow(), true)) {
        throw app::exception(make_error_code(error::pause_is_not_supported));
    }
    if (mGateDistribution->isImplement()) {
        throw app::exception(make_error_code(error::passing_through_the_gate_manual_control_is_not_supported));
    }
    notify_one([this]() {
        pushManual(loop::manual_epoll::manual_pause);
    });
}

void AsyncTaskCall::enterManual() {//进入手动模式接口
    if (isUnrecoverableError()) {
        throw app::exception(make_error_code(error::the_current_state_is_uncontrollable));
    }
    if (isUrgencyStop()) {
        throw app::exception(make_error_code(error::machine_is_in_emergency_stop_command_not_supported));
    }
    if (isSpecialDevice()) {
        throw app::exception(make_error_code(error::the_current_state_is_uncontrollable));
    }
    if (isManualMode()) {//已经在手动模式下
        throw app::exception(make_error_code(error::already_in_manual_clean_mode));
    }
    if (mGateDistribution->isImplement()) {
        throw app::exception(make_error_code(error::passing_through_the_gate_manual_control_is_not_supported));
    }

    if (isWaitTask(currentFlow()))
        if (!NodeWorkModeManager::instance().enterWorkMode(2))
            throw app::exception(make_error_code(error::mode_switching_is_not_supported));

    notify_one([this]() {
        pushError(loop::error_epoll::error_manual_clean_start);
    });
}

ManualModel AsyncTaskCall::quitManual() {//退出手动模式接口
    ManualModel manualModel;

    manualModel.setIsUnrecoverableError(isUnrecoverableError());
    if (isUnrecoverableError()) {
        manualModel.setUnrecoverableError(epoll_error);
        LOG(INFO) << "ManualModel 程序异常，不能处理退出手动模式的命令 ";
        return manualModel;
    }

    manualModel.setIsManualMode(isManualMode());
    if (!isManualMode()) {//不在手动模式下
        LOG(INFO) << "ManualModel 不在手动模式下，不支持退出手动模式 ";
        return manualModel;
    }

    manualModel.setIsUrgencyStop(isUrgencyStop());
    if (isUrgencyStop()) {
        LOG(INFO) << "ManualModel 机器处于急停状态中，不支持退出手动模式 ";
        return manualModel;
    }

    manualModel.setIsCharging(isCharging());
    manualModel.setIsWaitTask(isWaitTask(currentFlow()));
    if (isCharging()) {
        notify_one([this]() {
            pushError(loop::error_epoll::error_manual_clean_end);
        });

        LOG(INFO) << "ManualModel 充电中，触发退出手动模式指令 ";
        return manualModel;
    } else {
        manualModel.setIsContinueWork(isContinueWork(currentFlow(), true, true));

        bool isWorkMode = false;
        int work_mode = 1;
        ros::param::get(NODE_CONTROLLER_WORK_MODE, work_mode);
        int carto_mode = 0;
        ros::param::get(CARTOGRAPHER_WORK_MODE, carto_mode);
        if (work_mode != 2) {
            LOG_IF(INFO, DEBUG_RESTORE) << "ManualModel " << "/node_controller/work_mode 检测为非工作模式，无法确认定位，不能退出手动模式指令 "
                                        << work_mode;
            manualModel.setIsWorkMode(isWorkMode);
            return manualModel;
        }
        if (carto_mode != 0) {
            LOG_IF(INFO, DEBUG_RESTORE) << "restore " << "/cartographer_work_mode 检测为非定位模式，无法确认定位，不能退出手动模式指令 "
                                        << carto_mode;
            manualModel.setIsWorkMode(isWorkMode);
            return manualModel;
        }
        isWorkMode = true;
        manualModel.setIsWorkMode(isWorkMode);

        bool isOffMap = false;
        bool isRestrictedZone = false;
        bool isMaxPassable = false;
        bool isPlanPath = false;
        const cv::Mat &map = SegmentationCenter::instance().generateMat();
        SegmentationCenter::instance()
                .isRestrictedZone(map, isOffMap, isRestrictedZone, isMaxPassable, isPlanPath, false);
        manualModel.setIsOffMap(isOffMap);
        manualModel.setIsRestrictedZone(isRestrictedZone);
        manualModel.setIsMaxPassable(isMaxPassable);
        manualModel.setIsPlanPath(isPlanPath);

        if (isOffMap) {
            LOG(INFO) << "ManualModel 机器不在地图内 ";
            return manualModel;
        }
        if (isRestrictedZone) {
            LOG(INFO) << "ManualModel 机器在禁区内 ";
            return manualModel;
        }
        if (!isMaxPassable) {
            LOG(INFO) << "ManualModel 机器不在最大的可通行区域内 ";
            return manualModel;
        }
        if (!isPlanPath) {
            LOG(INFO) << "ManualModel 机器位置与基站无法规划出有效路径 ";
            return manualModel;
        }

        notify_one([this]() {
            pushError(loop::error_epoll::error_manual_clean_end);
        });

        LOG(INFO) << "ManualModel 初步确定定位未丢，触发退出手动模式指令 ";
        return manualModel;
    }

}

void AsyncTaskCall::executeUrgencyStop(bool isUrgencyStop) {
    if (isUrgencyStop) {
        if (urgency_stop == loop::urgency_stop::trigger_urgency_stop) {
            return;
        }
        if (urgency_stop == loop::urgency_stop::recovery_urgency_stop) {
            return;
        }
        notify_one([this]() {
            pushUrgencyStop(loop::urgency_stop::trigger_urgency_stop);
        });
    } else {
        if (urgency_stop == loop::urgency_stop::release_urgency_stop) {
            return;
        }
        notify_one([this]() {
            pushUrgencyStop(loop::urgency_stop::release_urgency_stop);
        });
    }
}

void AsyncTaskCall::urgencyStopAndCharge() {
    if (isUnrecoverableError()) {
        return;
    }
    if (!isCharging()) {
        return;
    }
    if (urgency_stop != loop::urgency_stop::trigger_urgency_stop) {
        return;
    }
    notify_one([this]() {
        pushUrgencyStop(loop::urgency_stop::recovery_urgency_stop);
    });
}

void AsyncTaskCall::forceBackToBase(loop::special_epoll operation) {
    if (isCharging()) {
        return;
    }
    if (isUrgencyStop()) {
        return;
    }
    if (isUnrecoverableError()) {
        return;
    }
    if (isPreparation(currentFlow())) {
        return;
    }
    notify_one([this, &operation]() {
        pushSpecial(operation);
    });
}

void AsyncTaskCall::executeCarpet(bool carpet) {
    if (isCharging()) {
        return;
    }
    if (isUrgencyStop()) {
        return;
    }
    if (isUnrecoverableError()) {
        return;
    }
    if (isManualMode()) {
        return;
    }
    if (isFlowingWater(currentFlow())) {
//        "1.仅在尘推和湿拖模式下识别到地毯后抬起清洁机构；
//        2.识别到地毯后不关闭香氛或消杀。"
        if (baseWorkStatus().getPushStatus() > 0 || baseWorkStatus().getMopStatus() > 0) {
            if (carpet) {
                if (!isCarpetAndPack) {
                    isCarpetAndPack = true;
                    MechanismManager::instance().resetBelowWorkStatus();
                    LOG_IF(INFO, DEBUG_TASK) << "NativeSystemManager : executeCarpet "
                                             << "  检测到地毯并且已经收起清洁机构"
                                             << " ...";
                    for (int i = 0; i < 15; i++) {
                        carpetStop();
                        ros::Duration(0.3).sleep();
                    }
                }
            } else {
                if (isCarpetAndPack) {
                    isCarpetAndPack = false;
                    LOG_IF(INFO, DEBUG_TASK) << "NativeSystemManager : executeCarpet "
                                             << "  离开地毯，且机构已收起，执行再次放下清洁机构"
                                             << " ...";
                    MechanismManager::instance().forceControlWorkStatus(runTask.getWorkStatus(), runTask.isKnife());
                }
            }
        }
    }
}

void AsyncTaskCall::carpetStop() {
    geometry_msgs::Twist twist;
    twist.linear.x = 0;
    twist.linear.y = 0;
    twist.linear.z = 0;
    twist.angular.x = 0;
    twist.angular.y = 0;
    twist.angular.z = 0;
    PublishInnerManager::instance().publishVelocity(twist);
}

void AsyncTaskCall::executeLift(bool lift) {
    if (isCharging()) {
        return;
    }
    if (isUrgencyStop()) {
        return;
    }
    if (isUnrecoverableError()) {
        return;
    }
    if (isManualMode()) {
        return;
    }
    if (isPreparation(currentFlow())) {
        return;
    }
//    if (isReturningBase(currentFlow())) {
//        return;
//    }
    if (!isRegularTask(currentFlow())) {
        return;
    }
    if (lift) {
        notify_one([this]() {
            pushError(loop::error_epoll::error_lift);
        });
    }
}

void AsyncTaskCall::executeElectricMove() {
    if (isCharging()) {
        return;
    }
    if (isUrgencyStop()) {
        return;
    }
    if (isUnrecoverableError()) {
        return;
    }
    if (isManualMode()) {
        return;
    }
    notify_one([this]() {
        pushError(loop::error_epoll::error_electric_move);
    });
}

RealTask AsyncTaskCall::runningTask() const {
    return runTask;
}

std::vector<RealTask> AsyncTaskCall::runTaskList() {
    std::vector<RealTask> result;
    if (isFlowingWater(currentFlow())) {
        result.push_back(runTask);
    }
    return result;
}

std::vector<PointProgressVo> AsyncTaskCall::runTaskPointList() {
    std::vector<PointProgressVo> result;
    if (isFlowingWater(currentFlow())) {
        for (const auto &item: finishedPoints) {
            result.push_back(item);
        }
    }
    return result;
}

void AsyncTaskCall::restore() {
    if (!Environment::instance().rec_app_node_crash) {
        return;
    }
    LOG_IF(INFO, DEBUG_RESTORE) << "restore " << "检测到崩溃后的重启了 ";
    const EnterStatus &enterStatus = AsyncMachine::instance().getEnterStatus();
    event::flow restore_flow = enterStatus.task_flow;
    loop::manual_epoll restore_manual = enterStatus.epoll_manual;
    loop::special_epoll restore_special = enterStatus.epoll_special;
    loop::error_epoll restore_error = enterStatus.epoll_error;
    loop::urgency_stop restore_urgency_stop = enterStatus.urgency_stop;
    LOG_IF(INFO, DEBUG_RESTORE) << "restore " << "enterStatus " << enterStatus;
    if (enterStatus.node_mode != 2) {
        LOG_IF(INFO, DEBUG_RESTORE) << "restore " << "/node_controller/work_mode 检测为非工作模式，无法确认定位，不能处理返回基站 "
                                    << enterStatus.node_mode;
        return;
    }
    if (enterStatus.carto_mode != 0) {
        LOG_IF(INFO, DEBUG_RESTORE) << "restore " << "/cartographer_work_mode 检测为非定位模式，无法确认定位，不能处理返回基站 "
                                    << enterStatus.carto_mode;
        return;
    }
    if (isWaitTask(restore_flow)) {
        LOG_IF(INFO, DEBUG_RESTORE) << "restore " << "不是任务中的崩溃，无法确认定位，不能处理返回基站";
        return;
    }
    if (isCharging()) {
        LOG_IF(INFO, DEBUG_RESTORE) << "restore " << "已经在充电，无需返回基站";
        return;
    }
    if (restore_error == loop::error_epoll::error_unrecoverable ||
        restore_error == loop::error_epoll::error_lift ||
        restore_error == loop::error_epoll::error_electric_move ||
        restore_manual == loop::manual_epoll::manual_unknown ||
        restore_special == loop::special_epoll::special_unknown ||
        restore_error == loop::error_epoll::error_unknown) {
        LOG_IF(INFO, DEBUG_RESTORE) << "restore " << "原因多数未知，无法处理返回基站 ";
        return;
    }
    if (restore_urgency_stop == loop::urgency_stop::trigger_urgency_stop ||
        restore_urgency_stop == loop::urgency_stop::recovery_urgency_stop) {
        LOG_IF(INFO, DEBUG_RESTORE) << "restore " << "急停，无法处理返回基站";
        return;
    }
    if (restore_error == loop::error_epoll::error_manual_clean_start ||
        restore_error == loop::error_epoll::error_manual_clean_end) {
        LOG_IF(INFO, DEBUG_RESTORE) << "restore " << "手动模式触发，无法处理返回基站";
        return;
    }
    if (isPreparation(restore_flow)) {
        LOG_IF(INFO, DEBUG_RESTORE) << "restore " << "任务的前期准备工作，如工作模式切换、出站等，无法处理返回基站";
        return;
    }
    if (isPreConditions(restore_flow)) {
        LOG_IF(INFO, DEBUG_RESTORE) << "restore " << "梯控前期流程，无法处理返回基站";
        return;
    }
    if (isPostConditions(restore_flow)) {
        LOG_IF(INFO, DEBUG_RESTORE) << "restore " << "梯控后期流程，无法处理返回基站";
        return;
    }
    if (!Environment::instance().direct_start_move_base) {
        LOG_IF(INFO, DEBUG_RESTORE) << "restore " << "direct_start_move_base 为 false, move_base 暂不支持";
    }
    bool baseAvailable = ModeValidate::validateMoveBaseAvailable();
    if (!baseAvailable) {
        LOG_IF(INFO, DEBUG_RESTORE) << "restore " << "move_base 服务不可可用, 无法处理返回基站";
    }
    bool coreMoveAvailable = ModeValidate::validateCoreMoveAvailable();
    if (!coreMoveAvailable) {
        LOG_IF(INFO, DEBUG_RESTORE) << "restore " << "core_move 服务不可可用, 无法处理返回基站";
    }
    auto gateList = SegmentationDataBase::instance().loadGate(SegmentationDataBase::instance().getDbMap().id);
    mGateComprehensive = std::make_shared<GateComprehensive>(gateList);
    notify_one([this]() {
        setFlow(event::flow::ensure_move_to_start_point);
        pushManual(loop::manual_epoll::manual_force_back);
    });
}
