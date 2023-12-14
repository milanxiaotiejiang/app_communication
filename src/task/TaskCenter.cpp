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
#include "task/manager/MechanismManager.h"
#include "leave/MaintenanceMode.h"
#include "leave/auto_maintenance_mode.h"

#include "leave/ParamManager.h"
#include "db/task_data_base.h"
#include "db/segmentation_data_base.h"
#include "leave/sensor/sensor_center.h"
#include "segmentation/GateComprehensive.h"

std::string TaskCenter::preTask(const RealTask &task) {
    //拦截手动下发的任务且前期出站后期进站
    const std::string &source = task.getOnSource();
    TaskSource taskSource = SqliteDataBase::TaskSourceFromString(source);
    if (taskSource == TaskSource::App || taskSource == TaskSource::Pad || taskSource == TaskSource::Cloud) {
        if (!asyncTaskCall->canIssuedTask(task)) {
            throw app::exception(make_error_code(error::the_current_task_is_not_completed));
        }
    }

    //添加一条历史纪录
    clean_history_db::CleanHistoryCenter::instance().addCleanHistory(task);
    try {
        return proTask(task);
    } catch (app::exception const &e) {
        //如果错误，会走到此处，历史更新错误信息
        clean_history_db::CleanHistoryCenter::instance().launchFailed(task, e);
        const std::error_code &code = e.code();
        throw e;
    }
}

std::string TaskCenter::proTask(const RealTask &task) {
    LOG_IF(INFO, DEBUG_TASK) << "TASK ID : " << task.getId();
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

    if (MaintenanceModeSingleton::instance().isMaintenanceMode()) {
        throw app::exception(make_error_code(error::dispatcher_maintenance_mode));
    }

    //如果当前电量少于10%，那么报错且不执行任务
    auto RSOC = ZooInnerStatus::instance().getRsoc();
    if (RSOC < LOW_RSOC) {
        throw app::exception(make_error_code(error::dispatcher_task_low_rsoc));
    }

    if (AutoMaintenanceModeManager::instance().isMaintenanceMode()) {
        throw app::exception(
                make_error_code(error::during_the_automatic_maintenance_period_the_task_cannot_be_started));
    }
    if (SensorCenter::instance().isSensorSelfMode()) {
        throw app::exception(
                make_error_code(error::during_self_check_the_task_cannot_be_started));
    }
    if (GateSettingCenter::instance().isGateSettingMode()) {
        throw app::exception(
                make_error_code(error::in_the_setting_of_gate_the_task_cannot_be_started));
    }

    if (NodeControl::instance().cameraFiringAvailable != 3) {
        throw app::exception(
                make_error_code(error::camera_starting_or_failed_to_start));
    }

    if (!asyncTaskCall->canIssuedTask(task)) {
        throw app::exception(make_error_code(error::the_current_task_is_not_completed));
    }

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

    return realTask(task);
}

std::string TaskCenter::realTask(RealTask task) {
    TaskDispatcher::instance().dispatcherTask(task);
    return task.getId();
}

void TaskCenter::initialize(const ros::NodeHandle &handle) {

    asyncTaskCall = std::make_shared<ReservedCall>();

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
    CartographerServiceClient::instance().initialize(handle);
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
            sleep(5);
            NodeControl::instance().emulate();
            int last_machine_code = 10006;
            long ii = 0;
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
                PublishOutManager::instance().publishStatus(statusResponse);

                if (ZooInnerStatus::instance().getNeedSleep() && ZooInnerStatus::instance().getIsCharging()) {
                    SwitchModePublish::instance().publish();
                    ZooInnerStatus::instance().setNeedSleep(false);
                }

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
    delete zooRobotStatusSubscribe;
    delete flagOutSubscribe;
    delete flagInSubscribe;
    delete carpetDetectSubscribe;
}

std::string TaskCenter::performTask(const long taskId, TaskSource on_source, int on_rate) {
    long perform_task_id = taskId;
    //雨雪天模式
    if (ParamManager::instance().getRainSnow()) {
        MapPo map = SegmentationDataBase::instance().getDbMap();
        const TaskVo &rainSnowTask = TaskDataBase::instance().loadRainSnowTask(map.id);
        if (rainSnowTask.getId() == -1) {
            throw app::exception(make_error_code(error::the_rain_snow_task_is_not_set));
        }
        perform_task_id = rainSnowTask.getId();
    }

    auto task = TaskDataBase::instance().loadTaskFoId(perform_task_id);
    RealTask realTask;
    realTask.setRate(task.getRate() * on_rate);
    realTask.setOnSource(SqliteDataBase::SourceToString(on_source));
    TaskExploration::task2RealTask(task, realTask);


    const std::string &oMapId = task.getOMapId();
    if (oMapId != SegmentationDataBase::instance().getDbMap().id) {
//        throw app::exception(make_error_code(error::cross_floor_tasks_are_currently_not_supported));

        if (!SegmentationDataBase::instance().getDbMap().elevator)
            throw app::exception(make_error_code(error::the_current_map_does_not_have_ladder_control_points_set));

        auto taskMap = SegmentationDataBase::instance().selectMapById(oMapId);
        if (!taskMap.elevator)
            throw app::exception(make_error_code(error::no_ladder_control_points_have_been_set_on_the_task_map));


        //多地图任务，进行任务类型判断
        TaskMode mode = SqliteDataBase::TaskModeFromInt(realTask.getMode());
        if (mode == TaskMode::Subregion || mode == TaskMode::Line)
            throw app::exception(make_error_code(error::cross_floor_tasks_currently_only_support_full_coverage_tasks));

        if (Environment::instance().isRealEnvironment)
            if (mode == TaskMode::Zoned)
                throw app::exception(
                        make_error_code(error::cross_floor_tasks_currently_only_support_full_coverage_tasks));

        //多地图任务，进行楼宇判断
        auto buildMaps = SegmentationDataBase::instance().findBuildMapsForMap(oMapId);
        if (buildMaps.empty()) {
            throw app::exception(make_error_code(error::no_multi_map_buildings_have_been_set_up));
        } else if (buildMaps.size() == 1) {

            std::pair<BuildPo, MapPo> buildMap = buildMaps[0];
            BuildPo &buildPo = buildMap.first;

            realTask.setAsyncMap(true);
            realTask.setBuildId(buildPo.id);

            // 设置任务地图的梯控信息
            RealPoint taskPoint;
            TaskExploration::mapElevator2RealPoint(taskMap, taskPoint);
            realTask.setDoMapId(taskMap.id);
            realTask.setDoFloor(taskMap.floor);
            realTask.setDoPoint(taskPoint);

            // 设置当前地图的梯控信息
            auto preMap = SegmentationDataBase::instance().getDbMap();
            RealPoint preRealPoint;
            TaskExploration::mapElevator2RealPoint(preMap, preRealPoint);
            realTask.setPreMapId(preMap.id);
            realTask.setPreFloor(preMap.floor);
            realTask.setPrePoint(preRealPoint);

            // 设置基站地图的梯控信息
            // 此处正在清洁的地图有基站则用之，正在清洁的没有找上一张地图，上一张没有，找最低层的基站（1、-1、2、-2）
            if (taskMap.base_station) {
                realTask.setPostMapId(taskMap.id);
                realTask.setPostFloor(taskMap.floor);
                realTask.setPostPoint(taskPoint);
            } else if (preMap.base_station) {
                realTask.setPostMapId(preMap.id);
                realTask.setPostFloor(preMap.floor);
                realTask.setPostPoint(preRealPoint);
            } else {

                auto buildAllMaps = SegmentationDataBase::instance().findBuildMapsForBuild(buildPo.id);
                if (buildAllMaps.empty()) {
                    throw app::exception(make_error_code(error::multiple_map_building_data_error));
                } else {
                    int postMapIndex = -1;
                    int optimumFloor = 0;
                    for (int i = 0; i < buildAllMaps.size(); i++) {
                        auto curBuildMap = buildAllMaps[i];
                        auto curMap = curBuildMap.second;
                        if (curMap.elevator) {
                            if (optimumFloor == 0) {
                                optimumFloor = curMap.floor;
                                postMapIndex = i;
                            } else {
                                if (abs(curMap.floor) < optimumFloor) {
                                    optimumFloor = abs(curMap.floor);
                                } else if (abs(curMap.floor) == optimumFloor) {
                                    optimumFloor = curMap.floor > 0 ? optimumFloor : curMap.floor;
                                    postMapIndex = curMap.floor > 0 ? postMapIndex : i;
                                }
                            }
                        }
                    }
                    if (postMapIndex == -1)
                        throw app::exception(make_error_code(error::multiple_map_building_data_error));

                    auto postBuildMap = buildAllMaps[postMapIndex];
                    auto postMap = postBuildMap.second;
                    RealPoint postPoint;
                    TaskExploration::mapElevator2RealPoint(postMap, postPoint);
                    realTask.setPostMapId(postMap.id);
                    realTask.setPostFloor(postMap.floor);
                    realTask.setPostPoint(postPoint);
                }

            }

        } else {
            throw app::exception(make_error_code(error::multiple_map_building_data_error));
        }
    }

    return preTask(realTask);
}
