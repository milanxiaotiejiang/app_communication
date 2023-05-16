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
#include "task/task_util.h"

/*
 * 初始化函数将当墙状态设置为等待任务（状态机起始）
 */
AsyncTaskCall::AsyncTaskCall() {
    fbPtr = make_unique<TaskFeedback>();

    setFlow(event::flow::waiting_for_task);
    setEpollManual(loop::manual_epoll::manual_normal);
    setEpollSpecial(loop::special_epoll::special_normal);
    setEpollError(loop::error_epoll::error_normal);
    setUrgencyStop(loop::urgency_stop::trigger_urgency_stop);

    initTaskPoint(runTask);
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
            goodGame(event::GG::gg_task_over);
            break;
        default:
            LOG(INFO) << "AsyncTaskCall handleManualOperation : " << epoll_manual << " ...";
            break;
    }
}

void AsyncTaskCall::handleSpecialOperation() {
    switch (epoll_special) {
        case loop::special_epoll::special_low_battery: {
            LOG(INFO) << "AsyncTaskCall : 低电量，低电量导致需要强制返回基站点 ...";
            break;
        }
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
        case loop::special_epoll::special_dust_push_anomaly: {
            LOG(INFO) << "AsyncTaskCall : 电机堵转，尘推滚异常导致需要强制返回基站点 ...";
            break;
        }
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
        case loop::error_epoll::error_lift:
            LOG(INFO) << "AsyncTaskCall : 走到电梯上了 ... ";
            forceInterruptTask(event::SB::sb_lift);
            break;
        case loop::error_epoll::error_unrecoverable:
            LOG(INFO) << "AsyncTaskCall : 出现不可恢复的错误 ... ";
            forceInterruptTask(event::SB::sb_unrecoverable);
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
            if (!isWaitTask(currentFlow())) {
                callUrgencyStop();
            }
            break;
        case loop::urgency_stop::recovery_urgency_stop:
            LOG(INFO) << "AsyncTaskCall : 急停后推回基站，任务结束 ... ";
            if (!isWaitTask(currentFlow())) {
                callRecoveryStop();
            }
            break;
        case loop::urgency_stop::release_urgency_stop:
            LOG(INFO) << "AsyncTaskCall : 解除急停了 ... ";
            if (!isWaitTask(currentFlow())) {
                callReleaseStop();
            }
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

    SwitchModePublish::instance().cancel();

    if (isWaitTask(event_flow)) {

        handleExecuteTask(realTask);
    } else {
        if (isManualTask(realTask) && isFlowingWater(event_flow)) {
            notify_one([this, &realTask]() {
                waitTaskQueue.push_back(realTask);
                pushManual(loop::manual_epoll::manual_task_over);
            });
        } else {
            LOG(INFO) << "AsyncTaskCall : 不支持前期出站阶段及后期回充阶段添加任务 event_flow : " << event_flow;
        }
    }
}

void AsyncTaskCall::handlePoint(const RealPoint &realPoint) {
    if (isUnrecoverableError()) {
        LOG(INFO) << "AsyncTaskCall : 程序运行异常，抛弃 " << output_interpolation_point(realPoint.id) << " ...";
        return;
    }
    if (isUrgencyStop()) {
        LOG(INFO) << "AsyncTaskCall : 急停了，抛弃 " << output_interpolation_point(realPoint.id) << " ...";
        return;
    }
    if (isManualMode()) {
        LOG(INFO) << "AsyncTaskCall : 手动模式抛弃 " << output_interpolation_point(realPoint.id) << " ...";
        return;
    }
    if (isPause()) {
        LOG(INFO) << "AsyncTaskCall : 暂停了，抛弃 " << output_interpolation_point(realPoint.id) << " ...";
        return;
    }
    if (isExchangeTask()) {
        LOG(INFO) << "AsyncTaskCall : 切换新的任务中，抛弃 " << output_interpolation_point(realPoint.id) << " ...";
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

    initTaskPoint(runTask);

    int totalTimeout = 0;
    //清扫队列中的正常点全部加入
    plannerQueue.clear();
    for (const auto &point: planPoints()) {
        plannerQueue.push_back(point);

        totalTimeout = totalTimeout + point.timeout;
    }

    unitTimeout = totalTimeout / planPoints().size();

    firstRetryCount = 0;
    backBaseRetryCount = 0;
    rechargeRetryCount = 0;

    HotWindNoteSingleton::instance().closeHotWind();

    //预埋点，执行当期任务的第一个点，触发 handlePoint 流程
    notify_one([this]() {
        pushPoint(flowSeizeSeatPoint);
    });
}

void AsyncTaskCall::handleAutoPoint(const RealPoint &point) {
    switch (point.id) {
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
    switch (point.id) {
        case FLOW_IN_BASE_POINT:
        case FLOW_CLOSE_MECHANISM:
        case FLOW_IN_STATION:
            handleFlowPoint(point);
            processControl(point);
            break;
        default:
            LOG(INFO) << "AsyncTaskCall : 手动接管期间不必要接受 " << output_interpolation_point(point.id) << " ...";
            break;
    }
}

void AsyncTaskCall::handlePointSpecialDevice(const RealPoint &point) {
    switch (point.id) {
        case FLOW_IN_BASE_POINT:
        case FLOW_CLOSE_MECHANISM:
        case FLOW_IN_STATION:
            handleFlowPoint(point);
            processControl(point);
            break;
        default:
            LOG(INFO) << "AsyncTaskCall : 强制模式下不必要接受 " << output_interpolation_point(point.id) << " ...";
            break;
    }
}

void AsyncTaskCall::initTaskPoint(const RealTask &realTask) {
    //预埋点流转循环，打开清洁机构，关闭清洁机构，出站
    realTask.assignmentPoint(flowSeizeSeatPoint, FLOW_SEIZE_SEAT);
    realTask.assignmentPoint(flowOpenMechanismPoint, FLOW_OPEN_MECHANISM);
    realTask.assignmentPoint(flowCloseMechanismPoint, FLOW_CLOSE_MECHANISM);
    realTask.assignmentPoint(flowOutStationPoint, FLOW_OUT_STATION);
    //结束睡眠模式，抵达摆渡点，进站
    realTask.assignmentPoint(flowEndSleepPoint, FLOW_END_SLEEP);
    realTask.assignmentPoint(flowInBasePoint, FLOW_IN_BASE_POINT);
    realTask.assignmentPoint(flowInStationPoint, FLOW_IN_STATION);
}


void AsyncTaskCall::goodGame(event::GG gg) {
    LOG(WARNING) << "AsyncTaskCall : goodGame " << gg;

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
        callSubsequentMode(baseTaskMode());
    }
}


void AsyncTaskCall::garbage(event::SB sb) {
    LOG(INFO) << "AsyncTaskCall : 程序出现严重错误，不可恢复，以下是现场可保存的信息 " << sb;
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

    LOG(INFO) << "AsyncTaskCall OpenMechanism: " << flowOpenMechanismPoint.arrive
              << " , CloseMechanism : " << flowCloseMechanismPoint.arrive
              << " , OutStation : " << flowOutStationPoint.arrive
              << " , EndSleep : " << flowEndSleepPoint.arrive
              << " , InBase : " << flowInBasePoint.arrive
              << " , InStation : " << flowInStationPoint.arrive;

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
    childPointQueue.clear();
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

    isCarpetAndPack = false;
}

void AsyncTaskCall::handlePlannerPoint(const RealPoint &point) {
    LOG(INFO) << "AsyncTaskCall : handlePlannerPoint "
              << " taskId: " << point.taskId << " Id: " << point.id
              << " arrive : " << point.arrive << " "
              << point.realProgress;

    PointProgressVo pointProgressVo(
            point.realPosition.x, point.realPosition.y,
            point.realProgress.currentStep, point.realProgress.totalStep,
            point.realProgress.currentFrequency, point.realProgress.totalFrequency,
            point.work_status, point.mode, point.inClean,
            point.taskId, point.renew, point.oldTaskId, point.newTaskId);
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
    int id = nextPoint.id;
    int timeout = nextPoint.timeout;
    if (timeout > 0) {
        async::TimerCall::instance().baseLoop()
                ->scheduleLater(std::chrono::seconds(timeout), [this, &id]() {
                    auto currentPoint = findFrontPoint();
                    if (currentPoint.id == id) {
                        executeOnNext(event::error::TIMEOUT);
                    }
                });
    }
}

void AsyncTaskCall::callPointComplete(const std::function<void()> &f) {
    plannerQueue.pop_front();
    f();
}

void AsyncTaskCall::callGoPath() {
    const vector<RealPoint> &points = std::vector<RealPoint>{plannerQueue.begin(), plannerQueue.end()};
    std::vector<Cp> cps;
    generateChildPointFlow(points, cps);

//    for (const auto &item: cps) {
//        LOG(ERROR) << "AsyncTaskCall : callGoPath : "
//                   << "  cp.id " << item.id
//                   << "  cp.pId " << item.pId
//                   << "  last " << item.last;
//    }

    childPointQueue.clear();
    for (const auto &cp: cps) {
        childPointQueue.push_back(cp);
    }

    PointPlanner::instance().goToPath(cps);
    async::TimerCall::instance().baseLoop()
            ->scheduleLater(std::chrono::seconds(plannerQueue.size() * unitTimeout), [this]() {
                executeOnPathDone(event::error::TIMEOUT);
            });
}

void AsyncTaskCall::callManualCleanStart() {
    HotWindNoteSingleton::instance().closeHotWind();
    if (!isWaitTask(currentFlow())) {
        cancelTask();
        goodGame(event::GG::gg_manual_mode);
    }
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

void AsyncTaskCall::callSubsequentSelfClean(const WorkStatus &status) {
    LOG(INFO) << "AsyncTaskCall : 处理 WorkStatus " << status << " ...";
    if (!isCharging()) {
        return;
    }
    if (status.getMopStatus() > 0) {
        if (ParamManager::instance().getDry() == -1) {
            return;
        }
        time_t now = time(0);
        tm *ltm = localtime(&now);
        std::cout << "年: " << 1900 + ltm->tm_year << std::endl;
        std::cout << "月: " << 1 + ltm->tm_mon << std::endl;
        std::cout << "日: " << ltm->tm_mday << std::endl;
        std::cout << "时间: " << ltm->tm_hour << ":" << ltm->tm_min << ":" << ltm->tm_sec << endl;
        if (ParamManager::instance().getDry() == 0 && ltm->tm_hour >= 7) {
            return;
        }
        HotWindNoteSingleton::instance().openHotWind();
    }
}

void AsyncTaskCall::callSelfCleanClose() {

}

void AsyncTaskCall::callSubsequentMode(int mode) {
    LOG(INFO) << "AsyncTaskCall : 处理 mode " << mode << " ...";

    if (mode == 6 && realPoints().size() == planPoints().size() &&
        Environment::instance().update_map) {
        LOG(INFO) << "AsyncTaskCall : 全覆盖清洁后需要更新地图信息 ...";
        CartographerPublisher::instance().publishUpdateMap();
    } else {
        callNeedPublishSleep();
        LOG(ERROR) << "AsyncTaskCall : gg";
    }
}

void AsyncTaskCall::callUrgencyStop() {
    if (!isPause()) {
        if (isContinueWork(event_flow, true)) {
            LOG(INFO) << "AsyncTaskCall : 手动暂停任务，增加暂停拦截 ...";
            LOG(INFO) << "AsyncTaskCall : event_flow : " << event_flow << "   " << recoverableEmergencyStop();
            setEpollManual(loop::manual_epoll::manual_pause);
            if (isRechargeFLow(event_flow)) {
                LOG(INFO) << "AsyncTaskCall : 回充中触发急停，为保证清洁机构确保收起，将回充重试次数设置为 0 ...";
                callCancelBackStation();
                rechargeRetryCount = 0;
                recordEmergencyStop(event::flow::flowing_water_production, flowInBasePoint);
            }
            callPause();
        }
    }
}

void AsyncTaskCall::callReleaseStop() {
    if (recoverableEmergencyStop()) {
        if (isPause()) {
            if (recoverableSuspend()) {
                LOG(INFO) << "AsyncTaskCall : 急停可恢复暂停状态 ... ";
//                if (!isReturningBase(event_flow)) {
                MechanismManager::instance().forceControlWorkStatus(baseWorkStatus(), isKnife());
//                }
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
        LOG(INFO) << "AsyncTaskCall : 可继续执行任务 ...";
        MechanismManager::instance().forceControlWorkStatus(baseWorkStatus(), isKnife());
        auto lastStack = lastEmergencyStop();
        LOG(INFO) << "AsyncTaskCall : 继续 lastStack : " << lastStack << " ...";

        if (isPlannerEmpty(lastStack.flow)) {
            LOG(INFO) << "AsyncTaskCall : 流水点的最后，点位规划队列为空，需要直接返回基站 ...";
            callBackBasePoint();
        } else {
            notify_one([this, &lastStack]() {
                setFlow(lastStack.flow);
                pushPoint(lastStack.suspendPoint);
            });
        }
    }
}

void AsyncTaskCall::callPause() {
    MechanismManager::instance().resetWorkStatus();
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
            childPointQueue.clear();
            setFlow(event::flow::flowing_water_production);
            recordEmergencyStop(event::flow::flowing_water_production, flowInBasePoint);
        }
        callBackBasePoint();
    } else {
        LOG(INFO) << "AsyncTaskCall : 已经触发返回基站的动作了 ...";
    }
}

void AsyncTaskCall::cancelTask() {
    if (isRegularTask(event_flow)) {
        PointPlanner::instance().cancelGoal();
        async::TimerCall::instance().baseLoop()->cancelAny();
        waitTaskQueue.clear();
    }
}

void AsyncTaskCall::forceInterruptTask(event::SB sb) {
    if (event_flow != event::flow::waiting_for_task &&
        event_flow != event::flow::hardware_interrupt_task &&
        event_flow != event::flow::software_interrupt_task) {
        LOG(INFO) << "AsyncTaskCall : 当前有任务取消任务 ... ";
        PointPlanner::instance().cancelGoal();
        async::TimerCall::instance().baseLoop()->cancelAny();
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
    notify_one([this, &task]() {
        pushTask(task);
    });
}

void AsyncTaskCall::executeOnNext(event::error error) {
    if (isCharging()) {
        return;
    }
    if (isWaitTask(event_flow)) {
        return;
    }
    if (isPreparation(event_flow)) {
        return;
    }
    if (isUnrecoverableError()) {
        return;
    }
    if (isUrgencyStop()) {
        return;
    }
    if (isManualMode()) {
        return;
    }

    if (error != event::error::TIMEOUT) {
        async::TimerCall::instance().baseLoop()->cancelAny();
    }

    if (!plannerQueue.empty()) {
        notify_one([this, &error]() {
            auto currentPoint = findFrontPoint();
            currentPoint.arrive = error == event::error::SUCCEEDED;
            pushPoint(currentPoint);
        });
    } else {
        notify_one([this, &error]() {
            flowInBasePoint.arrive = error == event::error::SUCCEEDED;
            pushPoint(flowInBasePoint);
        });
    }

}

void AsyncTaskCall::executePointFeedback(geometry_msgs::Pose2D pose) {
    fbPtr->triggerFeedback(pose);
}

void AsyncTaskCall::executeOnPathDone(event::error error) {
    if (isCharging()) {
        return;
    }
    if (isWaitTask(event_flow)) {
        return;
    }
    if (isPreparation(event_flow)) {
        return;
    }
    if (isUnrecoverableError()) {
        return;
    }
    if (isUrgencyStop()) {
        return;
    }
    if (isManualMode()) {
        return;
    }

    if (error != event::error::TIMEOUT) {
        async::TimerCall::instance().baseLoop()->cancelAny();
    }

    if (!plannerQueue.empty() && !childPointQueue.empty()) {
        notify_one([this, &error]() {
            if (error == event::error::SUCCEEDED) {
                childPointQueue.clear();

                while (plannerQueue.size() > 1) {
                    plannerQueue.pop_front();
                }
            } else {
                Cp &cp = childPointQueue.front();

                auto points = std::vector<RealPoint>{plannerQueue.begin(), plannerQueue.end()};
                plannerQueue.clear();

                for (auto &point: points) {
                    if (point.id == cp.pId) {
                        point.realPosition = cp.realPosition;
                        point.realOrientation = cp.realOrientation;
                        plannerQueue.push_back(point);
                    } else if (point.id > cp.pId) {
                        plannerQueue.push_back(point);
                    }
                }

                childPointQueue.clear();
            }
            auto currentPoint = findFrontPoint();
            currentPoint.arrive = error == event::error::SUCCEEDED;;
            pushPoint(currentPoint);
        });
    }
}

void AsyncTaskCall::executeOnPathFeedBack(int step, geometry_msgs::Pose pose) {
    lock([this, &step]() {
        if (!plannerQueue.empty() && !childPointQueue.empty()) {
            RealPoint &point = plannerQueue.front();
            Cp &cp = childPointQueue.front();

//            LOG(ERROR) << "AsyncTaskCall : executeOnPathFeedBack : "
//                       << "  step " << step
//                       << "  plannerQueue " << plannerQueue.size()
//                       << "  childPointQueue " << childPointQueue.size()
//                       << "  cp.id " << cp.id
//                       << "  cp.pId " << cp.pId
//                       << "  last " << cp.last;

            if (cp.id <= step) {
                childPointQueue.pop_front();
            }
            if (point.id < cp.pId && cp.last) {
                point.arrive = true;
                pushPoint(point);
                plannerQueue.pop_front();
                notify_one();
            }
        }
    });
}

void AsyncTaskCall::executeOutStation(bool result) {
    notify_one([this, &result]() {
        flowOutStationPoint.arrive = result;
        pushPoint(flowOutStationPoint);
    });
}

void AsyncTaskCall::executeInStation(bool result) {
    notify_one([this, &result]() {
        flowInStationPoint.arrive = result;
        pushPoint(flowInStationPoint);
    });
}

void AsyncTaskCall::executeCover() {
    LOG(INFO) << "AsyncTaskCall : 地图信息已更新完成，准备发布睡眠模式 ...";
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
    if (isCharging()) {
        return;
    }
    if (isUrgencyStop()) {
        return;
    }
    if (isUnrecoverableError()) {
        return;
    }
    if (isPlannerEmpty(event_flow)) {
        return;
    }
    if (isFlowingWater(event_flow)) {
        notify_one([this, &operation]() {
            pushSpecial(operation);
        });
    }
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
    if (isPlannerEmpty(event_flow)) {
        return;
    }
    if (isFlowingWater(event_flow)) {
//        "1.仅在尘推和湿拖模式下识别到地毯后抬起清洁机构；
//        2.识别到地毯后不关闭香氛或消杀。"
        if (baseWorkStatus().getPushStatus() > 0 || baseWorkStatus().getMopStatus() > 0) {
            if (carpet) {
                if (!isCarpetAndPack) {
                    isCarpetAndPack = true;
                    MechanismManager::instance().resetBelowWorkStatus();
                    LOG(INFO) << "NativeSystemManager : executeCarpet "
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
                    LOG(INFO) << "NativeSystemManager : executeCarpet "
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
    if (isPreparation(event_flow)) {
        return;
    }
    if (isReturningBase(event_flow)) {
        return;
    }
    if (lift) {
        notify_one([this]() {
            pushError(loop::error_epoll::error_lift);
        });
    }
}


RealTask AsyncTaskCall::runningTask() const {
    return runTask;
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
        for (const auto &item: realPoints()) {
            result.push_back(item);
        }
    }
    return result;
}
