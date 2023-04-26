//
// Created by Looper on 2022/11/5.
//

#include "task/TaskCenter.h"
#include "task/point_planner.h"
#include "task/point_routine.h"
#include "task/task_dispatcher.h"
#include "task/call/head_tail_call.h"
#include "simulation.h"

#include "task/manager/PointProgressPublish.h"
#include "task/manager/SwitchModePublish.h"
#include "task/manager/manual.h"
#include "task/manager/NodeWorkModeManager.h"
#include "task/manager/StationManager.h"
#include "task/manager/NativeSystemManager.h"

#include "task/subscribe/zoo_inner_status.h"
#include "task/subscribe/async_machine.h"

#include "net/base/VersionSubscribe.h"

#include "manager/PublishOutManager.h"
#include "manager/InternalEventPubManager.h"

#include "future/thread_pool.h"
#include "clean_history/CleanHistoryCenter.h"
#include "future/node/node_control.h"

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

#include "db/task_data_base.h"
#include "exploration/path_exploration_preview_task.h"

std::string TaskCenter::preTask(const RealTask &task) {
    //拦截手动下发的任务且前期出站后期进站
    if (task.isRenew()) {
        const std::string &source = task.getSource();
        TaskSource taskSource = SqliteDataBase::TaskSourceFromString(source);
        if (taskSource == TaskSource::App || taskSource == TaskSource::Pad) {
            if (!asyncTaskCall->canIssuedTask(task)) {
                throw app::exception(make_error_code(error::the_current_task_is_not_completed));
            }
        }
    } else {
        const std::string &launchPeople = task.getLaunchPeople();
        if (launchPeople == "App" || launchPeople == "Pad") {
            if (!asyncTaskCall->canIssuedTask(task)) {
                throw app::exception(make_error_code(error::the_current_task_is_not_completed));
            }
        }
    }

    //添加一条历史纪录
    clean_history_db::CleanHistoryCenter::instance().addCleanHistory(task);
    try {
        return proTask(task);
    } catch (app::exception const &e) {
        //如果错误，会走到此处，历史更新错误信息
        clean_history_db::CleanHistoryCenter::instance().launchFailed(task, e);
        const error_code &code = e.code();
        throw e;
    }
}

std::string TaskCenter::proTask(const RealTask &task) {
    LOG(INFO) << "TASK ID : " << task.getId();
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
        if (NodeControl::instance().isMap()) {
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

    if (!asyncTaskCall->canIssuedTask(task)) {
        throw app::exception(make_error_code(error::the_current_task_is_not_completed));
    }

    if (task.isRenew()) {
        //如果任务是湿拖任务，清水箱已空或者污水箱已满，不能分发任务
        if (task.getWorkStatus().getMopStatus() == 1) {
            if (ZooInnerStatus::instance().getCleanWaterLevel() == 0) {
                throw app::exception(make_error_code(error::clean_water_level_check_failed));
            }
            if (ZooInnerStatus::instance().getDirtyWaterLevel() == 100) {
                throw app::exception(make_error_code(error::dirty_water_level_check_failed));
            }
        }
        if (task.getWorkStatus().getVacuumStatus() == 1) {
            if (ZooInnerStatus::instance().getDirtyWaterLevel() == 100) {
                throw app::exception(make_error_code(error::dirty_water_level_check_failed));
            }
        }
    }

    return realTask(task);
}

std::string TaskCenter::realTask(RealTask task) {
    TaskDispatcher::instance().dispatcherTask(task);
    return task.getId();
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

    //地图管理类
    CartographerPublisher::instance().initialize(handle);
    CartographerSubscribe::instance().initialize(handle);
    CartographerSubscribe::instance().setAsyncTaskCall(asyncTaskCall);

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
    //地毯检测
    carpetDetectSubscribe = new CarpetDetectSubscribe(handle);
    carpetDetectSubscribe->setAsyncTaskCall(asyncTaskCall);
    //电梯
    liftDetectSubscribe = new LiftDetectSubscribe(handle);
    liftDetectSubscribe->setAsyncTaskCall(asyncTaskCall);

    if (!Environment::instance().isRealEnvironment) {
        std::thread moveBaseThread([]() {
            sleep(10);
            NodeControl::instance().emulate();
            int last_machine_code = 10006;
            while (1) {
                sleep(1);
//                LOG(ERROR) << "isSleep : " << NodeControl::instance().isSleep()
//                           << " isWork : " << NodeControl::instance().isWork()
//                           << " isMap : " << NodeControl::instance().isMap();
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
                PublishOutManager::instance().publishStatus(statusResponse);
            }
        });
        moveBaseThread.detach();
    }

    if (!Environment::instance().isRealEnvironment) {
        ZooInnerStatus::instance().setRsoc(60);
        ZooInnerStatus::instance().setCleanWaterLevel(50);
    }
}

void TaskCenter::uninstall() {
    delete asyncTaskCall;
    asyncTaskCall = nullptr;
    delete zooRobotStatusSubscribe;
    delete flagOutSubscribe;
    delete flagInSubscribe;
    delete carpetDetectSubscribe;
}

void TaskCenter::executeTask(const Task &task) {
    RealTask realTask;
    TaskExploration::task2RealTask(task, realTask);
    preTask(realTask);
}

std::string TaskCenter::performTask(const long taskId, TaskSource on_source, int on_rate) {
    auto task = TaskDataBase::instance().loadTaskFoId(taskId);
    RealTask realTask;
    realTask.setRate(task.getRate() * on_rate);
    realTask.setOnSource(SqliteDataBase::SourceToString(on_source));
    TaskExploration::task2RealTask(task, realTask);
    return preTask(realTask);
}
