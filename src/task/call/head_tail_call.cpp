//
// Created by Looper on 2022/11/26.
//

#include "task/call/head_tail_call.h"
#include "task/point_planner.h"
#include "simulation.h"
#include "future/timer_call.h"
#include "task/manager/MechanismManager.h"

void HeadTailPointCall::handleFlowPoint(const RealPoint &point) {
    if (point.getId() == FLOW_SEIZE_SEAT) {
//        setFlow(event::flow::out_base_station);
        setFlow(event::flow::switch_node_work_mode);
    } else if (point.getId() == FLOW_OUT_STATION) {
        if (point.realError.arrive) {
//            setFlow(event::flow::switch_node_work_mode);
            setFlow(event::flow::preliminary_preparation_completed);
        } else {
            setFlow(event::flow::software_interrupt_task);
        }
    } else if (point.getId() == FLOW_END_SLEEP) {
        if (point.realError.arrive) {
//            setFlow(event::flow::preliminary_preparation_completed);
            setFlow(event::flow::out_base_station);
        } else {
            setFlow(event::flow::software_interrupt_task);
        }
    } else if (point.getId() == FLOW_IN_BASE_POINT) {
        if (point.realError.arrive) {
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
                setFlow(event::flow::software_interrupt_task);
            }
        }
    } else if (point.getId() == FLOW_IN_STATION) {
        if (point.realError.arrive) {
            setFlow(event::flow::arrive_base_station_success);
        } else {
            if (rechargeRetryCount < MAX_RECHARGE_RETRY_COUNT) {
                rechargeRetryCount++;
                setFlow(event::flow::try_recharging_again);
            } else {
                setFlow(event::flow::software_interrupt_task);
            }
        }
    } else if (point.getId() == FLOW_CLOSE_MECHANISM) {
        if (point.realError.arrive) {
            setFlow(event::flow::flowing_water_execution_completed);
        } else {
            setFlow(event::flow::hardware_interrupt_task);
        }
    } else if (point.getId() == FLOW_OPEN_MECHANISM) {
        if (point.realError.arrive) {
            setFlow(event::flow::cleaning_mechanism_ready);
        } else {
            setFlow(event::flow::hardware_interrupt_task);
        }
    }
}

void HeadTailPointCall::processControl(const RealPoint &point) {
    switch (currentFlow()) {
        case event::flow::out_base_station: {
            callOutBaseStation();
            break;
        }
        case event::flow::switch_node_work_mode: {
            callSwitchWorkMode([this](bool work) {
                flowEndSleepPoint.realError.arrive = work;
                pushPoint(flowEndSleepPoint);
            });
            break;
        }
        case event::flow::preliminary_preparation_completed: {
            LOG(INFO) << "HeadTailPointCall : 前期的出站、睡眠等流程执行成功，现在启动清洁机构 ...";
//            callOpenMechanism(point.getWorkStatus(), [this]() {
//                flowOpenMechanismPoint.realError.arrive = true;
//                pushPoint(flowOpenMechanismPoint);
//            });
            callOpenMechanism(point.getWorkStatus(), runTask.isKnife(), []() {});
            break;
        }
        case event::flow::cleaning_mechanism_ready: {
            LOG(INFO) << "HeadTailPointCall : 清洁机构下放成功，准备执行规划点位任务，当前去第一个点 ...";
            recordEmergencyStop(event::flow::ensure_move_to_start_point, point);
            setFlow(event::flow::ensure_move_to_start_point);
            RealPoint front = plannerQueue.front();
            callGoFirstPoint(front);
            break;
        }
        case event::flow::ensure_move_to_start_point: {
            if (point.realError.arrive) {
                LOG(INFO) << "HeadTailPointCall : 到达第一个点位，开始流水线作业 ...";
                setFlow(event::flow::flowing_water_production);
                pushPoint(point);
            } else {
                auto currentPoint = findFrontPoint();
                if (firstRetryCount < MAX_FIRST_RETRY_COUNT) {
                    LOG(INFO) << "HeadTailPointCall : 未到达第一个点位，重试中 ...";
                    firstRetryCount++;
                    exchangeFrontPoint(currentPoint);
                    callGoFirstPoint(currentPoint);
                } else {
                    LOG(INFO) << "HeadTailPointCall : 第一个点位重试后不能到达，跳点进入后续流程 ...";
                    setFlow(event::flow::flowing_water_production);
                    pushPoint(point);
                }
            }
            break;
        }
        case event::flow::flowing_water_production: {
            if (plannerQueue.size() == 1) {
                //最后一个已经走完，移除最后一个再次执行一次，走收拖头
                LOG(INFO) << "HeadTailPointCall : 清扫结束，准备回基站点 ...";
                callPointComplete([this]() {
                    callBackBasePoint();
                });
            } else {
                auto nextPoint = findFrontNextPoint();
                callGoNextPoint(nextPoint);
            }
            break;
        }
        case event::flow::arrive_base_point_success: {
            LOG(INFO) << "HeadTailPointCall : 成功到达基站前点位，收起清洁机构 ...";
//            callCloseMechanism([this]() {
//                flowCloseMechanismPoint.realError.arrive = true;
//                pushPoint(flowCloseMechanismPoint);
//            });
            callCloseMechanism([]() {});
            break;
        }
        case event::flow::flowing_water_execution_completed: {
            LOG(INFO) << "HeadTailPointCall : 任务执行完成且返回了基站点，准备回充 ...";
            callBackStation();
            break;
        }
        case event::flow::arrive_base_station_success: {
            LOG(INFO) << "HeadTailPointCall : 回到基站，且已充电 ...";
            goodGame();
            break;
        }
        case event::flow::try_recharging_again: {
            LOG(INFO) << "HeadTailPointCall : 回充失败 rechargeRetryCount : " << rechargeRetryCount << " , 再次返回基站点位置 ...";
            backBaseRetryCount = 0;
            callBackBasePoint();
            break;
        }
        case event::flow::try_move_base_point_again: {
            LOG(INFO) << "HeadTailPointCall : 返回基站点位失败 backBaseRetryCount : " << backBaseRetryCount << " , 重试中 ...";
            callBackBasePoint();
            break;
        }
        case event::flow::hardware_interrupt_task: {
            LOG(INFO) << "HeadTailPointCall : 清洁机构出错，执行返回基站命令 错误代码" << point.getId();
            callBackBasePoint();
            break;
        }
        case event::flow::software_interrupt_task: {
            LOG(ERROR) << "HeadTailPointCall software_interrupt_task ...";
            cancelTask();
            softwareInterruptTask(point);
            break;
        }
    }
}

void HeadTailPointCall::callOpenMechanism(const WorkStatus &status, bool knife, function<void()> f) {
//    AsyncTaskFramework::callOpenMechanism(status, f);

    flowOpenMechanismPoint.realError.arrive = true;

    MechanismManager::instance().controlWorkStatus(status, knife);
    if (!Environment::instance().isRealEnvironment) {
        async::TimerCall::instance().baseLoop()->scheduleLater(std::chrono::seconds(1), [this]() {
            LOG(INFO) << "AsyncTaskFramework : 相应的清洁机构已打开 ...";
            notify_one([this]() {
                pushPoint(flowOpenMechanismPoint);
            });
        });
    } else {
        async::TimerCall::instance().baseLoop()->scheduleLater(
                std::chrono::seconds(OPENING_TIME_OF_CLEANING_MECHANISM), [this]() {
                    LOG(INFO) << "AsyncTaskFramework : 相应的清洁机构已打开 ...";
                    notify_one([this]() {
                        pushPoint(flowOpenMechanismPoint);
                    });
                });
    }
}

void HeadTailPointCall::callCloseMechanism(function<void()> f) {
//    AsyncTaskFramework::callCloseMechanism(f);
    LOG(INFO) << "AsyncTaskFramework : 准备关闭相应的清洁机构 ...";

    flowCloseMechanismPoint.realError.arrive = true;

    //这个函数里面关闭所有清洁机构
    MechanismManager::instance().resetWorkStatus();
    if (!Environment::instance().isRealEnvironment) {
        async::TimerCall::instance().baseLoop()
                ->scheduleLater(std::chrono::seconds(1), [this]() {
                    LOG(INFO) << "AsyncTaskFramework : 相应的清洁机构已关闭 ...";
                    notify_one([this]() {
                        pushPoint(flowCloseMechanismPoint);
                    });
                });
    } else {
        async::TimerCall::instance().baseLoop()
                ->scheduleLater(std::chrono::seconds(CLOSING_TIME_OF_CLEANING_MECHANISM), [this]() {
                    LOG(INFO) << "AsyncTaskFramework : 相应的清洁机构已关闭 ...";
                    notify_one([this]() {
                        pushPoint(flowCloseMechanismPoint);
                    });
                });
    }
}

void HeadTailPointCall::callGoFirstPoint(RealPoint point) {
//    PublishInner
    PointPlanner::instance().gotoPlannerFirstPoint(point);
    async::TimerCall::instance().baseLoop()
            ->scheduleLater(std::chrono::seconds(point.realError.timeout), [this, &point]() {
                auto currentPoint = findFrontPoint();
                if (currentPoint.getId() == point.id) {
                    executeOnNext(event::error::TIMEOUT);
                }
            });
}

void HeadTailPointCall::exchangeFrontPoint(const RealPoint &point) {
    plannerQueue.pop_front();
    plannerQueue.push_front(point);
}

bool HeadTailPointCall::canIssuedTask(const std::string &launchPeople) {
    if (isWaitTask(currentFlow())) {
        return true;
    }
    if (isManualTask(launchPeople)) {
        if (isFlowingWater(currentFlow())) {
            return true;
        }
    }
    return false;
}

bool HeadTailPointCall::taskRunning() {
    return !isWaitTask(currentFlow());
}
