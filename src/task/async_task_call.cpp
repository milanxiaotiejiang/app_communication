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
#include "model/CleanHistory.h"
#include "manager/InternalEventPubManager.h"
#include "manager/NoticeManager.h"
#include "model/Station.h"

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
    runTask.assignmentPoint(flowInterruptPoint, FLOW_INTERRUPT);
}

void AsyncTaskCall::handleManualOperation() {
    switch (epoll_manual) {
        case loop::manual_epoll::manual_resume:
//            callResume();
//            LOG(INFO) << "handlePoint flow : 手动继续任务，撤销暂停拦截 ...";
//            event_status = event::status::AUTO_STATE;//状态切换回自动状态
//
//            LOG(INFO) << "handlePoint flow : 继续 recordFlow : " << recordFlow << " recordPoint : " << recordPoint.getId()
//                      << " ...";
//
//            setPauseInterception(false);//暂停标志位置成false
//            setFlow(currentRecordFlow());//恢复流程为暂停前记录的流程
//            handleAutoPoint(currentRecordPoint());//恢复目标点为暂停前的目标点
            break;
        case loop::manual_epoll::manual_pause:
//            callPause();
//            LOG(INFO) << "handlePoint flow : 手动暂停任务，增加暂停拦截 ...";
//            event_status = event::status::MANUAL_STATE;//修改状态到MANUAL_STATE
//            setPauseInterception(true);//暂停标志位置为true
//            PointPlanner::instance().cancelGoal();//先取消当前的导航点
//            async::TimerCall::instance().baseLoop()->cancelAny();//停止超时计时器计时
//            if (!plannerQueue.empty()) {
//                auto currentPoint = findFrontPoint();//取消的的导航点再继续放入队列
//                plannerQueue.push_front(currentPoint);
//            }
//            setFlow(event::flow::manual_task_pause);//流程置回暂停
//            clean_history_db::CleanHistoryCenter::instance().addPause();
            break;
        case loop::manual_epoll::manual_back:
//            LOG(INFO) << "handlePoint flow : 手动取消任务，进入手动接管模式 ...";
//            peculiarTriggerBack([this]() {
//                event_status = event::status::MANUAL_STATE;
//                setFlow(event::flow::manual_over_and_move_base_point);
//                setPauseInterception(false);
//                LOG(INFO) << "handlePoint flow : 手动需要返回基站点 ...";
//            });
//            //清洁记录更新
//            internal_event::InternalEventPubManager::get_instance()->pubOper(internal_event::MANUAL_BACK_TO_BASE);
//            clean_history_db::CleanHistoryCenter::instance().manualBack();
            break;
        case loop::manual_epoll::manual_force_back:
//            LOG(INFO) << "handlePoint flow : 强制返回，进入强制接管模式 ...";
//            peculiarTriggerBack([this]() {
//                event_status = event::status::FORCE_STATE;
//                setFlow(event::flow::manual_control_over_and_move_base_point);
//                LOG(INFO)
//                        << "handlePoint flow : 不在基站位置，导致需要强制返回基站点 ...";
//            });
//            //清洁记录
//            clean_history_db::CleanHistoryCenter::instance().forceBack();
            break;
        default:
            LOG(INFO) << "handlePoint handleManualOperation : " << epoll_manual << " ...";
            break;
    }
}

void AsyncTaskCall::handleSpecialOperation() {
    switch (epoll_special) {
        case loop::special_epoll::special_low_battery:
            LOG(INFO) << "handlePoint flow : 低电量，进入强制接管模式 ...";
            peculiarTriggerBack([this]() {
                event_status = event::status::FORCE_STATE;
                setFlow(event::flow::force_over_and_move_base_point);
                LOG(INFO) << "handlePoint flow : 低电量导致需要强制返回基站点 ...";
            });
            //清洁记录
            internal_event::InternalEventPubManager::get_instance()->pubOper(internal_event::LOW_BATTERY_BACK_CHARGE);
            clean_history_db::CleanHistoryCenter::instance().lowPowerBack();
            break;
        case loop::special_epoll::special_branch_water:
        case loop::special_epoll::special_sewage_water:
        case loop::special_epoll::special_branch_sewage_water:
        case loop::special_epoll::special_dust_push_anomaly:
            LOG(INFO) << "handlePoint flow : 污水箱满/清水箱空/电机堵转，进入强制接管模式 ...";
            peculiarTriggerBack([this]() {
                event_status = event::status::FORCE_STATE;
                setFlow(event::flow::force_over_and_move_base_point);
                LOG(INFO) << "handlePoint flow : 污水箱满/清水箱空/电机堵转导致需要强制返回基站点 ...";
            });
            //清洁记录
            if (epoll_special == loop::special_epoll::special_branch_water) {
                internal_event::InternalEventPubManager::get_instance()->pubOper(
                        internal_event::CLEAN_WATER_LEVEL_CHECK_FAILED);
            }
            if (epoll_special == loop::special_epoll::special_sewage_water) {
                internal_event::InternalEventPubManager::get_instance()->pubOper(
                        internal_event::DIRTY_WATER_LEVEL_CHECK_FAILED);
            }
            if (epoll_special == loop::special_epoll::special_branch_sewage_water) {
                internal_event::InternalEventPubManager::get_instance()->pubOper(
                        internal_event::CLEAN_WATER_LEVEL_CHECK_FAILED);
                internal_event::InternalEventPubManager::get_instance()->pubOper(
                        internal_event::DIRTY_WATER_LEVEL_CHECK_FAILED);
            }
            if (epoll_special == loop::special_epoll::special_dust_push_anomaly) {
                internal_event::InternalEventPubManager::get_instance()->pubOper(
                        internal_event::MOTOR_ERROR_RECOVERY_FAILED);
            }
            //todo
//            clean_history_db::CleanHistoryCenter::instance().equipmentErrorBack(point.getSpecialInfo());
            break;
        default:
            LOG(INFO) << "handlePoint handleSpecialOperation : " << epoll_special << " ...";
            break;
    }
}

void AsyncTaskCall::handleErrorOperation() {
    switch (epoll_error) {
        case loop::error_epoll::error_manual_clean_start:
//            callManualCleanStart();
//            if (event_flow != event::flow::waiting_for_task || workTask.getId() != "") {
//                waitTaskQueue.clear();
//                PointPlanner::instance().cancelGoal();               //先取消当前的导航点
//                async::TimerCall::instance().baseLoop()->cancelAny();//停止超时计时器计时
//                setPauseInterception(false);                         //暂停标志位置为false
//                //结束当前任务
//                internal_event::InternalEventPubManager::get_instance()->pubOper(internal_event::ENTER_MANUAL_CLEAN_MODE);
//                internal_event::InternalEventPubManager::get_instance()->taskStop(workTask.getId());
//                clean_history_db::CleanHistoryCenter::instance().enterManualCleanMode();
//                reset();
//            }
//            //关闭清洁机构
//            MechanismManager::instance().resetWorkStatus();
//            //电机失能
//            MechanismManager::instance().enterManualControl();
//            //标志位置true
//            setManualClean(true);
//            event_status = event::status::MANUAL_STATE;
//            setFlow(event::flow::manual_cleaning);
//
//            LOG(INFO) << "handlePoint flow : 进入手动模式";
            break;
        case loop::error_epoll::error_manual_clean_end:
//            callManualCleanEnd();
//            //关闭清洁机构
//            MechanismManager::instance().resetWorkStatus();
//            //电机使能
//            MechanismManager::instance().quitManualControl();
//            //睡眠模式标志设置
//            ZooInnerStatus::instance().setIsFirstSwitchMode(true);
//            //标志位置true
//            setManualClean(false);
//            event_status = event::status::AUTO_STATE;
//            setFlow(event::flow::waiting_for_task);
//
//            LOG(INFO) << "handlePoint flow : 退出手动模式";
            break;
        case loop::error_epoll::error_unrecoverable:
            break;
        default:
            LOG(INFO) << "handlePoint handleErrorOperation : " << epoll_error << " ...";
            break;
    }

//    setFlow(event::flow::hardware_interrupt_task);
//    if (event_flow != event::flow::waiting_for_task || !workTask.getId().empty()) {
//        waitTaskQueue.clear();
//        PointPlanner::instance().cancelGoal();               //先取消当前的导航点
//        async::TimerCall::instance().baseLoop()->cancelAny();//停止超时计时器计时
//        setPauseInterception(false);                         //暂停标志位置为false
//        //结束当前任务
//        internal_event::InternalEventPubManager::get_instance()->pubAlarm(
//                SelfCheckErrorType::SOFTWARE_INTERRUPT);
//        internal_event::InternalEventPubManager::get_instance()->taskStop(workTask.getId());
//        clean_history_db::CleanHistoryCenter::instance().laserInterrupt();
//        reset();
//    }
//    internal_event::InternalEventPubManager::get_instance()->pubAlarm(SelfCheckErrorType::LASER_RESTART_FAILED);
//
//    //关闭清洁机构
//    MechanismManager::instance().resetWorkStatus();
//    //睡眠模式标志设置
////            ZooInnerStatus::instance().setIsFirstSwitchMode(true);
//    //标志位重置
//    urgencyStopEventFlow = event::flow::waiting_for_task;
//    urgencyStopStatus = event::status::AUTO_STATE;
//    urgencyStopPoint = flowSeizeSeatPoint;
//
//    LOG(INFO) << "handlePoint flow :  unrecoverable error";
}

void AsyncTaskCall::handleStop() {
    switch (urgency_stop) {
        case loop::urgency_stop::trigger_urgency_stop:
//            LOG(INFO) << "急停了 ... ";
//            setFlow(event::flow::force_task_pause);//如果是急停，flow设置为force_task_pause
//            PointPlanner::instance().cancelGoal();//取消当前点导航
//            async::TimerCall::instance().baseLoop()->cancelAny();//判断超时的计时器取消
//            clean_history_db::CleanHistoryCenter::instance().addUrgencyStop();//历史记录增加，急停一次
//            internal_event::InternalEventPubManager::get_instance()->pubOper(internal_event::URGENCY_STOP);
            break;
        case loop::urgency_stop::release_urgency_stop:
//            if (event_flow != event::flow::waiting_for_task || workTask.getId() != "") {
//                waitTaskQueue.clear();
//                PointPlanner::instance().cancelGoal();               //先取消当前的导航点
//                async::TimerCall::instance().baseLoop()->cancelAny();//停止超时计时器计时
//                setPauseInterception(false);                         //暂停标志位置为false
//                //结束当前任务
//                internal_event::InternalEventPubManager::get_instance()->taskStop(workTask.getId());
//                clean_history_db::CleanHistoryCenter::instance().specialComplete();
//                reset();
//            }
//            //关闭清洁机构
//            MechanismManager::instance().resetWorkStatus();
//            //睡眠模式标志设置
//            ZooInnerStatus::instance().setIsFirstSwitchMode(true);
//            //标志位重置
//            urgencyStopEventFlow = event::flow::waiting_for_task;
//            urgencyStopStatus = event::status::AUTO_STATE;
//            urgencyStopPoint = flowSeizeSeatPoint;
//
//            LOG(INFO) << "handlePoint flow :  callStopUrgentInBase";
            break;
        case loop::urgency_stop::recovery_urgency_stop:
//            //急停解除
//            clean_history_db::CleanHistoryCenter::instance().cancelUrgencyStop();
//            internal_event::InternalEventPubManager::get_instance()->pubOper(internal_event::CANCEL_URGENCY_STOP);
//            LOG(INFO) << "解除急停了 ... ";
//            if (runTask.getId().empty()) {
//                setFlow(event::flow::waiting_for_task);
//                event_status = event::status::AUTO_STATE;
//                recordLockState(flowSeizeSeatPoint);
//                //没有任务，收回所有清洁机构
//                MechanismManager::instance().resetWorkStatus();
//            } else {
//                //设置 recordLockState 的状态和点位
//                LOG(INFO) << "解除急停的时候还没有推回基站...";
//                //要先进入暂停模式，解除暂停模式后才能继续运行
//                if (manualClean() ||
//                    currentInterception() ||
//                    plannerQueue.empty() ||
//                    (event_flow == event::flow::waiting_for_task ||
//                     event_flow == event::flow::out_base_station ||
//                     event_flow == event::flow::switch_node_work_mode ||
//                     event_flow == event::flow::preliminary_preparation_completed ||
//                     event_flow == event::flow::arrive_base_point_success ||
//                     event_flow == event::flow::try_recharging_again ||
//                     event_flow == event::flow::arrive_base_station_success ||
//                     event_flow == event::flow::manual_mechanism_close_and_charging ||
//                     event_flow == event::flow::force_base_point_and_close_mechanism ||
//                     event_flow == event::flow::force_mechanism_close_and_charging ||
//                     event_flow == event::flow::manual_control_base_point_and_charging ||
//                     event_flow == event::flow::manual_control_over_success ||
//                     event_flow == event::flow::hardware_interrupt_task ||
//                     event_flow == event::flow::software_interrupt_task)
//                        ) {
//                    //手动模式没法暂停
//                    //已经暂停的情况没法进入暂停状态
//                    //没有在自动清扫任务中，不支持暂停
//                    //没有完成出站等前期工作不支持暂停
//                    int noticeCode = 6666;
//                    time_t t;
//                    t = time(NULL);
//                    long noticeTime = t;
//                    std::string noticeTitle = "pause";
//                    std::string noticeMessage = "unsupport pause mode";
//                    std::string solution = "";
//                    NoticeManager::get_instance()->sendNotice(noticeCode, noticeTime, noticeTitle, noticeMessage,
//                                                              solution);
//                    // set to front point
//                    setFlow(stopEventFlow());
//                    event_status = stopStatus();
//                    pointEpollDeque.push_back(stopPoint());
//                } else {
//                    LOG(INFO) << "handlePoint flow : 手动暂停任务，增加暂停拦截 ...";
//                    event_status = event::status::MANUAL_STATE; //修改状态到MANUAL_STATE
//                    setPauseInterception(true);      //暂停标志位置为true
//                    PointPlanner::instance().cancelGoal(); //先取消当前的导航点
//                    async::TimerCall::instance().baseLoop()->cancelAny(); //停止超时计时器计时
//                    if (!plannerQueue.empty()) {
//                        auto currentPoint = findFrontPoint(); //取消的的导航点再继续放入队列
//                        plannerQueue.push_front(currentPoint);
//                    }
//                    setFlow(event::flow::manual_task_pause); //流程置回暂停
//                    //重新打开所有清洁机构
//                    MechanismManager::instance().forceControlWorkStatus(workTask.getWorkStatus());
//                }
//            }
            break;
        default:
            break;
    }
}

void AsyncTaskCall::handleTask(const RealTask &realTask) {
    if (epoll_error != loop::error_epoll::error_normal) {
        LOG(INFO) << "程序达到不可恢复状态，不能接受任何数据，当前状态 " << epoll_error;
        return;
    }
    if (urgency_stop != loop::urgency_stop::urgency_normal) {

        LOG(INFO) << "急停拦截，不能接受 task 了，看看前面哪里有错误 " << realTask.getId();
        return;
    }
    if (event_flow == event::flow::waiting_for_task) {

        runTask = realTask;
        //记录开始执行时间
        internal_event::InternalEventPubManager::get_instance()->taskStart(realTask.getId());
        clean_history_db::CleanHistoryCenter::instance().executeTask(realTask);
        //在这里添加一条清洁记录
        //addNewCleanHistory(task);
        //预埋点流转循环，打开清洁机构，关闭清洁机构，出站
        runTask.assignmentPoint(flowSeizeSeatPoint, FLOW_SEIZE_SEAT);
        runTask.assignmentPoint(flowOpenMechanismPoint, FLOW_OPEN_MECHANISM);
        runTask.assignmentPoint(flowCloseMechanismPoint, FLOW_CLOSE_MECHANISM);
        runTask.assignmentPoint(flowOutStationPoint, FLOW_OUT_STATION);
        //结束睡眠模式，抵达摆渡点，进站，中断
        runTask.assignmentPoint(flowEndSleepPoint, FLOW_END_SLEEP);
        runTask.assignmentPoint(flowInBasePoint, FLOW_IN_BASE_POINT);
        runTask.assignmentPoint(flowInStationPoint, FLOW_IN_STATION);
        runTask.assignmentPoint(flowInterruptPoint, FLOW_INTERRUPT);
        //清扫队列中的正常点全部加入
        plannerQueue.clear();
        for (const auto &point: runTask.getPlanPoints()) {
            plannerQueue.push_back(point);
        }

        firstRetryCount = 0;
        backBaseRetryCount = 0;
        rechargeRetryCount = 0;

        //预埋点，执行当期任务的第一个点，触发 handlePoint 流程
        handleAutoPoint(flowSeizeSeatPoint);
    } else {
        waitTaskQueue.push_back(realTask);
    }
}

void AsyncTaskCall::handlePoint(const RealPoint &realPoint) {
//    if (epoll_error != loop::error_epoll::error_normal) {
//        LOG(INFO) << "程序达到不可恢复状态，不能接受任何数据，当前状态 " << epoll_error;
//        return;
//    }
//    recordLockState(realPoint);//记录当前运行状态，为了暂停或者急停后可以恢复到上一次运行状态中去
//    if (event_status == event::status::AUTO_STATE) {
//        handleAutoPoint(realPoint);
//    } else if (event_status == event::status::MANUAL_STATE) {
//        handleManualPoint(realPoint);
//    } else if (event_status == event::status::FORCE_STATE) {
//        handleForcePoint(realPoint);
//    }
}

void AsyncTaskCall::handleAutoPoint(const RealPoint &point) {
//    //根据ID判断执行什么样的策略
//    //手动暂停和手动继续和手动返回基站，执行handleSpecialPoint函数，处理特殊点位
//    switch (point.getId()) {
//        //中断，进基站，返回摆渡点，结束睡眠模式，出站，开关清洁机构，抢占，为流程点位，执行流程工作
//        case FLOW_INTERRUPT:
//        case FLOW_IN_STATION:
//        case FLOW_IN_BASE_POINT:
//        case FLOW_END_SLEEP:
//        case FLOW_OUT_STATION:
//        case FLOW_CLOSE_MECHANISM:
//        case FLOW_OPEN_MECHANISM:
//        case FLOW_SEIZE_SEAT:
//            recordAutoState(point); //先记录走到哪个流程
//            handleFlowPoint(point); //随后进行处理
//            break;
//            //正常规划出的点位
//        default:
//            recordAutoState(point);
//            handlePlannerPoint(point);
//            break;
//    }
//    //流程控制
//    processControl(point);
}

void AsyncTaskCall::handleManualPoint(const RealPoint &point) {
    switch (point.getId()) {
        case FLOW_IN_BASE_POINT: //进站-到达摆渡点
            peculiarDisposeBasePoint(point, event::flow::manual_base_point_and_close_mechanism, true);
            break;
        case FLOW_CLOSE_MECHANISM: //关闭清洁机构
            peculiarDisposeMechanism(point, event::flow::manual_mechanism_close_and_charging);
            break;
        case FLOW_IN_STATION: //进站
            peculiarDisposeStation(point, event::flow::manual_over_success,
                                   event::flow::manual_back_try_move_base_point);
            break;
        default:
            LOG(INFO) << "handlePoint flow : 手动接管期间不必要接受的点位 "
                      << point.getId() << " ...";
            break;
    }
}

//低电量回充
void AsyncTaskCall::handleForcePoint(const RealPoint &point) {
    switch (point.getId()) {
        case FLOW_IN_BASE_POINT:
            if (event_flow == event::flow::force_over_and_move_base_point ||
                event_flow == event::flow::force_back_try_move_base_point) {
                peculiarDisposeBasePoint(point, event::flow::force_base_point_and_close_mechanism, true);
            } else if (event_flow == event::flow::manual_control_over_and_move_base_point ||
                       event_flow == event::flow::manual_control_back_try_move_base_point) {
                peculiarDisposeBasePoint(point, event::flow::manual_control_base_point_and_charging, false);
            } else {
                LOG(ERROR) << "handlePoint flow : status error ... ";
            }
            break;
        case FLOW_CLOSE_MECHANISM:
            peculiarDisposeMechanism(point, event::flow::force_mechanism_close_and_charging);
            break;
        case FLOW_IN_STATION:
            if (event_flow == event::flow::force_mechanism_close_and_charging) {
                peculiarDisposeStation(point, event::flow::force_over_success,
                                       event::flow::force_back_try_move_base_point);
            } else if (event_flow == event::flow::manual_control_base_point_and_charging) {
                peculiarDisposeStation(
                        point, event::flow::manual_control_over_success,
                        event::flow::manual_control_back_try_move_base_point);
            }
            break;
        default:
            LOG(INFO) << "handlePoint flow : 强制模式下不必要接受的点位 " << point.getId() << " ...";
            break;
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

void AsyncTaskCall::executeUrgencyStop(bool isUrgencyStop) {
    if (urgency_stop == loop::urgency_stop::trigger_urgency_stop) {
        return;
    }
    if (urgency_stop == loop::urgency_stop::release_urgency_stop) {
        return;
    }
    notify_one([&isUrgencyStop, this]() {
        if (isUrgencyStop) {
            pushUrgencyStop(loop::urgency_stop::trigger_urgency_stop);
        } else {
            pushUrgencyStop(loop::urgency_stop::recovery_urgency_stop);
        }
    });
}

void AsyncTaskCall::executeOneTask(const RealTask &task) {
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
        if (event_flow != event::flow::manual_cleaning) {
            notify_one([this, &error]() {
                flowInBasePoint.realError.arrive = error == event::error::SUCCEEDED;
                pushPoint(flowInBasePoint);
            });
        }
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

RealPoint AsyncTaskCall::findFrontPoint() {
    return plannerQueue.front();
}

RealPoint AsyncTaskCall::findFrontNextPoint() {
    plannerQueue.pop_front();
    return plannerQueue.front();
}

void AsyncTaskCall::exchangeFrontPoint(const RealPoint &point) {
    plannerQueue.pop_front();
    plannerQueue.push_front(point);
}

void AsyncTaskCall::goodGame() {
    switch (event_flow) {
        case event::flow::waiting_for_task : {
            LOG(ERROR) << "goodGame -> waiting_for_task";
            break;
        }
        case event::flow::out_base_station : {
            LOG(ERROR) << "goodGame -> out_base_station";
            break;
        }
        case event::flow::switch_node_work_mode : {
            LOG(ERROR) << "goodGame -> switch_node_work_mode";
            break;
        }
        case event::flow::preliminary_preparation_completed : {
            LOG(ERROR) << "goodGame -> preliminary_preparation_completed";
            break;
        }
        case event::flow::ensure_move_to_start_point: {
            LOG(ERROR) << "goodGame -> ensure_move_to_start_point";
            break;
        }
        case event::flow::cleaning_mechanism_ready: {
            LOG(ERROR) << "goodGame -> cleaning_mechanism_ready";
            break;
        }
        case event::flow::flowing_water_production: {
            LOG(ERROR) << "goodGame -> flowing_water_production";
            break;
        }
        case event::flow::again_move_to_start_point: {
            LOG(ERROR) << "goodGame -> again_move_to_start_point";
            break;
        }
        case event::flow::again_prepare_cleaning_mechanism: {
            LOG(ERROR) << "goodGame -> again_prepare_cleaning_mechanism";
            break;
        }
        case event::flow::flowing_water_execution_completed: {
            LOG(ERROR) << "goodGame -> flowing_water_execution_completed";
            break;
        }
        case event::flow::arrive_base_point_success: {
            LOG(ERROR) << "goodGame -> arrive_base_point_success";
            break;
        }
        case event::flow::arrive_base_station_success: {
            LOG(ERROR) << "goodGame -> arrive_base_station_success";
            break;
        }
        case event::flow::try_recharging_again: {
            LOG(ERROR) << "goodGame -> try_recharging_again";
            break;
        }
        case event::flow::try_move_base_point_again: {
            LOG(ERROR) << "goodGame -> try_move_base_point_again";
            break;
        }
        case event::flow::manual_over_and_move_base_point: {
            LOG(ERROR) << "goodGame -> manual_over_and_move_base_point";
            break;
        }
        case event::flow::manual_back_try_move_base_point: {
            LOG(ERROR) << "goodGame -> manual_back_try_move_base_point";
            break;
        }
        case event::flow::manual_base_point_and_close_mechanism: {
            LOG(ERROR) << "goodGame -> manual_base_point_and_close_mechanism";
            break;
        }
        case event::flow::manual_mechanism_close_and_charging: {
            LOG(ERROR) << "goodGame -> manual_mechanism_close_and_charging";
            break;
        }
        case event::flow::manual_over_success: {
            LOG(ERROR) << "goodGame -> manual_over_success";
            break;
        }
        case event::flow::manual_task_pause: {
            LOG(ERROR) << "goodGame -> manual_task_pause";
            break;
        }
        case event::flow::manual_cleaning: {
            LOG(ERROR) << "goodGame -> manual_cleaning";
            break;
        }
        case event::flow::force_over_and_move_base_point: {
            LOG(ERROR) << "goodGame -> force_over_and_move_base_point";
            break;
        }
        case event::flow::force_back_try_move_base_point: {
            LOG(ERROR) << "goodGame -> force_back_try_move_base_point";
            break;
        }
        case event::flow::force_base_point_and_close_mechanism: {
            LOG(ERROR) << "goodGame -> force_base_point_and_close_mechanism";
            break;
        }
        case event::flow::force_mechanism_close_and_charging: {
            LOG(ERROR) << "goodGame -> force_mechanism_close_and_charging";
            break;
        }
        case event::flow::force_over_success: {
            LOG(ERROR) << "goodGame -> force_over_success";
            break;
        }
        case event::flow::force_task_pause: {
            LOG(ERROR) << "goodGame -> force_task_pause";
            break;
        }
        case event::flow::manual_control_over_and_move_base_point: {
            LOG(ERROR) << "goodGame -> manual_control_over_and_move_base_point";
            break;
        }
        case event::flow::manual_control_back_try_move_base_point: {
            LOG(ERROR) << "goodGame -> manual_control_back_try_move_base_point";
            break;
        }
        case event::flow::manual_control_base_point_and_charging: {
            LOG(ERROR) << "goodGame -> manual_control_base_point_and_charging";
            break;
        }
        case event::flow::manual_control_over_success: {
            LOG(ERROR) << "goodGame -> manual_control_over_success";
            break;
        }
        case event::flow::hardware_interrupt_task: {
            LOG(ERROR) << "goodGame -> hardware_interrupt_task";
            break;
        }
        case event::flow::software_interrupt_task: {
            LOG(ERROR) << "goodGame -> software_interrupt_task";
            break;
        }
    }
    internal_event::InternalEventPubManager::get_instance()->taskStop(runTask.getId());
    clean_history_db::CleanHistoryCenter::instance().complete();
    runTask;

    event_status = event::status::AUTO_STATE;
    setFlow(event::flow::waiting_for_task);
    reset();

    if (!waitTaskQueue.empty()) {
        notify_one([this]() {
            auto task = waitTaskQueue.front();
            pushTask(task);
            waitTaskQueue.pop_front();
        });
    } else {
        SwitchModePublish::instance().publish();
        LOG(ERROR) << "gg";
    }
}

void AsyncTaskCall::reset() {
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
    flowInterruptPoint.realError.arrive = false;
//    setPauseInterception(false);
//    setManualClean(false);
//    workTask.setId("");
}

void AsyncTaskCall::manualBackToBase(bool force) {
    if (force) {
        notify_one([this]() {
            pushManual(loop::manual_epoll::manual_force_back);
        });
    } else {
        if (ZooInnerStatus::instance().getUrgencyStopStatus()) {
            throw app::exception(make_error_code(error::machine_is_in_emergency_stop_command_not_supported));
        }
        if (ZooInnerStatus::instance().getIsCharging()) {
            throw app::exception(make_error_code(error::already_in_the_base_station));
        }
        //todo
//        if (specialBackPoint.realError.arrive) {
//            throw app::exception(make_error_code(error::already_returning_to_the_base_station));
//        }
//        if (specialLowBatteryPoint.realError.arrive) {
//            throw app::exception(make_error_code(error::already_returning_to_the_base_station));
//        }
//        if (specialMCFBackPoint.realError.arrive) {
//            throw app::exception(make_error_code(error::already_returning_to_the_base_station));
//        }
        if (event_status == event::status::FORCE_STATE) {
            throw app::exception(make_error_code(error::the_current_state_is_uncontrollable));
        }
        if (event_flow == event::flow::waiting_for_task) {
            if (isBasePointReached(0.3, 0.3)) {
                throw app::exception(make_error_code(error::already_in_the_base_station));
            } else {
                notify_one([this]() {
                    pushManual(loop::manual_epoll::manual_force_back);
                });
            }
        } else {
            notify_one([this]() {
                pushManual(loop::manual_epoll::manual_back);
            });
        }
    }
}

//队列前方加入恢复运行的点
void AsyncTaskCall::manualResume() {
//    if (ZooInnerStatus::instance().getUrgencyStopStatus()) {
//        throw app::exception(make_error_code(error::machine_is_in_emergency_stop_command_not_supported));
//    }
//    if (event_status == event::status::FORCE_STATE || manualClean()) {//手动模式暂停已经被解除了，没法再解除一次
//        throw app::exception(make_error_code(error::the_current_state_is_uncontrollable));
//    }
//
//    if (!currentInterception()) {
//        throw app::exception(make_error_code(error::not_paused_status));
//    }
//    notify_one([this]() {
//        manualEpollDeque.push_back(loop::manual_epoll::manual_resume);
//    });
}

//手动暂停，队列中加入一个暂停点
void AsyncTaskCall::manualPause() {
//    if (ZooInnerStatus::instance().getUrgencyStopStatus()) {
//        throw app::exception(make_error_code(error::machine_is_in_emergency_stop_command_not_supported));
//    }
//    if (event_status == event::status::FORCE_STATE || manualClean()) {//手动模式没法暂停
//        throw app::exception(make_error_code(error::the_current_state_is_uncontrollable));
//    }
//    if (currentInterception()) {
//        throw app::exception(make_error_code(error::it_is_currently_suspended));
//    }
//    if (plannerQueue.empty()) {
//        throw app::exception(make_error_code(error::pause_is_not_supported));
//    }
//    if (event_flow < event::flow::preliminary_preparation_completed) {
//        throw app::exception(make_error_code(error::pause_is_not_supported));
//    }
//    notify_one([this]() {
//        manualEpollDeque.push_back(loop::manual_epoll::manual_pause);
//    });
}

void AsyncTaskCall::enterManual() {//进入手动模式接口
//    if (ZooInnerStatus::instance().getUrgencyStopStatus()) {//急停状态下没法进入手动模式
//        throw app::exception(make_error_code(error::machine_is_in_emergency_stop_command_not_supported));
//    }
//    if (event_status == event::status::FORCE_STATE) {//强制模式下没法进入手动模式
//        throw app::exception(make_error_code(error::the_current_state_is_uncontrollable));
//    }
//    if (manualClean()) {//已经在手动模式下
//        throw app::exception(make_error_code(error::already_in_manual_clean_mode));
//    }
//    notify_one([this]() {
//        errorEpollDeque.push_back(loop::error_epoll::error_manual_clean_start);
//    });
}

void AsyncTaskCall::quitManual() {//退出手动模式接口
//    if (ZooInnerStatus::instance().getUrgencyStopStatus()) {
//        throw app::exception(make_error_code(error::machine_is_in_emergency_stop_command_not_supported));
//    }
//    if (event_flow != event::flow::manual_cleaning) {
//        throw app::exception(make_error_code(error::not_in_manual_clean_mode));
//    }
//    if (!manualClean()) {//不在手动模式下
//        throw app::exception(make_error_code(error::not_in_manual_clean_mode));
//    }
//    notify_one([this]() {
//        errorEpollDeque.push_back(loop::error_epoll::error_manual_clean_end);
//    });
}

void AsyncTaskCall::urgencyStopAndCharge() {
//    //不在急停或充电不进入
//    if (!ZooInnerStatus::instance().getUrgencyStopStatus() ||
//        !ZooInnerStatus::instance().getIsCharging()) {
//        return;
//    }
//    //没任务不进入
//    if (workTask.getId().empty()) {
//        return;
//    }
//    notify_one([this]() {
//        urgencyStopDeque.push_back(loop::urgency_stop::release_urgency_stop);
//    });
}

void AsyncTaskCall::callManualCleanStart() {//进入手动模式

}

void AsyncTaskCall::callManualCleanEnd() {//退出手动模式

}

void AsyncTaskCall::callStopUrgentInBase() {

}


std::string AsyncTaskCall::runTaskId() {
    if (event_flow == event::flow::waiting_for_task || event_flow == event::flow::manual_cleaning) {
        return "";
    }
    return runTask.getId();
}

std::vector<RealTask> AsyncTaskCall::runTaskList() {
    std::vector<RealTask> result;
    if (event_flow != event::flow::waiting_for_task && event_flow != event::flow::manual_cleaning) {
        result.push_back(runTask);
    }
    return result;
}

std::vector<RealPoint> AsyncTaskCall::runTaskPoint() {
    std::vector<RealPoint> result;
    if (event_flow != event::flow::waiting_for_task && event_flow != event::flow::manual_cleaning) {
        for (const auto &item: runTask.getRealPoints()) {
            result.push_back(item);
        }
    }
    return result;
}

void AsyncTaskCall::forceBackToBase(loop::special_epoll operation) {
    // 低电量时候一次任务只能触发一次强制返回基站
    // 在返回基站时，不能再次接受返回基站了
    // 急停中也不能接受返回基站指令
    // 没有任务时候也不能触发低电量回充
    // 在基站也不能触发返回基站流程（此处应该在源头有判断）
    //以后所有的强制返回基站都塞到这个接口，通过force_type区分
    if (ZooInnerStatus::instance().getUrgencyStopStatus()) {
        return;
    }
    //返回基站中，急停中，暂停中都不需要再强制返回基站
    if (event_flow == event::flow::waiting_for_task ||
        event_flow == event::flow::manual_cleaning ||
        event_flow == event::flow::flowing_water_execution_completed ||
        event_flow == event::flow::arrive_base_point_success ||
        event_flow == event::flow::try_recharging_again ||
        event_flow == event::flow::try_move_base_point_again ||
        event_flow == event::flow::manual_over_and_move_base_point ||
        event_flow == event::flow::manual_back_try_move_base_point ||
        event_flow == event::flow::manual_base_point_and_close_mechanism ||
        event_flow == event::flow::manual_mechanism_close_and_charging ||
        event_flow == event::flow::manual_control_over_and_move_base_point ||
        event_flow == event::flow::manual_control_base_point_and_charging ||
        event_flow == event::flow::manual_task_pause ||
        event_flow == event::flow::force_task_pause) {
        return;
    }
    pushSpecial(operation);
}

void AsyncTaskCall::handlePlannerPoint(const RealPoint &point) {
    LOG(INFO) << "handlePlannerPoint "
              << " taskId: " << point.getTaskId() << " Id: " << point.getId()
              << " arrive : " << point.realError.arrive << " "
              << point.realProgress;
    //当前进度和清洁面积更新到历史记录中
    clean_history_db::CleanHistoryCenter::instance().updateCleanHistory(point);

    PointProgressVo pointProgressVo(
            point.task_id, point.realPosition.x, point.realPosition.y,
            point.realProgress.currentStep, point.realProgress.totalStep,
            point.realProgress.currentFrequency, point.realProgress.totalFrequency,
            point.work_status, point.mode, point.inClean);
    PointProgressPublish::instance().publishProgressPoint(pointProgressVo);

    runTask.changeArrivalStatus(point);
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


//前往第一个清扫点
void AsyncTaskCall::callGoFirstPoint() {
    RealPoint front = plannerQueue.front();
    PointPlanner::instance().gotoPlannerPoint(front);
}

void AsyncTaskCall::callRetryFirstPoint(const std::function<void()> &f) {
    auto currentPoint = findFrontPoint();
    if (firstRetryCount < MAX_FIRST_RETRY_COUNT) {
        LOG(INFO) << "handlePoint flow : 未到达第一个点位，重试中 ...";
        firstRetryCount++;
        exchangeFrontPoint(currentPoint);
        PointPlanner::instance().gotoPlannerPoint(currentPoint);
    } else {
        plannerQueue.clear();
        f();
    }
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


void AsyncTaskCall::callTaskInterrupt(const RealPoint &point) {
    event_status = event::status::AUTO_STATE;
    waitTaskQueue.clear();
    auto error_pair = generateErrorByRealPoint(point);
    internal_event::InternalEventPubManager::get_instance()->taskStop(runTask.getId());
    clean_history_db::CleanHistoryCenter::instance().errorComplete(
            std::get<0>(error_pair), std::get<1>(error_pair), std::get<2>(error_pair));
    reset();
    LOG(INFO) << "handlePoint flow : 程序出现严重错误，不可恢复 错误代码 " << point.getId();
    internal_event::InternalEventPubManager::get_instance()->pubAlarm(SelfCheckErrorType::SOFTWARE_INTERRUPT);
}

void AsyncTaskCall::callPause() {

}

void AsyncTaskCall::callResume() {

}

//返回基站，取消当前规划，计时器，清空队列，返回基站
void AsyncTaskCall::peculiarTriggerBack(const function<void()> &f) {
    f();
    PointPlanner::instance().cancelGoal();
    async::TimerCall::instance().baseLoop()->cancelAny();
    waitTaskQueue.clear();
    plannerQueue.clear();
    callBackBasePoint();
}

void AsyncTaskCall::peculiarDisposeBasePoint(const RealPoint &point,
                                             event::flow needFlow,
                                             bool whetherDisposeMechanism) {
    geometry_msgs::Pose2D current_pose = MapAttribute::instance().getRobotPositionPose();
    //判断一下是否真的在基站点
    bool isInBasePoint =
            ((abs(current_pose.x + 0.3) < 0.5) && (abs(current_pose.y) < 0.5) &&
             (abs(current_pose.theta) < 1.0));
    if (isInBasePoint == false) {
        LOG(INFO) << "没有完全返回摆渡点，误报！！！！！！！！";
    }
    if (point.realError.arrive && isInBasePoint) {
        //记录成功到达摆渡点
        clean_history_db::CleanHistoryCenter::instance().setBackBasePointArrived(
                clean_history_db::SUCCEED);
        if (whetherDisposeMechanism) {
            LOG(INFO) << "handlePoint flow : 完美返回基站点位，准备收起清洁机构 ...";
            setFlow(needFlow);
            callCloseMechanism([this]() {
                flowCloseMechanismPoint.realError.arrive = true;
                pushPoint(flowCloseMechanismPoint);
            });
        } else {
            LOG(INFO) << "handlePoint flow : "
                         "完美返回基站点位，不用处理清洁机构，直接返回基站充电 ...";
            setFlow(needFlow);
            callBackStation();
        }
    } else {
        if (backBaseRetryCount < MAX_BASE_POINT_RETRY_COUNT) {
            backBaseRetryCount++;
            //记录返回摆渡点尝试次数
            clean_history_db::CleanHistoryCenter::instance().setBackBaseRetries(
                    backBaseRetryCount);
            LOG(INFO) << "handlePoint flow : 返回基站点位失败，重试中 ...";
            callBackBasePoint();
        } else {
            setFlow(event::flow::software_interrupt_task);
            //记录返回摆渡点失败
            clean_history_db::CleanHistoryCenter::instance().setBackBasePointArrived(
                    clean_history_db::FAIL);
            callTaskInterrupt(point);
        }
    }
}

void AsyncTaskCall::peculiarDisposeMechanism(const RealPoint &point,
                                             event::flow needFlow) {
    LOG(INFO) << "handlePoint flow : 清洁机构已收起，自动回充中 ...";
    clean_history_db::CleanHistoryCenter::instance().setCloseMechanism(clean_history_db::SUCCEED);
    setFlow(needFlow);
    callBackStation();
}

void AsyncTaskCall::peculiarDisposeStation(const RealPoint &point, event::flow arriveFlow, event::flow retryFlow) {
    if (point.realError.arrive) {
        clean_history_db::CleanHistoryCenter::instance().setStationArrived(clean_history_db::SUCCEED);
        LOG(INFO) << "handlePoint flow : 完成自动回充 ...";
        setFlow(arriveFlow);
        goodGame();
    } else {
        if (rechargeRetryCount < MAX_RECHARGE_RETRY_COUNT) {
            LOG(INFO) << "handlePoint flow : 自动回充失败，重试中 ...";
            rechargeRetryCount++;
            clean_history_db::CleanHistoryCenter::instance().setRechargeRetries(rechargeRetryCount);
            backBaseRetryCount = 0;
            setFlow(retryFlow);
            callBackBasePoint();
        } else {
            clean_history_db::CleanHistoryCenter::instance().setStationArrived(clean_history_db::FAIL);
            setFlow(event::flow::software_interrupt_task);
            callTaskInterrupt(point);
        }
    }
}

std::tuple<int, std::string, std::string> AsyncTaskCall::generateErrorByRealPoint(const RealPoint &real_point) {
    std::string error_string;
    int error_code;
    std::string error_code2;
    switch (real_point.getId()) {
//        case SPECIAL_MANUAL_RESUME:
//            error_string = "解除暂停时出错";
//            error_code = 3201;
//            error_code2 = "CCR_201";
//            break;
//        case SPECIAL_MANUAL_PAUSE:
//            error_string = "暂停时出错";
//            error_code = 3202;
//            error_code2 = "CCR_202";
//            break;
//        case SPECIAL_MANUAL_CLEAN_START:
//            error_string = "手动接管";
//            error_code = 3204;
//            error_code2 = "CCR_204";
//            break;
//        case SPECIAL_MANUAL_BACK:
//            error_string = "手动返回基站失败";
//            error_code = 3203;
//            error_code2 = "CCR_203";
//            break;
//        case SPECIAL_FORCE_LOW_BATTERY:
//            error_string = "低电量返回基站失败";
//            error_code = 3206;
//            error_code2 = "CCR_206";
//            break;
//        case SPECIAL_EQUIPMENT_ERROR_BACK:
//            error_string = "强制返回基站失败";
//            error_code = 3208;
//            error_code2 = "CCR_208";
//            break;
//        case SPECIAL_MANUAL_CONTROL_FORCE_BACK:
//            error_string = "模式切换强制返回基站失败";
//            error_code = 3207;
//            error_code2 = "CCR_207";
//            break;
        case FLOW_SEIZE_SEAT:
            error_string = "默认状态下出错";
            error_code = 3210;
            error_code2 = "CCR_210";
            break;
        case FLOW_OPEN_MECHANISM:
            error_string = "开启清洁机构失败";
            error_code = 3211;
            error_code2 = "CCR_211";
            break;
        case FLOW_CLOSE_MECHANISM:
            error_string = "关闭清洁机失败";
            error_code = 3212;
            error_code2 = "CCR_212";
            break;
        case FLOW_OUT_STATION:
            error_string = "出站时失败";
            error_code = 3213;
            error_code2 = "CCR_213";
            break;
        case FLOW_END_SLEEP:
            error_string = "结束睡眠模式失败";
            error_code = 3214;
            error_code2 = "CCR_214";
            break;
        case FLOW_IN_BASE_POINT:
            error_string = "返回基站摆渡点失败";
            error_code = 3215;
            error_code2 = "CCR_215";
            break;
        case FLOW_IN_STATION:
            error_string = "基站对接失败";
            error_code = 3216;
            error_code2 = "CCR_216";
            break;
        case FLOW_INTERRUPT:
            error_string = "程序中断";
            error_code = 3217;
            error_code2 = "CCR_217";
            break;
        default:
            error_string = "未知错误";
            error_code = 3200 - real_point.getId();
            std::string base_string = "CCR_";
            std::string flow_string = to_string(200 - real_point.getId());
            error_code2 = base_string + flow_string;

            break;
    }
    return make_tuple(error_code, error_string, error_code2);
}

void AsyncTaskCall::recordMotorError() {
    internal_event::InternalEventPubManager::get_instance()->pubOper(internal_event::MOTOR_ERROR_RECOVERY_SCCEED);
}

void AsyncTaskCall::recordLaserError(std::string error_event) {
    if (error_event == "laser_scan_4014") {
        internal_event::InternalEventPubManager::get_instance()->pubAlarm(
                SelfCheckErrorType::LASER_RESTART_START);
    } else if (error_event == "laser_scan_4015") {
        internal_event::InternalEventPubManager::get_instance()->pubAlarm(
                SelfCheckErrorType::LASER_RESTART_SUCCEED);
    }
}
