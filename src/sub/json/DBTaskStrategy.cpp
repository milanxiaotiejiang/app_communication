//
// Created by Looper on 2023/3/20.
//

#include "sub/json/DBTaskStrategy.h"
#include <regex>
#include "db/segmentation_data_base.h"
#include "db/task_data_base.h"
#include "tool/regex_valid.h"
#include "tool/param_check.h"
#include "schedule/schedule_manager_singleton.h"
#include "leave/ParamManager.h"

long AddTaskStrategy::handler(TaskVo params) {
    checkWorkStatus(params.getWorkStatus());
    checkName(params.getName());
    checkRate(params.getRate());
    checkMode(params.getMode());
    checkSource(params.getSource());

    if (params.getMode() == static_cast<int>(TaskMode::Zoned)) {
        checkZoned(params.getZones());
    } else if (params.getMode() == static_cast<int>(TaskMode::Subregion)) {
        checkSubregion(params.getSubregions());
    }

    MapPo map = SegmentationDataBase::instance().getDbMap();
    return TaskDataBase::instance().addTask(map.id, params);
}

long MultipleAddTaskStrategy::handler(TaskVo params) {
    checkWorkStatus(params.getWorkStatus());
    checkName(params.getName());
    checkRate(params.getRate());
    checkMode(params.getMode());
    checkSource(params.getSource());

    if (!SegmentationDataBase::instance().existMap(params.getOMapId())) {
        throw std::invalid_argument("mapId " + params.getOMapId() + " is not exist");
    }

    if (params.getMode() == static_cast<int>(TaskMode::Zoned)) {
        checkZoned(params.getZones());
    } else if (params.getMode() == static_cast<int>(TaskMode::Subregion)) {
        checkSubregion(params.getSubregions());
    }

    MapPo map = SegmentationDataBase::instance().getDbMap();
    return TaskDataBase::instance().addTask(params.getOMapId(), params);
}

std::string DeleteTaskStrategy::handler(long params) {
    if (ParamManager::instance().getRainSnow()) {
        MapPo map = SegmentationDataBase::instance().getDbMap();
        const TaskVo &taskVo = TaskDataBase::instance().loadTaskFoId(params);
        //开启“雨雪天模式”后，已勾选的雨雪天任务不能取消勾选或删除任务。
        if (taskVo.isRainSnow()) {
            throw app::exception(make_error_code(
                    error::the_rain_snow_mode_has_been_activated_and_this_task_not_be_deleted_or_cancelled));
        }
    }
    TaskDataBase::instance().deleteTaskFoId(params);
    ScheduleManagerSingleton::instance().trigger_task_update();
    return "";
}

std::string DeleteMultipleTaskStrategy::handler(std::vector<long> params) {
    MapPo map = SegmentationDataBase::instance().getDbMap();
    if (ParamManager::instance().getRainSnow()) {
        for (const auto &item: params) {
            const TaskVo &taskVo = TaskDataBase::instance().loadTaskFoId(item);
            //开启“雨雪天模式”后，已勾选的雨雪天任务不能取消勾选或删除任务。
            if (taskVo.isRainSnow()) {
                throw app::exception(make_error_code(
                        error::the_rain_snow_mode_has_been_activated_and_this_task_not_be_deleted_or_cancelled));
            }
        }
    }
    for (const auto &item: params) {
        TaskDataBase::instance().deleteTaskFoId(item);
    }
    return "";
}

std::string MultipleDeleteTaskStrategy::handler(long params) {
    const TaskVo &taskVo = TaskDataBase::instance().loadTaskFoId(params);
    std::string oMapId = taskVo.getOMapId();
    MapPo map = SegmentationDataBase::instance().getDbMap();

    if (oMapId == map.id) {
        if (ParamManager::instance().getRainSnow()) {
            //开启“雨雪天模式”后，已勾选的雨雪天任务不能取消勾选或删除任务。
            if (taskVo.isRainSnow()) {
                throw app::exception(make_error_code(
                        error::the_rain_snow_mode_has_been_activated_and_this_task_not_be_deleted_or_cancelled));
            }
        }
        TaskDataBase::instance().deleteTaskFoId(params);
        ScheduleManagerSingleton::instance().trigger_task_update();
    } else {
        TaskDataBase::instance().deleteTaskFoId(params);
    }
    return "";
}

std::vector<TaskVo> ListTaskStrategy::handler(std::string params) {
    MapPo map = SegmentationDataBase::instance().getDbMap();
    return TaskDataBase::instance().loadTaskFoMap(map.id);
}

std::vector<TaskVo> MultipleListTaskStrategy::handler(std::string params) {
    if (!SegmentationDataBase::instance().existMap(params)) {
        throw std::invalid_argument("mapId " + params + " is not exist");
    }

    MapPo map = SegmentationDataBase::instance().getDbMap();
    return TaskDataBase::instance().loadTaskFoMap(params);
}

std::vector<TaskVo> MultipleWholeListTaskStrategy::handler(std::string params) {
    return TaskDataBase::instance().loadTask();
}

TaskVo QueryIdTaskStrategy::handler(long params) {
    return TaskDataBase::instance().loadTaskFoId(params);
}

void ClearCurrentListTaskStrategy::handler() {
    TaskDataBase::instance().deleteTaskFoMap(SegmentationDataBase::instance().getDbMap().id);
    ScheduleManagerSingleton::instance().trigger_task_update();
}

long AddTimerTaskStrategy::handler(TimerVo params) {
    checkName(params.getTaskName());
    checkName(params.getTimerName());
    checkRate(params.getRate());

    if (!TaskDataBase::instance().existTask(params.getTaskId())) {
        throw std::invalid_argument("taskId " + std::to_string(params.getTaskId()) + " is not exist");
    }

    MapPo map = SegmentationDataBase::instance().getDbMap();

    checkSameTimer(map.id, params.getTimerRule(), -1);

    long timer = TaskDataBase::instance().addTimer(map.id, params);

    ScheduleManagerSingleton::instance().trigger_task_update();
    return timer;
}

long MultipleAddTimerTaskStrategy::handler(TimerVo params) {
    checkName(params.getTaskName());
    checkName(params.getTimerName());
    checkRate(params.getRate());

    if (!TaskDataBase::instance().existTask(params.getTaskId())) {
        throw std::invalid_argument("taskId " + std::to_string(params.getTaskId()) + " is not exist");
    }

    auto task = TaskDataBase::instance().loadTaskFoId(params.getTaskId());

    checkSameTimer(task.getOMapId(), params.getTimerRule(), -1);

    long timer = TaskDataBase::instance().addTimer(task.getOMapId(), params);

    if (task.getOMapId() == SegmentationDataBase::instance().getDbMap().id) {
        ScheduleManagerSingleton::instance().trigger_task_update();
    }
    return timer;
}

std::string DeleteTimerTaskStrategy::handler(long params) {
    TaskDataBase::instance().deleteTimerForId(params);
    ScheduleManagerSingleton::instance().trigger_task_update();
    return "";
}

std::string DeleteMultipleTimerTaskStrategy::handler(std::vector<long> params) {
    for (const auto &item: params) {
        TaskDataBase::instance().deleteTimerForId(item);
    }
    ScheduleManagerSingleton::instance().trigger_task_update();
    return "";
}

std::string MultipleDeleteTimerTaskStrategy::handler(long params) {
    auto timerPo = TaskDataBase::instance().loadTimerPoFoId(params);
    std::string oMapId = timerPo.o_map_id;
    MapPo map = SegmentationDataBase::instance().getDbMap();
    TaskDataBase::instance().deleteTimerForId(params);
    if (oMapId == map.id) {
        ScheduleManagerSingleton::instance().trigger_task_update();
    }
    return "";
}

std::vector<TimerVo> ListTimerTaskStrategy::handler(std::string params) {
    MapPo map = SegmentationDataBase::instance().getDbMap();
    return TaskDataBase::instance().loadTimerFoMap(map.id);
}

std::vector<TimerVo> MultipleListTimerTaskStrategy::handler(std::string params) {
    if (!SegmentationDataBase::instance().existMap(params)) {
        throw std::invalid_argument("mapId " + params + " is not exist");
    }

    return TaskDataBase::instance().loadTimerFoMap(params);
}

std::vector<TimerVo> MultipleWholeListTimerTaskStrategy::handler(std::string params) {
    return TaskDataBase::instance().loadTimerList();
}

std::string ModifyTimerTaskStrategy::handler(TimerVo params) {
    checkName(params.getTimerName());
    checkRate(params.getRate());

    if (!TaskDataBase::instance().existTask(params.getTaskId())) {
        throw std::invalid_argument("taskId " + std::to_string(params.getTaskId()) + " is not exist");
    }

    MapPo map = SegmentationDataBase::instance().getDbMap();
    checkSameTimer(map.id, params.getTimerRule(), params.getTimerId());

    TaskDataBase::instance().modifyTimer(map.id, params);
    ScheduleManagerSingleton::instance().trigger_task_update();
    return "";
}

std::string MultipleModifyTimerTaskStrategy::handler(TimerVo params) {
    checkName(params.getTimerName());
    checkRate(params.getRate());

    if (!TaskDataBase::instance().existTask(params.getTaskId())) {
        throw std::invalid_argument("taskId " + std::to_string(params.getTaskId()) + " is not exist");
    }

    auto task = TaskDataBase::instance().loadTaskFoId(params.getTaskId());

    checkSameTimer(task.getOMapId(), params.getTimerRule(), params.getTimerId());

    TaskDataBase::instance().modifyTimer(task.getOMapId(), params);

    if (task.getOMapId() == SegmentationDataBase::instance().getDbMap().id) {
        ScheduleManagerSingleton::instance().trigger_task_update();
    }
    return "";
}

TaskVo BuildPrincipalTaskStrategy::handler(long params) {
    return TaskDataBase::instance().modifyPrincipalTask(params, true);
}

TaskVo CancelPrincipalTaskStrategy::handler(long params) {
    return TaskDataBase::instance().modifyPrincipalTask(params, false);
}

TaskVo PrincipalTaskStrategy::handler(std::string params) {
    MapPo map = SegmentationDataBase::instance().getDbMap();
    const TaskVo &vo = TaskDataBase::instance().loadPrincipalTask(map.id);
    if (vo.getId() == -1) {
        throw app::exception(make_error_code(error::the_main_task_is_not_set));
    }
    return vo;
}

TaskVo MultiplePrincipalTaskStrategy::handler(std::string params) {
    if (!SegmentationDataBase::instance().existMap(params)) {
        throw std::invalid_argument("mapId " + params + " is not exist");
    }
    const TaskVo &vo = TaskDataBase::instance().loadPrincipalTask(params);
    if (vo.getId() == -1) {
        throw app::exception(make_error_code(error::the_main_task_is_not_set));
    }
    return vo;
}

TaskVo BuildRainSnowTaskStrategy::handler(long params) {
    const TaskVo &taskVo = TaskDataBase::instance().loadTaskFoId(params);
    if (SqliteDataBase::TaskModeFromInt(taskVo.getMode()) != TaskMode::Zoned) {
        throw app::exception(make_error_code(error::non_zoning_tasks_cannot_be_set_as_rainy_and_snowy_tasks));
    }
    return TaskDataBase::instance().modifyRainSnowTask(params, true);
}

TaskVo CancelRainSnowTaskStrategy::handler(long params) {
    const TaskVo &taskVo = TaskDataBase::instance().loadTaskFoId(params);
    if (ParamManager::instance().getRainSnow()) {
        //开启“雨雪天模式”后，已勾选的雨雪天任务不能取消勾选或删除任务。
        if (taskVo.isRainSnow()) {
            throw app::exception(make_error_code(
                    error::the_rain_snow_mode_has_been_activated_and_this_task_not_be_deleted_or_cancelled));
        }
    }
    if (SqliteDataBase::TaskModeFromInt(taskVo.getMode()) != TaskMode::Zoned) {
        throw app::exception(make_error_code(error::non_zoning_tasks_cannot_be_set_as_rainy_and_snowy_tasks));
    }
    return TaskDataBase::instance().modifyRainSnowTask(params, false);
}

TaskVo RainSnowTaskStrategy::handler(std::string params) {
    MapPo map = SegmentationDataBase::instance().getDbMap();
    const TaskVo &vo = TaskDataBase::instance().loadRainSnowTask(map.id);
    if (vo.getId() == -1) {
        throw app::exception(make_error_code(error::the_rain_snow_task_is_not_set));
    }
    return vo;
}

TaskVo MultipleRainSnowTaskStrategy::handler(std::string params) {
    if (!SegmentationDataBase::instance().existMap(params)) {
        throw std::invalid_argument("mapId " + params + " is not exist");
    }
    const TaskVo &vo = TaskDataBase::instance().loadRainSnowTask(params);
    if (vo.getId() == -1) {
        throw app::exception(make_error_code(error::the_rain_snow_task_is_not_set));
    }
    return vo;
}

std::string ModifyTaskNameStrategy::handler(ModifyTaskName params) {
    checkName(params.name);
    TaskDataBase::instance().modifyName(params.id, params.name);
    return "";
}

std::string ModifyTaskRateStrategy::handler(ModifyTaskRate params) {
    checkRate(params.rate);
    TaskDataBase::instance().modifyRate(params.id, params.rate);
    return "";
}

std::string ModifyTaskWorkStatusStrategy::handler(ModifyTaskWorkStatus params) {
    checkWorkStatus(params.workStatus);
    TaskDataBase::instance().modifyWorkStatus(params.id, params.workStatus);
    return "";
}

std::string ModifyTaskKnifeStrategy::handler(ModifyTaskKnife params) {
    TaskDataBase::instance().modifyKnife(params.id, params.knife);
    return "";
}

TaskVo ModifyCompleteTaskStrategy::handler(TaskVo params) {
    checkWorkStatus(params.getWorkStatus());
    checkName(params.getName());
    checkRate(params.getRate());
    checkMode(params.getMode());
    checkSource(params.getSource());

    if (params.getMode() == static_cast<int>(TaskMode::Zoned)) {
        checkZoned(params.getZones());
    } else if (params.getMode() == static_cast<int>(TaskMode::Subregion)) {
        checkSubregion(params.getSubregions());
    }

    MapPo map = SegmentationDataBase::instance().getDbMap();
    return TaskDataBase::instance().modifyTask(params);
}


long OperateAddZoneStrategy::handler(ModifyTaskZone params) {
    checkZoned(params.zone);
    long zoneId = TaskDataBase::instance().operateAddZone(params.id, params.zone);
    if (zoneId == -1) {
        throw app::exception(make_error_code(error::add_zone_fail));
    }
    return zoneId;
}

std::string OperateDeleteZoneStrategy::handler(ModifyTaskZone params) {
    TaskDataBase::instance().operateDeleteZone(params.id, params.zone);
    return "";
}

std::string OperateModifyZoneStrategy::handler(ModifyTaskZone params) {
    checkZoned(params.zone);
    TaskDataBase::instance().operateModifyZone(params.id, params.zone);
    return "";
}

std::string ModifyTaskPartitionStrategy::handler(ModifyTaskPartition params) {
    TaskDataBase::instance().modifyPartition(params.id, params.partition);
    return "";
}

long OperateAddSubregionStrategy::handler(ModifyTaskSubregion params) {
    checkSubregion(params.subregion);
    long subregionId = TaskDataBase::instance().operateAddSubregion(params.id, params.subregion);
    if (subregionId == -1) {
        throw app::exception(make_error_code(error::add_subregion_fail));
    }
    return subregionId;
}

std::string OperateDeleteSubregionStrategy::handler(ModifyTaskSubregion params) {
    TaskDataBase::instance().operateDeleteSubregion(params.id, params.subregion);
}

std::string ModifyTimerNameStrategy::handler(ModifyTimerName params) {
    checkName(params.timer_name);
    TaskDataBase::instance().modifyTimerName(params.id, params.timer_name);
    return "";
}

std::vector<BuildTimer> ListTimerTaskBuildStrategy::handler(std::string params) {
    std::vector<BuildTimer> results;
    auto buildList = SegmentationDataBase::instance().loadAllBuild();
    for (const auto &build: buildList) {
        BuildTimer buildTimer;
        buildTimer.setBuild(BuildVo(build.id, build.name, build.elevator_address));
        auto mapList = SegmentationDataBase::instance().findBuildMapsForBuild(build.id);
        for (const auto &buildMap: mapList) {
            auto timerList = TaskDataBase::instance().loadTimerFoMap(buildMap.second.id);
            buildTimer.setTimers(timerList);
        }
    }
    return results;
}
