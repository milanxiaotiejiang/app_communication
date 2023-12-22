//
// Created by Looper on 2022/11/26.
//

#include "task/call/head_tail_call.h"
#include "task/point_planner.h"
#include "simulation.h"
#include "future/timer_call.h"
#include "task/manager/MechanismManager.h"
#include "manager/elevator_control.h"

void HeadTailPointCall::handleFlowBlock(const RealBlock &block) {
    if (block.id == FLOW_SEIZE_SEAT) {
//        setFlow(event::flow::out_base_station);
        setFlow(event::flow::switch_node_work_mode);
    } else if (block.id == FLOW_OUT_STATION) {
        if (block.arrive) {
//            setFlow(event::flow::switch_node_work_mode);
            setFlow(event::flow::preliminary_preparation_completed);
        } else {
            setFlow(event::flow::software_interrupt_task);
        }
    } else if (block.id == FLOW_END_SLEEP) {
        if (block.arrive) {
//            setFlow(event::flow::preliminary_preparation_completed);
            setFlow(event::flow::out_base_station);
        } else {
            setFlow(event::flow::software_interrupt_task);
        }
    } else if (block.id == FLOW_IN_BASE_POINT) {
        if (isPostConditions(currentFlow())) {

        } else {
            if (block.arrive) {
                if (rechargeRetryCount == 0) {
                    setFlow(event::flow::arrive_base_point_success);
                } else {
                    setFlow(event::flow::flowing_water_execution_completed);
                }
            } else {
                if (backBaseRetryCount < MAX_BASE_POINT_RETRY_COUNT) {
                    backBaseRetryCount++;
                    setFlow(event::flow::try_move_base_point_again);
                } else {
//                setFlow(event::flow::software_interrupt_task);
                    LOG_IF(INFO, DEBUG_TASK)
                                    << "HeadTailPointCall : 多次返回摆渡点失败， 直接记为“任务执行完成且返回了基站点”， "
                                    <<
                                    "  backBaseRetryCount : " << backBaseRetryCount <<
                                    "  rechargeRetryCount : " << rechargeRetryCount << " ...";
                    setFlow(event::flow::flowing_water_execution_completed);
                }
            }
        }
    } else if (block.id == FLOW_IN_STATION) {
        if (block.arrive) {
            setFlow(event::flow::arrive_base_station_success);
        } else {
            if (rechargeRetryCount < MAX_RECHARGE_RETRY_COUNT) {
                rechargeRetryCount++;
                setFlow(event::flow::try_recharging_again);
            } else {
                setFlow(event::flow::software_interrupt_task);
            }
        }
    } else if (block.id == FLOW_CLOSE_MECHANISM) {
        if (block.arrive) {
            setFlow(event::flow::flowing_water_execution_completed);
        } else {
            setFlow(event::flow::hardware_interrupt_task);
        }
    } else if (block.id == FLOW_OPEN_MECHANISM) {
        if (block.arrive) {
            setFlow(event::flow::cleaning_mechanism_ready);
        } else {
            setFlow(event::flow::hardware_interrupt_task);
        }
    } else if (block.id == FLOW_ELEVATOR_PRE) {
        if (block.arrive) {
            setFlow(event::flow::cleaning_mechanism_ready);
        } else {
            setFlow(event::flow::software_interrupt_task);
        }
    } else if (block.id == FLOW_ELEVATOR_POST) {
        if (block.arrive) {
            setFlow(event::flow::formally_return_to_the_base_station);
        } else {
            setFlow(event::flow::software_interrupt_task);
        }
    } else if (block.id == FLOW_READY_BACK) {
        setFlow(event::flow::formally_return_to_the_base_station);
    } else {
        LOG(ERROR) << "HeadTailPointCall : 未知的流程点位 " << output_interpolation_block(block.id);
    }
}

void HeadTailPointCall::processControl(const RealBlock &block) {
    switch (currentFlow()) {
        case event::flow::out_base_station: {
            callOutBaseStation();
            break;
        }
        case event::flow::switch_node_work_mode: {
            callSwitchWorkMode([this](bool work) {
                flowEndSleepPoint.arrive = work;
                pushBlock(flowEndSleepPoint);
            });
            break;
        }
        case event::flow::preliminary_preparation_completed: {
            LOG_IF(INFO, DEBUG_TASK) << "HeadTailPointCall : 前期的出站、睡眠等流程执行成功，现在启动清洁机构 ...";
//            callOpenMechanism(point.getWorkStatus(), [this]() {
//                flowOpenMechanismPoint.realError.arrive = true;
//                pushPoint(flowOpenMechanismPoint);
//            });
            if (baseTaskMode() == static_cast<int>(TaskMode::Zoned)) {
                LOG_IF(INFO, DEBUG_TASK) << "AsyncTaskFramework : 矩形任务无需打开清洁机构 ...";
                flowOpenMechanismPoint.arrive = true;
                notify_one([this]() {
                    pushBlock(flowOpenMechanismPoint);
                });
            } else if (asyncMap()) {
                LOG_IF(INFO, DEBUG_TASK) << "AsyncTaskFramework : 多地图任务无需打开清洁机构 ...";
                flowOpenMechanismPoint.arrive = true;
                notify_one([this]() {
                    pushBlock(flowOpenMechanismPoint);
                });
            } else
                callOpenMechanism(baseWorkStatus(), isKnife(), []() {});
            break;
        }
        case event::flow::cleaning_mechanism_ready: {
            if (preConditions.empty()) {
                LOG_IF(INFO, DEBUG_TASK)
                                << "HeadTailPointCall : 清洁机构准备完成，准备执行规划点位任务，当前去第一个点 ...";
                recordEmergencyStop(event::flow::ensure_move_to_start_point, block);
                setFlow(event::flow::ensure_move_to_start_point);
                RealBlock front = plannerQueue.front();
                callGoFirstPoint(front);
            } else {
                LOG_IF(INFO, DEBUG_ELEVATOR) << "HeadTailPointCall : 梯控前期逻辑开始 ...";
                setFlow(event::flow::trigger_special_pre_conditions);
                ElevatorControlManager::instance().handlePreFlow(preBlocks());
            }
            break;
        }
        case event::flow::ensure_move_to_start_point: {
            if (block.arrive) {
                LOG_IF(INFO, DEBUG_TASK) << "HeadTailPointCall : 到达第一个区域，开始流水线作业 ...";
                setFlow(event::flow::flowing_water_production);
                pushBlock(block);
            } else {
                auto currentPoint = findFrontBlock();
                if (firstRetryCount < MAX_FIRST_RETRY_COUNT) {
                    LOG_IF(INFO, DEBUG_TASK) << "HeadTailPointCall : 未到达第一个区域，重试中 ...";
                    firstRetryCount++;
                    exchangeFrontPoint(currentPoint);
                    callGoFirstPoint(currentPoint);
                } else {
                    LOG_IF(INFO, DEBUG_TASK) << "HeadTailPointCall : 第一个区域重试后不能到达，跳点进入后续流程 ...";
                    setFlow(event::flow::flowing_water_production);
                    pushBlock(block);
                }
            }
            break;
        }
        case event::flow::flowing_water_production: {
            if (block.retry) {
                auto nextBlock = findFrontBlock();
                if (nextBlock.inClean) {
                    if (nextBlock.timely_step > 0) {
                        nextBlock.already_step = nextBlock.already_step + nextBlock.timely_step + 1;
                        nextBlock.timely_step = 0;
                        exchangeFrontPoint(nextBlock);
                    }
                }
                callGoNextBlock(nextBlock);
            } else {
                if (plannerQueue.size() == 1) {
                    //最后一个已经走完，移除最后一个再次执行一次，走收拖头
                    LOG_IF(INFO, DEBUG_TASK) << "HeadTailPointCall : 清扫结束，准备回基站点 ...";
                    if (baseTaskMode() == static_cast<int>(TaskMode::Zoned)) {
                        LOG_IF(INFO, DEBUG_TASK) << "HeadTailPointCall : 矩形任务及时收起清洁机构 ...";
                        MechanismManager::instance().resetWorkStatus();
                    } else if (asyncMap()) {
                        LOG_IF(INFO, DEBUG_TASK) << "AsyncTaskFramework : 多地图任务及时收起清洁机构 ...";
                        MechanismManager::instance().resetWorkStatus();
                    }

                    callBlockComplete([this]() {
                        pushBlock(flowReadyBackPoint);
                    });
                } else {
                    auto currentBlock = findFrontBlock();
                    auto plannerPoints = currentBlock.plannerPoints;
                    if (!plannerPoints.empty()) {
                        auto cmcMode = plannerPoints[plannerPoints.size() - 1].cmcMode;
                        LOG_IF(INFO, DEBUG_CLEAN_MECHANISM)
                                        << "DEBUG_CLEAN_MECHANISM 取已完成的点列队尾，清洁机构操控 mode 为 "
                                        << static_cast<int>(cmcMode)
                                        << " ... ";
                        if (cmcMode == CmcMode::Open) {
                            //
                        } else if (cmcMode == CmcMode::Close) {
                            MechanismManager::instance().resetWorkStatus();
                        }
                    }

                    auto nextBlockPair = findFrontNextBlock();
                    if (nextBlockPair.first) {
                        auto nextBlock = nextBlockPair.second;
                        if (nextBlock.inClean) {
                            if (nextBlock.timely_step > 0) {
                                nextBlock.already_step = nextBlock.already_step + nextBlock.timely_step + 1;
                                nextBlock.timely_step = 0;
                                exchangeFrontPoint(nextBlock);
                            }
                        }
                        callGoNextBlock(nextBlock);
                    }
                }
            }
            break;
        }
        case event::flow::formally_return_to_the_base_station: {
            waitTaskQueue.clear();
            plannerQueue.clear();
            callBackBasePoint();
            break;
        }
        case event::flow::arrive_base_point_success: {
            if (baseTaskMode() == static_cast<int>(TaskMode::Zoned) && !MechanismManager::instance().isOpening()) {
                LOG_IF(INFO, DEBUG_TASK) << "HeadTailPointCall : 矩形任务 并且 已经收起 无需再次收起清洁机构 ...";
                flowCloseMechanismPoint.arrive = true;
                notify_one([this]() {
                    pushBlock(flowCloseMechanismPoint);
                });
            } else if (asyncMap()) {
                LOG_IF(INFO, DEBUG_TASK) << "AsyncTaskFramework : 多地图 并且 已经收起 无需再次收起清洁机构 ...";
                flowCloseMechanismPoint.arrive = true;
                notify_one([this]() {
                    pushBlock(flowCloseMechanismPoint);
                });
            } else {
                LOG_IF(INFO, DEBUG_TASK) << "HeadTailPointCall : 成功到达基站前点位，收起清洁机构 ...";
//                callCloseMechanism([this]() {
//                    flowCloseMechanismPoint.realError.arrive = true;
//                    pushPoint(flowCloseMechanismPoint);
//                });
                callCloseMechanism([]() {});
            }
            break;
        }
        case event::flow::flowing_water_execution_completed: {
            LOG_IF(INFO, DEBUG_TASK) << "HeadTailPointCall : 任务执行完成且返回了基站点，准备回充 ...";
            callBackStation();
            break;
        }
        case event::flow::arrive_base_station_success: {
            LOG_IF(INFO, DEBUG_TASK) << "HeadTailPointCall : 回到基站，且已充电 ...";
            goodGame(event::GG::gg_normal_flow);
            break;
        }
        case event::flow::try_recharging_again: {
            LOG_IF(INFO, DEBUG_TASK)
                            << "HeadTailPointCall : 回充失败 rechargeRetryCount : " << rechargeRetryCount
                            << " , 再次返回基站点位置 ...";
            backBaseRetryCount = 0;
            pushBlock(flowReadyBackPoint);
            break;
        }
        case event::flow::try_move_base_point_again: {
            LOG_IF(INFO, DEBUG_TASK)
                            << "HeadTailPointCall : 返回基站点位失败 backBaseRetryCount : " << backBaseRetryCount
                            << " , 重试中 ...";
            pushBlock(flowReadyBackPoint);
            break;
        }
        case event::flow::hardware_interrupt_task: {
            LOG_IF(INFO, DEBUG_TASK)
                            << "HeadTailPointCall : 清洁机构出错，执行返回基站命令 错误 ： "
                            << output_interpolation_block(block.id);
            pushBlock(flowReadyBackPoint);
            break;
        }
        case event::flow::software_interrupt_task: {
            LOG(ERROR) << "HeadTailPointCall software_interrupt_task ...";
            cancelTask();
            softwareInterruptTask(block);
            break;
        }
        case event::waiting_for_task:
            break;
        case event::trigger_special_pre_conditions:
            ElevatorControlManager::instance().completePreCirculation(block.arrive);
            break;
        case event::trigger_special_post_conditions:
            ElevatorControlManager::instance().completePostCirculation(block.arrive);
            break;
        default:
            LOG(ERROR) << "HeadTailPointCall : 未知的流程 " << static_cast<int>(currentFlow());
            break;
    }
}

void HeadTailPointCall::callOpenMechanism(const WorkStatus &status, bool knife, std::function<void()> f) {
//    AsyncTaskFramework::callOpenMechanism(status, f);

    flowOpenMechanismPoint.arrive = true;

    MechanismManager::instance().controlWorkStatus(status, knife);
    if (!Environment::instance().isRealEnvironment) {
        async::TimerCall::instance().baseLoop()->scheduleLater(std::chrono::seconds(1), [this]() {
            LOG_IF(INFO, DEBUG_TASK) << "AsyncTaskFramework : 相应的清洁机构已打开 ...";
            notify_one([this]() {
                pushBlock(flowOpenMechanismPoint);
            });
        });
    } else {
        async::TimerCall::instance().baseLoop()->scheduleLater(
                std::chrono::seconds(OPENING_TIME_OF_CLEANING_MECHANISM), [this]() {
                    LOG_IF(INFO, DEBUG_TASK) << "AsyncTaskFramework : 相应的清洁机构已打开 ...";
                    notify_one([this]() {
                        pushBlock(flowOpenMechanismPoint);
                    });
                });
    }
}

void HeadTailPointCall::callCloseMechanism(std::function<void()> f) {
//    AsyncTaskFramework::callCloseMechanism(f);
    LOG_IF(INFO, DEBUG_TASK) << "AsyncTaskFramework : 准备关闭相应的清洁机构 ...";

    flowCloseMechanismPoint.arrive = true;

    //这个函数里面关闭所有清洁机构
    MechanismManager::instance().resetWorkStatus();
    if (!Environment::instance().isRealEnvironment) {
        async::TimerCall::instance().baseLoop()
                ->scheduleLater(std::chrono::seconds(1), [this]() {
                    LOG_IF(INFO, DEBUG_TASK) << "AsyncTaskFramework : 相应的清洁机构已关闭 ...";
                    notify_one([this]() {
                        pushBlock(flowCloseMechanismPoint);
                    });
                });
    } else {
        async::TimerCall::instance().baseLoop()
                ->scheduleLater(std::chrono::seconds(CLOSING_TIME_OF_CLEANING_MECHANISM), [this]() {
                    LOG_IF(INFO, DEBUG_TASK) << "AsyncTaskFramework : 相应的清洁机构已关闭 ...";
                    notify_one([this]() {
                        pushBlock(flowCloseMechanismPoint);
                    });
                });
    }
}

void HeadTailPointCall::callGoFirstPoint(const RealBlock &block) {
    callGoNextBlock(block, true);
}

void HeadTailPointCall::exchangeFrontPoint(const RealBlock &block) {
    plannerQueue.pop_front();
    plannerQueue.push_front(block);
}

bool HeadTailPointCall::canIssuedTask(const RealTask &task) {
    if (isWaitTask(currentFlow())) {
        return true;
    }
    if (isManualTask(task)) {
        if (isFlowingWater(currentFlow())) {
            return true;
        }
    }
    return false;
}

bool HeadTailPointCall::taskRunning() {
    return !isWaitTask(currentFlow());
}
