//
// Created by Looper on 2022/11/26.
//

#include "task/call/head_tail_call.h"

void HeadTailPointCall::handleFlowPoint(const RealPoint &point) {
    if (point.getId() == FLOW_SEIZE_SEAT) {
        setFlow(event::flow::out_base_station);
    } else if (point.getId() == FLOW_OUT_STATION) {
        if (point.realError.arrive) {
            setFlow(event::flow::switch_node_work_mode);
            clean_history_db::CleanHistoryCenter::instance().setOutStation(clean_history_db::SUCCEED);
        } else {
            setFlow(event::flow::software_interrupt_task);
            clean_history_db::CleanHistoryCenter::instance().setOutStation(clean_history_db::FAIL);
        }
    } else if (point.getId() == FLOW_END_SLEEP) {
        if (point.realError.arrive) {
            setFlow(event::flow::preliminary_preparation_completed);
            clean_history_db::CleanHistoryCenter::instance().setEndSleep(clean_history_db::SUCCEED);
        } else {
            setFlow(event::flow::software_interrupt_task);
            clean_history_db::CleanHistoryCenter::instance().setEndSleep(clean_history_db::FAIL);
        }
    } else if (point.getId() == FLOW_IN_BASE_POINT) {
        if (point.realError.arrive) {
            //记录返回摆渡点成功
            clean_history_db::CleanHistoryCenter::instance().setBackBasePointArrived(clean_history_db::SUCCEED);
            if (rechargeRetryCount == 0) {
                setFlow(event::flow::arrive_base_point_success);
            } else {
                setFlow(event::flow::flowing_water_execution_completed);
            }
        } else {
            if (backBaseRetryCount < MAX_BASE_POINT_RETRY_COUNT) {
                backBaseRetryCount++;
                //返回摆渡点重试次数
                clean_history_db::CleanHistoryCenter::instance().setBackBaseRetries(backBaseRetryCount);
                LOG(INFO) << "handlePoint flow : 返回基站点位失败，重试中 ...";
                setFlow(event::flow::try_move_base_point_again);
            } else {
                clean_history_db::CleanHistoryCenter::instance().setBackBasePointArrived(clean_history_db::FAIL);
                setFlow(event::flow::software_interrupt_task);
            }
        }
    } else if (point.getId() == FLOW_IN_STATION) {
        if (point.realError.arrive) {
            clean_history_db::CleanHistoryCenter::instance().setStationArrived(clean_history_db::SUCCEED);
            setFlow(event::flow::arrive_base_station_success);
        } else {
            if (rechargeRetryCount < MAX_RECHARGE_RETRY_COUNT) {
                rechargeRetryCount++;
                clean_history_db::CleanHistoryCenter::instance().setRechargeRetries(rechargeRetryCount);
                setFlow(event::flow::try_recharging_again);
            } else {
                clean_history_db::CleanHistoryCenter::instance().setStationArrived(clean_history_db::FAIL);
                setFlow(event::flow::software_interrupt_task);
            }
        }
    } else if (point.getId() == FLOW_CLOSE_MECHANISM) {
        if (point.realError.arrive) {
            clean_history_db::CleanHistoryCenter::instance().setCloseMechanism(clean_history_db::SUCCEED);
            setFlow(event::flow::flowing_water_execution_completed);
        } else {
            clean_history_db::CleanHistoryCenter::instance().setCloseMechanism(clean_history_db::FAIL);
            setFlow(event::flow::hardware_interrupt_task);
        }
    } else if (point.getId() == FLOW_OPEN_MECHANISM) {
        if (point.realError.arrive) {
            clean_history_db::CleanHistoryCenter::instance().setOpenMechanism(clean_history_db::SUCCEED);
            setFlow(event::flow::cleaning_mechanism_ready);
        } else {
            clean_history_db::CleanHistoryCenter::instance().setOpenMechanism(clean_history_db::FAIL);
            setFlow(event::flow::hardware_interrupt_task);
        }
    }
}

void HeadTailPointCall::processControl(const RealPoint &point) {
    switch (currentFlow()) {
        case event::flow::waiting_for_task: {
            break;
        }
        case event::flow::out_base_station: {
            callOutBaseStation();
            break;
        }
        case event::flow::switch_node_work_mode: {
            callSwitchWorkMode();
            break;
        }
        case event::flow::preliminary_preparation_completed: {
            LOG(INFO) << "handlePoint flow : 前期的出站、睡眠等流程执行成功，现在启动清洁机构 ...";
            callOpenMechanism(point);
            break;
        }
        case event::flow::cleaning_mechanism_ready: {
            LOG(INFO) << "handlePoint flow : 清洁机构下放成功，准备执行规划点位任务，当前去第一个点 ...";
            setFlow(event::flow::ensure_move_to_start_point);
            callGoFirstPoint();
            break;
        }
        case event::flow::ensure_move_to_start_point: {
            if (point.realError.arrive) {
                LOG(INFO) << "handlePoint flow : 到达第一个点位，开始流水线作业 ...";
                setFlow(event::flow::flowing_water_production);
                handleAutoPoint(point);
            } else {
                callRetryFirstPoint([this, point]() {
                    LOG(INFO) << "handlePoint flow : 第一个点位不能到达，收起清洁机构 ...";
                    callCloseMechanism();
                });
            }
            break;
        }
        case event::flow::flowing_water_production: {
            if (plannerQueue.size() == 1) {
                //最后一个已经走完，移除最后一个再次执行一次，走收拖头
                LOG(INFO) << "handlePoint flow : 清扫结束，准备回基站点 ...";
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
            LOG(INFO) << "handlePoint flow : 成功到达基站前点位，收起清洁机构 ...";
            callCloseMechanism();
            break;
        }
        case event::flow::flowing_water_execution_completed: {
            LOG(INFO) << "handlePoint flow : 任务执行完成且返回了基站点，准备回充 ...";
            callBackStation();
            break;
        }
        case event::flow::arrive_base_station_success: {
            LOG(INFO) << "handlePoint flow : 回到基站，且已充电 ...";
            goodGame();
            break;
        }
        case event::flow::try_recharging_again: {
            LOG(INFO) << "handlePoint flow : 回充失败，再次返回基站点位置 ...";
            backBaseRetryCount = 0;
            callBackBasePoint();
            break;
        }
        case event::flow::try_move_base_point_again: {
            callBackBasePoint();
            break;
        }
        case event::flow::hardware_interrupt_task: {
            LOG(INFO) << "handlePoint flow : 清洁机构出错，执行返回基站命令 错误代码" << point.getId();
            callBackBasePoint();
            break;
        }
        case event::flow::software_interrupt_task: {
            callTaskInterrupt(point);
            break;
        }
    }
}
