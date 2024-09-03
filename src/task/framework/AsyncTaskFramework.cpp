//
// Created by Looper on 2023/2/6.
//

#include "task/framework/AsyncTaskFramework.h"
#include "simulation.h"
#include "task/manager/StationManager.h"
#include "task/manager/NodeWorkModeManager.h"
#include "task/manager/MechanismManager.h"
#include "task/point_planner.h"
#include "task/point_routine.h"
#include "future/timer_call.h"
#include "future/node/node_control.h"
#include "task/manager/SwitchModePublish.h"

AsyncTaskFramework::AsyncTaskFramework() {
    int err = AsyncTaskCall::make_thread(run, this);
    LOG_IF(INFO, DEBUG_FIRING) << "AsyncTaskFramework : " << err;
}

void AsyncTaskFramework::execute() {
    for (;;) {
        std::unique_lock<std::mutex> lock(cv_mut);
        //当前线程条用 wait 后解锁互斥量并休眠，等待别的线程调用用 notify_one 或者 notify_all 唤醒当前线程。
        //一旦当前线程获得通知(notify)，那么会自动调用 lock 对互斥量加锁。
        //wait 包含第二个等待条件的参数，调用 wait 后解锁互斥量并休眠，直到被 notify 唤醒为止。
        //被唤醒后，wait 会自动加锁，如果得不到则阻塞等待锁，直到获得锁。
        //获得到该互斥量之后，开始判断第二个参数的条件，如果不满足条件表达式返回 false，则 wait 对互斥量解锁并休眠。
        //如果满足条件表达式返回 true，则 wait 函数调用结束，走下面的代码流程。
        cv.wait(lock, [this] {
            return !manualEpollDeque.empty() || !specialEpollDeque.empty() || !errorEpollDeque.empty() ||
                   !urgencyStopDeque.empty() || !taskEpollDeque.empty() || !blockEpollDeque.empty();
        });

        if (manualEpollDeque.size() + specialEpollDeque.size() + errorEpollDeque.size()
            + urgencyStopDeque.size() + taskEpollDeque.size() + blockEpollDeque.size() != 1) {
            LOG(WARNING) << "数据有误，请上传当前日志文件并联系开发者 "
                         << " manualEpollDeque " << manualEpollDeque.size()
                         << ", specialEpollDeque " << specialEpollDeque.size()
                         << ", errorEpollDeque " << errorEpollDeque.size()
                         << ", urgencyStopDeque " << urgencyStopDeque.size()
                         << ", taskEpollDeque " << taskEpollDeque.size()
                         << ", pointEpollDeque " << blockEpollDeque.size();
        }

        loop::execute_handle handle = loop::execute_handle::handle_unknown;
        RealTask realTask;
        RealBlock realBlock;
        if (!manualEpollDeque.empty()) {
            handle = function_manual_epoll();
        } else if (!specialEpollDeque.empty()) {
            handle = function_special_epoll();
        } else if (!errorEpollDeque.empty()) {
            handle = function_error_epoll();
        } else if (!urgencyStopDeque.empty()) {
            handle = function_urgency_stop();
        } else if (!taskEpollDeque.empty()) {
            realTask = taskEpollDeque.back();
            taskEpollDeque.clear();
            handle = loop::execute_handle::handle_task;
        } else if (!blockEpollDeque.empty()) {
            realBlock = blockEpollDeque.back();
            blockEpollDeque.clear();
            handle = loop::execute_handle::handle_point;
        }
        lock.unlock();

        if (handle == loop::execute_handle::handle_manual) {
            handleManualOperation();
        } else if (handle == loop::execute_handle::handle_special) {
            handleSpecialOperation();
        } else if (handle == loop::execute_handle::handle_error) {
            handleErrorOperation();
        } else if (handle == loop::execute_handle::handle_stop) {
            handleStop();
        } else if (handle == loop::execute_handle::handle_task) {
            handleTask(realTask);
        } else if (handle == loop::execute_handle::handle_point) {
            handleBlock(realBlock);
        }

    }
}

//void AsyncTaskFramework::notify_one(const function<void()> &triggerProcess) {
//    {
//        std::unique_lock<std::mutex> lock(cv_mut);
//        triggerProcess();
//    }
//    cv.notify_one();
//}

loop::execute_handle AsyncTaskFramework::function_manual_epoll() {
    switch (manualEpollDeque.back()) {
        case loop::manual_epoll::manual_normal:
            epoll_manual = loop::manual_epoll::manual_normal;
            break;
        case loop::manual_epoll::manual_resume:
            epoll_manual = loop::manual_epoll::manual_resume;
            break;
        case loop::manual_epoll::manual_pause:
            epoll_manual = loop::manual_epoll::manual_pause;
            break;
        case loop::manual_epoll::manual_back:
            epoll_manual = loop::manual_epoll::manual_back;
            break;
        case loop::manual_epoll::manual_force_back:
            epoll_manual = loop::manual_epoll::manual_force_back;
            break;
        case loop::manual_epoll::manual_task_over:
            epoll_manual = loop::manual_epoll::manual_task_over;
            break;
        case loop::manual_epoll::manual_abnormal:
            epoll_manual = loop::manual_epoll::manual_abnormal;
            break;
        default:
            epoll_manual = loop::manual_epoll::manual_unknown;
            break;
    }
    AsyncMachine::instance().setEpoll(epoll_manual, epoll_special, epoll_error, urgency_stop);
    manualEpollDeque.clear();
    return loop::execute_handle::handle_manual;
}


loop::execute_handle AsyncTaskFramework::function_special_epoll() {
    switch (specialEpollDeque.back()) {
        case loop::special_epoll::special_normal:
            epoll_special = loop::special_epoll::special_normal;
            break;
        case loop::special_epoll::special_low_battery:
            epoll_special = loop::special_epoll::special_low_battery;
            break;
        case loop::special_epoll::special_branch_water:
            epoll_special = loop::special_epoll::special_branch_water;
            break;
        case loop::special_epoll::special_sewage_water:
            epoll_special = loop::special_epoll::special_sewage_water;
            break;
        case loop::special_epoll::special_branch_sewage_water:
            epoll_special = loop::special_epoll::special_branch_sewage_water;
            break;
        case loop::special_epoll::special_dust_push_anomaly:
            epoll_special = loop::special_epoll::special_dust_push_anomaly;
            break;
        case loop::special_epoll::special_wet_tow_anomaly:
            epoll_special = loop::special_epoll::special_wet_tow_anomaly;
            break;
        default:
            epoll_special = loop::special_epoll::special_unknown;
            break;
    }
    AsyncMachine::instance().setEpoll(epoll_manual, epoll_special, epoll_error, urgency_stop);
    specialEpollDeque.clear();
    return loop::execute_handle::handle_special;
}


loop::execute_handle AsyncTaskFramework::function_error_epoll() {
    switch (errorEpollDeque.back()) {
        case loop::error_epoll::error_normal:
            epoll_error = loop::error_epoll::error_normal;
            break;
        case loop::error_epoll::error_manual_clean_start:
            epoll_error = loop::error_epoll::error_manual_clean_start;
            break;
        case loop::error_epoll::error_manual_clean_end:
            epoll_error = loop::error_epoll::error_manual_clean_end;
            break;
        case loop::error_epoll::error_lift:
            epoll_error = loop::error_epoll::error_lift;
            break;
        case loop::error_epoll::error_electric_move:
            epoll_error = loop::error_epoll::error_electric_move;
            break;
        case loop::error_epoll::error_unrecoverable:
            epoll_error = loop::error_epoll::error_unrecoverable;
            break;
        default:
            epoll_error = loop::error_epoll::error_unknown;
            break;
    }
    AsyncMachine::instance().setEpoll(epoll_manual, epoll_special, epoll_error, urgency_stop);
    errorEpollDeque.clear();
    return loop::execute_handle::handle_error;
}


loop::execute_handle AsyncTaskFramework::function_urgency_stop() {
    switch (urgencyStopDeque.back()) {
        case loop::urgency_stop::trigger_urgency_stop:
            urgency_stop = loop::urgency_stop::trigger_urgency_stop;
            break;
        case loop::urgency_stop::recovery_urgency_stop:
            urgency_stop = loop::urgency_stop::recovery_urgency_stop;
            break;
        case loop::urgency_stop::release_urgency_stop:
            urgency_stop = loop::urgency_stop::release_urgency_stop;
            break;
    }
    AsyncMachine::instance().setEpoll(epoll_manual, epoll_special, epoll_error, urgency_stop);
    urgencyStopDeque.clear();
    return loop::execute_handle::handle_stop;
}

void AsyncTaskFramework::setEpollManual(loop::manual_epoll epoll_manual) {
    AsyncTaskFramework::epoll_manual = epoll_manual;
    AsyncMachine::instance().setEpoll(epoll_manual, epoll_special, epoll_error, urgency_stop);
}

void AsyncTaskFramework::setEpollSpecial(loop::special_epoll epoll_special) {
    AsyncTaskFramework::epoll_special = epoll_special;
    AsyncMachine::instance().setEpoll(epoll_manual, epoll_special, epoll_error, urgency_stop);
}

void AsyncTaskFramework::setEpollError(loop::error_epoll epoll_error) {
    AsyncTaskFramework::epoll_error = epoll_error;
    AsyncMachine::instance().setEpoll(epoll_manual, epoll_special, epoll_error, urgency_stop);
}

void AsyncTaskFramework::setUrgencyStop(loop::urgency_stop urgency_stop) {
    AsyncTaskFramework::urgency_stop = urgency_stop;
    AsyncMachine::instance().setEpoll(epoll_manual, epoll_special, epoll_error, urgency_stop);
}

//void AsyncTaskFramework::callOutBaseStation() {
//    LOG_IF(INFO, DEBUG_TASK) << "AsyncTaskFramework : 准备齐全，请求出站啦 ...";
//    StationManager::instance().outStation();
//}

//void AsyncTaskFramework::callBackStation() {
//    LOG_IF(INFO, DEBUG_TASK) << "AsyncTaskFramework : 任务结束，准备返回基站充电啦 ...";
//    StationManager::instance().backStation();
//}

void AsyncTaskFramework::callCancelBackStation() {
    LOG_IF(INFO, DEBUG_TASK) << "AsyncTaskFramework : 取消回充动作 ...";
    StationManager::instance().cancelBackStation();
}

void AsyncTaskFramework::callSwitchWorkMode(const std::function<void(bool work)> f) {
    LOG_IF(INFO, DEBUG_TASK) << "AsyncTaskFramework : 新任务来了，查看当前是否处于工作状态 ...";
//    if (!isWorkMode()) {
//        LOG_IF(INFO, DEBUG_TASK) << "AsyncTaskFramework : 不是工作状态，准备启动工作状态 ...";
//
//        NodeWorkModeManager::instance().forceToWork();
//
//        async::ThreadPool pool_;
//        pool_.setNumOfThreads(1);
//        pool_.execute([this, &f]() {
//            while (!isWorkMode() && !sleepTimeout) {
//                sleep(1);
//            }
//            if (!sleepTimeout) {
//                async::TimerCall::instance().baseLoop()->cancelAny();
//            }
//            LOG_IF(INFO, DEBUG_TASK) << "AsyncTaskFramework : 工作模式启动状态 "
//                                     << "  是否是工作 ： " << NodeControl::instance().isWork()
//                                     << "  是否是建图 ： " << NodeControl::instance().isMap()
//                                     << "  是否是睡眠 ： " << NodeControl::instance().isSleep()
//                                     << " ...";
//            notify_one([this, &f]() {
//                f(isWorkMode());
//            });
//        });
//        async::TimerCall::instance().baseLoop()
//                ->scheduleLater(std::chrono::seconds(WAITING_TIME_OF_NODE_WORK_MODE), [this]() {
//                    sleepTimeout = true;
//                });
//    } else {
    LOG_IF(INFO, DEBUG_TASK) << "AsyncTaskFramework : 是工作状态 ...";
    notify_one([&f]() {
        f(true);
    });
//    }
}

void AsyncTaskFramework::callOpenMechanism(const WorkStatus &status, bool knife, std::function<void()> f) {
    LOG_IF(INFO, DEBUG_TASK) << "AsyncTaskFramework : 准备打开相应的清洁机构 " << status << " ...";
//    MechanismManager::instance().controlWorkStatus(status, knife);
    auto fun = std::move(f);
    if (!Environment::instance().isRealEnvironment) {
        async::TimerCall::instance().baseLoop()->scheduleLater(std::chrono::seconds(1), [this, fun]() {
            LOG_IF(INFO, DEBUG_TASK) << "AsyncTaskFramework : 相应的清洁机构已打开 ...";
            LOG_IF(INFO, DEBUG_TASK) << "thread " << pthread_self();
            notify_one(fun);
        });
    } else {
        async::TimerCall::instance().baseLoop()->scheduleLater(
                std::chrono::seconds(OPENING_TIME_OF_CLEANING_MECHANISM), [this, fun]() {
                    LOG_IF(INFO, DEBUG_TASK) << "AsyncTaskFramework : 相应的清洁机构已打开 ...";
                    notify_one(fun);
                });
    }
}

void AsyncTaskFramework::callCloseMechanism(std::function<void()> f) {
    LOG_IF(INFO, DEBUG_TASK) << "AsyncTaskFramework : 准备关闭相应的清洁机构 ...";
    //这个函数里面关闭所有清洁机构
//    MechanismManager::instance().resetWorkStatus();
    auto fun = std::move(f);
    if (!Environment::instance().isRealEnvironment) {
        async::TimerCall::instance().baseLoop()
                ->scheduleLater(std::chrono::seconds(1), [this, fun]() {
                    LOG_IF(INFO, DEBUG_TASK) << "AsyncTaskFramework : 相应的清洁机构已关闭 ...";
                    notify_one(fun);
                });
    } else {
        async::TimerCall::instance().baseLoop()
                ->scheduleLater(std::chrono::seconds(CLOSING_TIME_OF_CLEANING_MECHANISM), [this, fun]() {
                    LOG_IF(INFO, DEBUG_TASK) << "AsyncTaskFramework : 相应的清洁机构已关闭 ...";
                    notify_one(fun);
                });
    }
}

bool AsyncTaskFramework::isWorkMode() {
    return NodeControl::instance().isWork();
}

bool AsyncTaskFramework::isUrgencyStop() {
    return urgency_stop == loop::urgency_stop::trigger_urgency_stop ||
           urgency_stop == loop::urgency_stop::recovery_urgency_stop;
}

bool AsyncTaskFramework::isManualMode() {
    return epoll_error == loop::error_epoll::error_manual_clean_start ||
           epoll_error == loop::error_epoll::error_manual_clean_end;
}

bool AsyncTaskFramework::isUnrecoverableError() {
    return epoll_error == loop::error_epoll::error_unrecoverable ||
           epoll_error == loop::error_epoll::error_lift ||
           epoll_error == loop::error_epoll::error_electric_move ||
           epoll_manual == loop::manual_epoll::manual_unknown ||
           epoll_special == loop::special_epoll::special_unknown ||
           epoll_error == loop::error_epoll::error_unknown;
}

bool AsyncTaskFramework::isManualControl() {
    return epoll_manual == loop::manual_epoll::manual_resume ||
           epoll_manual == loop::manual_epoll::manual_pause ||
           epoll_manual == loop::manual_epoll::manual_back ||
           epoll_manual == loop::manual_epoll::manual_force_back;
}

bool AsyncTaskFramework::isExchangeTask() {
    return epoll_manual == loop::manual_epoll::manual_task_over;
}

bool AsyncTaskFramework::isSpecialDevice() {
    return epoll_special == loop::special_epoll::special_low_battery ||
           epoll_special == loop::special_epoll::special_branch_water ||
           epoll_special == loop::special_epoll::special_sewage_water ||
           epoll_special == loop::special_epoll::special_branch_sewage_water ||
           epoll_special == loop::special_epoll::special_dust_push_anomaly ||
           epoll_special == loop::special_epoll::special_wet_tow_anomaly;
}

bool AsyncTaskFramework::isCharging() {
    return ZooInnerStatus::instance().getIsCharging();
}

void AsyncTaskFramework::callBackBasePoint() {
//    LOG_IF(INFO, DEBUG_TASK) << "AsyncTaskFramework : 准备返回摆渡点了 ...";
    PointPlanner::instance().backBasePoint();
}

void AsyncTaskFramework::callNeedPublishSleep() {
    LOG_IF(INFO, DEBUG_TASK) << "AsyncTaskFramework : 等待充电成功即可发布睡眠模式 ...";
    ZooInnerStatus::instance().setNeedSleep(true);
}

void AsyncTaskFramework::release() {
    sleepTimeout = false;
}

void AsyncTaskFramework::pushManual(loop::manual_epoll loop) {
    manualEpollDeque.push_back(loop);
}

void AsyncTaskFramework::pushSpecial(loop::special_epoll data) {
    specialEpollDeque.push_back(data);
}

void AsyncTaskFramework::pushError(loop::error_epoll data) {
    errorEpollDeque.push_back(data);
}

void AsyncTaskFramework::pushUrgencyStop(loop::urgency_stop data) {
    urgencyStopDeque.push_back(data);
}

void AsyncTaskFramework::pushTask(const RealTask &data) {
    taskEpollDeque.push_back(data);
}

void AsyncTaskFramework::pushBlock(const RealBlock &data) {
    blockEpollDeque.push_back(data);
}

