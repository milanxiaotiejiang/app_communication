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
#include "task/manager/SwitchModePublish.h"
#include "task/manager/MechanismManager.h"

/*
 * 初始化函数将当墙状态设置为等待任务（状态机起始）
 */
AsyncTaskCall::AsyncTaskCall() {
    int err = AsyncTaskCall::make_thread(run, this);
    LOG(INFO) << err;
    setFlow(event::flow::waiting_for_task);

    //预埋点流转循环，打开清洁机构，关闭清洁机构，出站
    runTask.assignmentPoint(flowSeizeSeatPoint, FLOW_SEIZE_SEAT);
    runTask.assignmentPoint(flowOpenMechanismPoint, FLOW_OPEN_MECHANISM);
    runTask.assignmentPoint(flowCloseMechanismPoint, FLOW_CLOSE_MECHANISM);
    runTask.assignmentPoint(flowOutStationPoint, FLOW_OUT_STATION);
    //结束睡眠模式，抵达摆渡点，进站，中断
    runTask.assignmentPoint(flowEndSleepPoint, FLOW_END_SLEEP);
    runTask.assignmentPoint(flowInBasePoint, FLOW_IN_BASE_POINT);
    runTask.assignmentPoint(flowInStationPoint, FLOW_IN_STATION);
}

void AsyncTaskCall::handleManualOperation() {
    switch (epoll_manual) {
        case loop::manual_epoll::manual_resume:
            LOG(INFO) << "AsyncTaskCall : 手动继续任务，撤销暂停拦截 ...";
            callResume();
            break;
        case loop::manual_epoll::manual_pause:
            LOG(INFO) << "AsyncTaskCall : 手动暂停任务，增加暂停拦截 ...";
            callPause();
            break;
        case loop::manual_epoll::manual_back:
            LOG(INFO) << "AsyncTaskCall : 手动取消任务，进入手动接管模式，手动需要返回基站点 ...";
            cancelTaskAndBack();
            break;
        case loop::manual_epoll::manual_force_back:
            LOG(INFO) << "AsyncTaskCall : 强制返回，进入强制接管模式，强制返回基站点 ...";
            cancelTaskAndBack();
            break;
        case loop::manual_epoll::manual_task_over:
            LOG(INFO) << "AsyncTaskCall : 有 App 或 Pad 下发任务，停止当前任务 ...";
            PointPlanner::instance().cancelGoal();
            async::TimerCall::instance().baseLoop()->cancelAny();
            goodGame();
            break;
        default:
            LOG(INFO) << "AsyncTaskCall handleManualOperation : " << epoll_manual << " ...";
            break;
    }
}

void AsyncTaskCall::handleSpecialOperation() {
    switch (epoll_special) {
        case loop::special_epoll::special_low_battery:
            LOG(INFO) << "AsyncTaskCall : 低电量，低电量导致需要强制返回基站点 ...";
            break;
        case loop::special_epoll::special_branch_water: {
            LOG(INFO) << "AsyncTaskCall : 清水箱空，清水箱空导致需要强制返回基站点 ...";
            break;
        }
        case loop::special_epoll::special_sewage_water: {
            LOG(INFO) << "AsyncTaskCall : 污水箱满，污水箱满导致需要强制返回基站点 ...";
            break;
        }
        case loop::special_epoll::special_branch_sewage_water: {
            LOG(INFO) << "AsyncTaskCall : 污水箱满/清水箱空，污水箱满/清水箱空导致需要强制返回基站点 ...";
            break;
        }
        case loop::special_epoll::special_dust_push_anomaly:
            LOG(INFO) << "AsyncTaskCall : 电机堵转，尘推滚异常导致需要强制返回基站点 ...";
            break;
        default:
            LOG(INFO) << "AsyncTaskCall handleSpecialOperation : " << epoll_special << " ...";
            break;
    }
    cancelTaskAndBack();
}

void AsyncTaskCall::handleErrorOperation() {
    switch (epoll_error) {
        case loop::error_epoll::error_manual_clean_start:
            LOG(INFO) << "AsyncTaskCall : 进入手动模式";
            callManualCleanStart();
            break;
        case loop::error_epoll::error_manual_clean_end:
            LOG(INFO) << "AsyncTaskCall : 退出手动模式";
            callManualCleanEnd();
            break;
        case loop::error_epoll::error_unrecoverable:
            LOG(INFO) << "AsyncTaskCall : 出现不可恢复的错误 ... ";
            triggerSuspend();
            break;
        default:
            LOG(INFO) << "AsyncTaskCall handleErrorOperation : " << epoll_error << " ...";
            break;
    }
}

void AsyncTaskCall::handleStop() {
    switch (urgency_stop) {
        case loop::urgency_stop::trigger_urgency_stop:
            LOG(INFO) << "AsyncTaskCall : 急停了 ... ";
            callUrgencyStop();
            break;
        case loop::urgency_stop::recovery_urgency_stop:
            LOG(INFO) << "AsyncTaskCall : 急停后推回基站，任务结束 ... ";
            callRecoveryStop();
            break;
        case loop::urgency_stop::release_urgency_stop:
            LOG(INFO) << "AsyncTaskCall : 解除急停了 ... ";
            callReleaseStop();
            break;
        default:
            break;
    }
}

void AsyncTaskCall::handleTask(const RealTask &realTask) {
    if (isUnrecoverableError()) {
        LOG(INFO) << "AsyncTaskCall : 程序达到不可恢复状态，不能接受任何数据，当前状态 "
                  << "epoll_manual " << epoll_manual << " "
                  << "epoll_special " << epoll_special << " "
                  << "epoll_error " << epoll_error << " "
                  << "urgency_stop " << urgency_stop << " ";
        return;
    }
    if (isUrgencyStop()) {
        LOG(INFO) << "AsyncTaskCall : 急停拦截，不能接受 task 了 " << realTask.getId() << " "
                  << "urgency_stop " << urgency_stop << " ";
        return;
    }
    if (isManualMode()) {
        LOG(INFO) << "AsyncTaskCall : 手动模式开启，暂不接受 task " << realTask.getId() << " "
                  << "epoll_error " << epoll_error << " ";
        return;
    }

    if (isWaitTask(event_flow)) {

        handleExecuteTask(realTask);
    } else {
        const std::string &launchPeople = realTask.getLaunchPeople();
        if (isManualTask(launchPeople) && isFlowingWater(event_flow)) {
            waitTaskQueue.push_back(realTask);
            pushManual(loop::manual_epoll::manual_task_over);
        } else {
            LOG(INFO) << "AsyncTaskCall : 不支持前期出站阶段及后期回充阶段添加任务 event_flow : " << event_flow;
        }
    }
}

void AsyncTaskCall::handlePoint(const RealPoint &realPoint) {
    if (isUnrecoverableError()) {
        LOG(INFO) << "AsyncTaskCall : 程序运行异常，抛弃不需要的点 " << realPoint.getId() << " ...";
        return;
    }
    if (isUrgencyStop()) {
        LOG(INFO) << "AsyncTaskCall : 急停了，抛弃不需要的点 " << realPoint.getId() << " ...";
        return;
    }
    if (isManualMode()) {
        LOG(INFO) << "AsyncTaskCall : 手动模式抛弃不需要的点 " << realPoint.getId() << " ...";
        return;
    }
    if (isPause()) {
        LOG(INFO) << "AsyncTaskCall : 暂停了，抛弃不需要的点 " << realPoint.getId() << " ...";
        return;
    }
    if (isExchangeTask()) {
        LOG(INFO) << "AsyncTaskCall : 切换新的任务中，抛弃不需要的点 " << realPoint.getId() << " ...";
        return;
    }
    recordEmergencyStop(event_flow, realPoint);
    if (isManualControl()) {
        handlePointManualControl(realPoint);
    } else if (isSpecialDevice()) {
        handlePointSpecialDevice(realPoint);
    } else {
        handleAutoPoint(realPoint);
    }
}

void AsyncTaskCall::handleExecuteTask(const RealTask &task) {
    runTask = task;

    //预埋点流转循环，打开清洁机构，关闭清洁机构，出站
    runTask.assignmentPoint(flowSeizeSeatPoint, FLOW_SEIZE_SEAT);
    runTask.assignmentPoint(flowOpenMechanismPoint, FLOW_OPEN_MECHANISM);
    runTask.assignmentPoint(flowCloseMechanismPoint, FLOW_CLOSE_MECHANISM);
    runTask.assignmentPoint(flowOutStationPoint, FLOW_OUT_STATION);
    //结束睡眠模式，抵达摆渡点，进站
    runTask.assignmentPoint(flowEndSleepPoint, FLOW_END_SLEEP);
    runTask.assignmentPoint(flowInBasePoint, FLOW_IN_BASE_POINT);
    runTask.assignmentPoint(flowInStationPoint, FLOW_IN_STATION);
    //清扫队列中的正常点全部加入
    plannerQueue.clear();
    for (const auto &point: runTask.getPlanPoints()) {
        plannerQueue.push_back(point);
    }

    firstRetryCount = 0;
    backBaseRetryCount = 0;
    rechargeRetryCount = 0;

    //预埋点，执行当期任务的第一个点，触发 handlePoint 流程
    pushPoint(flowSeizeSeatPoint);
}

void AsyncTaskCall::handleAutoPoint(const RealPoint &point) {
    switch (point.getId()) {
        //中断，进基站，返回摆渡点，结束睡眠模式，出站，开关清洁机构，抢占，为流程点位，执行流程工作
        case FLOW_IN_STATION:
        case FLOW_IN_BASE_POINT:
        case FLOW_END_SLEEP:
        case FLOW_OUT_STATION:
        case FLOW_CLOSE_MECHANISM:
        case FLOW_OPEN_MECHANISM:
        case FLOW_SEIZE_SEAT:
            handleFlowPoint(point); //随后进行处理
            break;
            //正常规划出的点位
        default:
            handlePlannerPoint(point);
            break;
    }
    //流程控制
    processControl(point);
}

void AsyncTaskCall::handlePointManualControl(const RealPoint &point) {
    switch (point.getId()) {
        case FLOW_IN_BASE_POINT:
        case FLOW_CLOSE_MECHANISM:
        case FLOW_IN_STATION:
            handleFlowPoint(point);
            processControl(point);
            break;
        default:
            LOG(INFO) << "AsyncTaskCall : 手动接管期间不必要接受的点位 " << point.getId() << " ...";
            break;
    }
}

void AsyncTaskCall::handlePointSpecialDevice(const RealPoint &point) {
    switch (point.getId()) {
        case FLOW_IN_BASE_POINT:
        case FLOW_CLOSE_MECHANISM:
        case FLOW_IN_STATION:
            handleFlowPoint(point);
            processControl(point);
            break;
        default:
            LOG(INFO) << "AsyncTaskCall : 强制模式下不必要接受的点位 " << point.getId() << " ...";
            break;
    }
}


void AsyncTaskCall::goodGame() {
    LOG(ERROR) << "AsyncTaskCall : goodGame";

    setEpollManual(loop::manual_epoll::manual_normal);
    setEpollSpecial(loop::special_epoll::special_normal);
    if (!isManualMode()) {
        setEpollError(loop::error_epoll::error_normal);
    }

    setFlow(event::flow::waiting_for_task);

    reset();

    if (!waitTaskQueue.empty()) {
        notify_one([this]() {
            auto task = waitTaskQueue.front();
            pushTask(task);
            waitTaskQueue.pop_front();
        });
    } else {
        callNeedPublishSleep();
        LOG(ERROR) << "AsyncTaskCall : gg";
    }
}


void AsyncTaskCall::garbage() {
    LOG(INFO) << "AsyncTaskCall : 程序出现严重错误，不可恢复，以下是现场可保存的信息 ";
    LOG(INFO) << " start ————————————————————————————————————————————————————";

    for (const auto &item: stopStack) {
        LOG(INFO) << "AsyncTaskCall stopStack : " << item;
    }

    LOG(INFO) << "AsyncTaskCall epoll_manual : " << epoll_manual;
    LOG(INFO) << "AsyncTaskCall epoll_special : " << epoll_special;
    LOG(INFO) << "AsyncTaskCall epoll_error : " << epoll_error;
    LOG(INFO) << "AsyncTaskCall urgency_stop : " << urgency_stop;

    LOG(INFO) << "AsyncTaskCall event_flow: " << event_flow;

    LOG(INFO) << "AsyncTaskCall plannerQueue: " << plannerQueue.size();

    LOG(INFO) << "AsyncTaskCall firstRetryCount: " << firstRetryCount
              << " , backBaseRetryCount : " << backBaseRetryCount
              << " , rechargeRetryCount : " << rechargeRetryCount;

    LOG(INFO) << "AsyncTaskCall OpenMechanism: " << flowOpenMechanismPoint.realError.arrive
              << " , CloseMechanism : " << flowCloseMechanismPoint.realError.arrive
              << " , OutStation : " << flowOutStationPoint.realError.arrive
              << " , EndSleep : " << flowEndSleepPoint.realError.arrive
              << " , InBase : " << flowInBasePoint.realError.arrive
              << " , InStation : " << flowInStationPoint.realError.arrive;

    MechanismManager::instance().resetWorkStatus();

    reset();

    if (!isManualMode()) {
        setEpollError(loop::error_epoll::error_unrecoverable);
    }

    LOG(INFO) << " end ————————————————————————————————————————————————————";
}

void AsyncTaskCall::reset() {

    AsyncTaskRecord::release();

    runTask.setId("");

    plannerQueue.clear();
    firstRetryCount = 0;
    backBaseRetryCount = 0;
    rechargeRetryCount = 0;
    flowSeizeSeatPoint.realError.arrive = false;
    flowOpenMechanismPoint.realError.arrive = false;
    flowCloseMechanismPoint.realError.arrive = false;
    flowOutStationPoint.realError.arrive = false;
    flowEndSleepPoint.realError.arrive = false;
    flowInBasePoint.realError.arrive = false;
    flowInStationPoint.realError.arrive = false;
}

void AsyncTaskCall::handlePlannerPoint(const RealPoint &point) {
    LOG(INFO) << "AsyncTaskCall : handlePlannerPoint "
              << " taskId: " << point.getTaskId() << " Id: " << point.getId()
              << " arrive : " << point.realError.arrive << " "
              << point.realProgress;

    PointProgressVo pointProgressVo(
            point.task_id, point.realPosition.x, point.realPosition.y,
            point.realProgress.currentStep, point.realProgress.totalStep,
            point.realProgress.currentFrequency, point.realProgress.totalFrequency,
            point.work_status, point.mode, point.inClean);
    PointProgressPublish::instance().publishProgressPoint(pointProgressVo);

    runTask.changeArrivalStatus(point);
}

RealPoint AsyncTaskCall::findFrontPoint() {
    return plannerQueue.front();
}

RealPoint AsyncTaskCall::findFrontNextPoint() {
    plannerQueue.pop_front();
    return plannerQueue.front();
}

bool AsyncTaskCall::isBasePointReached(float disAccuracy, float angleAccuracy) {
    geometry_msgs::Pose2D current_point = MapAttribute::instance().getRobotPositionPose();
    float robot_yaw = current_point.theta;
    float robot_x = current_point.x;
    float robot_y = current_point.y;
    float dist_error = sqrt(robot_x * robot_x + robot_y * robot_y);
    float angle_error = abs(robot_yaw);
    return (abs(dist_error) < disAccuracy) && (abs(angle_error) < angleAccuracy);
}


void AsyncTaskCall::callGoNextPoint(const RealPoint &nextPoint) {
    PointPlanner::instance().gotoPlannerPoint(nextPoint);
    int id = nextPoint.getId();
    int timeout = nextPoint.realError.timeout;
    if (timeout > 0) {
        async::TimerCall::instance().baseLoop()
                ->scheduleLater(std::chrono::seconds(timeout), [this, &id]() {
                    auto currentPoint = findFrontPoint();
                    if (currentPoint.getId() == id) {
                        executeOnNext(event::error::TIMEOUT);
                    }
                });
    }
}

void AsyncTaskCall::callPointComplete(const std::function<void()> &f) {
    plannerQueue.pop_front();
    f();
}


void AsyncTaskCall::callManualCleanStart() {
    cancelTask([this]() {
        goodGame();
    });
    //电机失能
    MechanismManager::instance().enterManualControl();
}

void AsyncTaskCall::callManualCleanEnd() {//退出手动模式
    setEpollError(loop::error_epoll::error_normal);
    //电机使能
    MechanismManager::instance().quitManualControl();
    //睡眠模式标志设置
    callNeedPublishSleep();
}

void AsyncTaskCall::callUrgencyStop() {
    if (!isPause()) {
        if (isContinueWork(event_flow, true)) {
            LOG(INFO) << "AsyncTaskCall : 手动暂停任务，增加暂停拦截 ...";
            LOG(INFO) << "AsyncTaskCall : event_flow : " << event_flow << "   " << recoverableEmergencyStop();
            setEpollManual(loop::manual_epoll::manual_pause);
            callPause();
        }
    }
}

void AsyncTaskCall::callReleaseStop() {
    if (recoverableEmergencyStop()) {
        if (isPause()) {
            if (recoverableSuspend()) {
                LOG(INFO) << "AsyncTaskCall : 急停可恢复暂停状态 ... ";
                MechanismManager::instance().forceControlWorkStatus(runTask.getWorkStatus());
            }
        }
    }
}

void AsyncTaskCall::callRecoveryStop() {
    setEpollManual(loop::manual_epoll::manual_normal);
    cancelTask([this]() {
        goodGame();
    });
}

void AsyncTaskCall::callResume() {
    setEpollManual(loop::manual_epoll::manual_normal);
    if (recoverableSuspend()) {
        LOG(INFO) << "AsyncTaskCall : 可继续执行任务 ...";
        auto lastStack = lastEmergencyStop();
        LOG(INFO) << "AsyncTaskCall : 继续 lastStack : " << lastStack << " ...";

        if (lastStack.flow == event::flow::flowing_water_production && plannerQueue.empty()) {
            LOG(INFO) << "AsyncTaskCall : 流水点的最后，点位规划队列为空，需要直接返回基站 ...";
            callBackBasePoint();
        } else {
            setFlow(lastStack.flow);
            pushPoint(lastStack.suspendPoint);
        }
    }
}

void AsyncTaskCall::callPause() {
    if (isContinueWork(event_flow, true)) {
        makeSurePause(event_flow);
        PointPlanner::instance().cancelGoal();
        async::TimerCall::instance().baseLoop()->cancelAny();
        if (!plannerQueue.empty()) {
            auto currentPoint = findFrontPoint();
            plannerQueue.push_front(currentPoint);
        }
    }
}

void AsyncTaskCall::cancelTaskAndBack() {
    if (!isReturningBase(event_flow)) {
        if (isRegularTask(event_flow)) {
            PointPlanner::instance().cancelGoal();
            async::TimerCall::instance().baseLoop()->cancelAny();
            waitTaskQueue.clear();

            plannerQueue.clear();
            setFlow(event::flow::flowing_water_production);
            recordEmergencyStop(event::flow::flowing_water_production, flowInBasePoint);
        }
        callBackBasePoint();
    } else {
        LOG(INFO) << "AsyncTaskCall : 已经触发返回基站的动作了 ...";
    }
}

void AsyncTaskCall::cancelTask(const function<void()> &f) {
    if (isRegularTask(event_flow)) {
        PointPlanner::instance().cancelGoal();
        async::TimerCall::instance().baseLoop()->cancelAny();
        waitTaskQueue.clear();
        f();
    }
}

void AsyncTaskCall::triggerSuspend() {
    if (event_flow != event::flow::waiting_for_task &&
        event_flow != event::flow::hardware_interrupt_task &&
        event_flow != event::flow::software_interrupt_task) {
        LOG(INFO) << "AsyncTaskCall : 当前有任务取消任务 ... ";
        PointPlanner::instance().cancelGoal();
        async::TimerCall::instance().baseLoop()->cancelAny();
        waitTaskQueue.clear();
    }

    garbage();
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
        return;
    }
    if (isManualMode()) {
        return;
    }
    if (isUrgencyStop()) {
        return;
    }
    notify_one([this, &task]() {
        pushTask(task);
    });
}

void AsyncTaskCall::executeOnNext(event::error error) {
    if (error != event::error::TIMEOUT) {
        async::TimerCall::instance().baseLoop()->cancelAny();
    }
    if (!plannerQueue.empty()) {
        notify_one([this, &error]() {
            auto currentPoint = findFrontPoint();
            currentPoint.realError.arrive = error == event::error::SUCCEEDED;
            pushPoint(currentPoint);
        });
    } else {

        notify_one([this, &error]() {
            flowInBasePoint.realError.arrive = error == event::error::SUCCEEDED;
            pushPoint(flowInBasePoint);
        });

    }
}

void AsyncTaskCall::executeOutStation(bool result) {
    notify_one([this, &result]() {
        flowOutStationPoint.realError.arrive = result;
        pushPoint(flowOutStationPoint);
    });
}

void AsyncTaskCall::executeInStation(bool result) {
    notify_one([this, &result]() {
        flowInStationPoint.realError.arrive = result;
        pushPoint(flowInStationPoint);
    });
}


void AsyncTaskCall::manualBackToBase(bool force) {
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
    if (isPreparation(event_flow)) {
        throw app::exception(make_error_code(error::operation_not_allowed_in_outbound));
    }
    if (isPause()) {
        if (isReturningBase(event_flow)) {
            notify_one([this]() {
                pushManual(loop::manual_epoll::manual_resume);
            });
        } else {
            notify_one([this]() {
                pushManual(loop::manual_epoll::manual_force_back);
            });
        }
    } else {
        if (isReturningBase(event_flow)) {
            throw app::exception(make_error_code(error::already_returning_to_the_base_station));
        }
        if (force) {
            notify_one([this]() {
                pushManual(loop::manual_epoll::manual_force_back);
            });
        } else {
            if (isRegularTask(event_flow)) {
                notify_one([this]() {
                    pushManual(loop::manual_epoll::manual_back);
                });
            } else {
                notify_one([this]() {
                    pushManual(loop::manual_epoll::manual_force_back);
                });
            }
        }
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
    if (!isContinueWork(event_flow, true)) {
        throw app::exception(make_error_code(error::pause_is_not_supported));
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
    notify_one([this]() {
        pushError(loop::error_epoll::error_manual_clean_start);
    });
}

void AsyncTaskCall::quitManual() {//退出手动模式接口
    if (isUnrecoverableError()) {
        throw app::exception(make_error_code(error::the_current_state_is_uncontrollable));
    }
    if (isUrgencyStop()) {
        throw app::exception(make_error_code(error::machine_is_in_emergency_stop_command_not_supported));
    }
    if (isSpecialDevice()) {
        throw app::exception(make_error_code(error::the_current_state_is_uncontrollable));
    }
    if (!isManualMode()) {//不在手动模式下
        throw app::exception(make_error_code(error::not_in_manual_clean_mode));
    }
    if (!isCharging()) {
        throw app::exception(make_error_code(error::manual_in_the_base_station));
    }
    notify_one([this]() {
        pushError(loop::error_epoll::error_manual_clean_end);
    });
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
    if (ZooInnerStatus::instance().getIsCharging()) {
        return;
    }
    if (isUrgencyStop()) {
        return;
    }
    if (isUnrecoverableError()) {
        throw app::exception(make_error_code(error::the_current_state_is_uncontrollable));
    }
    pushSpecial(operation);
}


std::string AsyncTaskCall::runTaskId() {
    return runTask.getId();
}

std::vector<RealTask> AsyncTaskCall::runTaskList() {
    std::vector<RealTask> result;
    if (isFlowingWater(event_flow)) {
        result.push_back(runTask);
    }
    return result;
}

std::vector<RealPoint> AsyncTaskCall::runTaskPoint() {
    std::vector<RealPoint> result;
    if (isFlowingWater(event_flow)) {
        for (const auto &item: runTask.getRealPoints()) {
            result.push_back(item);
        }
    }
    return result;
}
