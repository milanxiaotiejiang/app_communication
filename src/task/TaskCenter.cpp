//
// Created by Looper on 2022/11/5.
//

#include "task/TaskCenter.h"
#include "task/point_planner.h"
#include "task/point_routine.h"
#include "task/task_dispatcher.h"
#include "task/subscribe/zoo_inner_status.h"
#include "task/manager/NodeWorkModeManager.h"
#include "task/manager/StationManager.h"
#include "task/call/head_tail_call.h"
#include "simulation.h"
#include "task/manager/PointProgressPublish.h"
#include "task/manager/SwitchModePublish.h"
#include "task/manager/manual.h"
#include "task/subscribe/async_machine.h"
#include "model/VersionSubscribe.h"
#include "manager/PublishOutManager.h"
#include "future/thread_pool.h"
#include "task/manager/NativeSystemManager.h"
#include "clean_history/CleanHistoryCenter.h"
#include "manager//InternalEventPubManager.h"

/*
 * task转换成realtask，赋值taskid，mode，rate，区域，组合路径区域描述，任务发起人，任务启动时间，timeMode
 */
void TaskCenter::task2RealTask(const Task &task, RealTask &realTask) {
    realTask.setId(task.getTaskId());
    realTask.setMode(task.getMode());
    realTask.setRate(task.getRate());
//    realTask.setWorkStatus(task.getWorkStatus());
    realTask.setZoned(task.getZoned());
    realTask.setCombination(task.getCombination());
    realTask.setLaunchPeople(task.getLaunchPeople());
    realTask.setLaunchTime(task.getLaunchTime());
    realTask.setTimeMode(task.getTimeMode());

    //realTask.setCombinationType(task.)；
}


/*
 * 执行函数，调用来执行整个任务
 */
void TaskCenter::realExecuteTask(const Task &task) {
    SwitchModePublish::instance().cancel();

    LOG(INFO) << "TASK ID : " << task.getTaskId();
    if (AsyncMachine::instance().getError() == loop::error_epoll::error_unrecoverable) {
        throw app::exception(make_error_code(error::operation_failure_please_restart_the_machine));
    }

    //如果是急停按钮推下的状态中，那么直接报错且不执行任务
    auto isUrgencyStopStatus = ZooInnerStatus::instance().getUrgencyStopStatus();
    if (isUrgencyStopStatus) {
        throw app::exception(make_error_code(error::machine_is_in_emergency_stop));
    }

    //当前在手动模式中
    if (AsyncMachine::instance().getError() == loop::error_epoll::error_manual_clean_start
        || AsyncMachine::instance().getError() == loop::error_epoll::error_manual_clean_end) {
        throw app::exception(make_error_code(error::current_in_manual_clean_mode));
    }

    //建图模式下，不能够分发任务
    if (Environment::instance().isRealEnvironment) {
        auto workMode = NodeWorkModeManager::instance().getWorkMode();
        if (workMode == WorkMode::MAPPING) {
            throw app::exception(make_error_code(error::dispatcher_task_work_mode_mapping));
        }
    }

    //如果当前电量少于10%，那么报错且不执行任务
    auto RSOC = ZooInnerStatus::instance().getRsoc();
    if (RSOC < LOW_RSOC) {
        throw app::exception(make_error_code(error::dispatcher_task_low_rsoc));
    }

    //没有传感器数据的情况下，不能够分发任务
    //todo /imu /scan /odom without any data reject
    //todo /knob

    const std::string &launchPeople = task.getLaunchPeople();
    if (!asyncTaskCall->canIssuedTask(launchPeople)) {
        throw app::exception(make_error_code(error::the_current_task_is_not_completed));
    }

    //先验条件全部满足，可以下发任务，先将task转换成realtask，再通过TaskDtcher分发
    RealTask realTask;
    task2RealTask(task, realTask);
    TaskDispatcher::instance().dispatcherTask(realTask);

}

void TaskCenter::initialize(ros::NodeHandle handle) {

    asyncTaskCall = new ReservedCall();

    PointProgressPublish::instance().initialize(handle);

    PointPlanner::instance().initialize(handle);
    PointRoutine::instance().setAsyncTaskCall(asyncTaskCall);

    //任务分发类
    TaskDispatcher::instance().setAsyncTaskCall(asyncTaskCall);

    //状态模式管理类
    NodeWorkModeManager::instance().initialize(handle);

    //基站管理类
    StationManager::instance().initialize(handle);
    StationManager::instance().setAsyncTaskCall(asyncTaskCall);

    //手动管理类
    ManualManager::instance().setAsyncTaskCall(asyncTaskCall);

    NativeSystemManager::instance().setAsyncTaskCall(asyncTaskCall);

    //工作模式管理类
    SwitchModePublish::instance().initialize(handle);

    //下位机状态管理类
    zooRobotStatusSubscribe = new ZooRobotStatusSubscribe(handle);
    //出站管理类
    flagOutSubscribe = new FlagOutSubscribe(handle);
    //进站管理类
    flagInSubscribe = new FlagInSubscribe(handle);

    if (!Environment::instance().isRealEnvironment) {
        std::thread moveBaseThread([]() {
            sleep(10);
            int last_machine_code = 10006;
            while (1) {
                sleep(1);

                NativeSystemManager::instance().urgencyStop(ZooInnerStatus::instance().getUrgencyStopStatus());
                long current_execute_time = clean_history_db::CleanHistoryCenter::instance().getCurrentCleanTime();
                WorkStatus workStatus(0, 0, 0, 0, 0, 0);
                int machineCode = AsyncMachine::instance().getMachineCode();
                if (machineCode != last_machine_code) {
                    internal_event::InternalEventPubManager::get_instance()->workStatusUpdate(machineCode);
                }
                last_machine_code = machineCode;
                std::string machineMessage = AsyncMachine::instance().getMachineMessage(machineCode);
                auto status = ShowWorkStatus(ZooInnerStatus::instance().getRsoc(), 28, 72,
                                             workStatus,
                                             machineMessage, machineCode,
                                             ZooInnerStatus::instance().getUrgencyStopStatus(),
                                             current_execute_time,
                                             ZooInnerStatus::instance().getIsCharging(),
                                             ZooInnerStatus::instance().getAromStatus());
                VersionSubscribe<ShowWorkStatus> statusResponse(1, status);
                PublishOutManager::instance().getPubOut()->publishStatus(statusResponse);
            }
        });
        moveBaseThread.detach();
    }

    if (!Environment::instance().isRealEnvironment) {
        ZooInnerStatus::instance().setRsoc(60);
    }
}

void TaskCenter::uninstall() {
    delete asyncTaskCall;
    asyncTaskCall = nullptr;
    delete zooRobotStatusSubscribe;
    delete flagOutSubscribe;
    delete flagInSubscribe;
}

//executTask主要增加了一条历史记录
void TaskCenter::executeTask(const Task &task) {
    //添加一条历史纪录
    clean_history_db::CleanHistoryCenter::instance().addCleanHistory(task);
    try {
        realExecuteTask(task);
    } catch (app::exception const &e) {
        //如果错误，会走到此处，历史更新错误信息
        clean_history_db::CleanHistoryCenter::instance().launchFailed(task, e);
        const error_code &code = e.code();
        throw e;
    }
}
