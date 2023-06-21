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

string DeleteTaskStrategy::handler(long params) {
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
    return "";
}

string DeleteMultipleTaskStrategy::handler(std::vector<long> params) {
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

vector<TaskVo> ListTaskStrategy::handler(string params) {
    MapPo map = SegmentationDataBase::instance().getDbMap();
    return TaskDataBase::instance().loadTaskFoMap(map.id);
}

TaskVo QueryIdTaskStrategy::handler(long params) {
    return TaskDataBase::instance().loadTaskFoId(params);
}

void ClearCurrentListTaskStrategy::handler() {
    TaskDataBase::instance().deleteTaskFoMap(SegmentationDataBase::instance().getDbMap().id);
}

long AddTimerTaskStrategy::handler(TimerVo params) {
    const TaskVo &taskVo = TaskDataBase::instance().loadTaskFoId(params.getTaskId());
    checkName(params.getTaskName());
    checkName(params.getTimerName());
    checkRate(params.getRate());

    MapPo map = SegmentationDataBase::instance().getDbMap();

    checkSameTimer(map.id, params.getTimerRule(), -1);

    long timer = TaskDataBase::instance().addTimer(map.id, params);

    ScheduleManagerSingleton::instance().trigger_task_update();
    return timer;
}

string DeleteTimerTaskStrategy::handler(long params) {
    TaskDataBase::instance().deleteTimerForId(params);
    ScheduleManagerSingleton::instance().trigger_task_update();
    return "";
}

string DeleteMultipleTimerTaskStrategy::handler(std::vector<long> params) {
    for (const auto &item: params) {
        TaskDataBase::instance().deleteTimerForId(item);
    }
    ScheduleManagerSingleton::instance().trigger_task_update();
    return "";
}

vector<TimerVo> ListTimerTaskStrategy::handler(string params) {
    MapPo map = SegmentationDataBase::instance().getDbMap();
    return TaskDataBase::instance().loadTimerFoMap(map.id);
}

string ModifyTimerTaskStrategy::handler(TimerVo params) {
    MapPo map = SegmentationDataBase::instance().getDbMap();

    checkName(params.getTimerName());
    checkRate(params.getRate());
    checkSameTimer(map.id, params.getTimerRule(), params.getTimerId());

    TaskDataBase::instance().modifyTimer(map.id, params);
    ScheduleManagerSingleton::instance().trigger_task_update();
    return "";
}

TaskVo BuildPrincipalTaskStrategy::handler(long params) {
    MapPo map = SegmentationDataBase::instance().getDbMap();
    return TaskDataBase::instance().modifyPrincipalTask(map.id, params, true);
}

TaskVo CancelPrincipalTaskStrategy::handler(long params) {
    MapPo map = SegmentationDataBase::instance().getDbMap();
    return TaskDataBase::instance().modifyPrincipalTask(map.id, params, false);
}

TaskVo PrincipalTaskStrategy::handler(string params) {
    MapPo map = SegmentationDataBase::instance().getDbMap();
    const TaskVo &vo = TaskDataBase::instance().loadPrincipalTask(map.id);
    if (vo.getId() == -1) {
        throw app::exception(make_error_code(error::the_main_task_is_not_set));
    }
    return vo;
}

TaskVo BuildRainSnowTaskStrategy::handler(long params) {
    MapPo map = SegmentationDataBase::instance().getDbMap();
    const TaskVo &taskVo = TaskDataBase::instance().loadTaskFoId(params);
    if (SqliteDataBase::TaskModeFromInt(taskVo.getMode()) != TaskMode::Zoned) {
        throw app::exception(make_error_code(error::non_zoning_tasks_cannot_be_set_as_rainy_and_snowy_tasks));
    }
    return TaskDataBase::instance().modifyRainSnowTask(map.id, params, true);
}

TaskVo CancelRainSnowTaskStrategy::handler(long params) {
    MapPo map = SegmentationDataBase::instance().getDbMap();
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
    return TaskDataBase::instance().modifyRainSnowTask(map.id, params, false);
}

TaskVo RainSnowTaskStrategy::handler(string params) {
    MapPo map = SegmentationDataBase::instance().getDbMap();
    const TaskVo &vo = TaskDataBase::instance().loadRainSnowTask(map.id);
    if (vo.getId() == -1) {
        throw app::exception(make_error_code(error::the_rain_snow_task_is_not_set));
    }
    return vo;
}

string ModifyTaskNameStrategy::handler(ModifyTaskName params) {
    checkName(params.name);
    TaskDataBase::instance().modifyName(params.id, params.name);
    return "";
}

string ModifyTaskRateStrategy::handler(ModifyTaskRate params) {
    checkRate(params.rate);
    TaskDataBase::instance().modifyRate(params.id, params.rate);
    return "";
}

string ModifyTaskWorkStatusStrategy::handler(ModifyTaskWorkStatus params) {
    checkWorkStatus(params.workStatus);
    TaskDataBase::instance().modifyWorkStatus(params.id, params.workStatus);
    return "";
}

string ModifyTaskKnifeStrategy::handler(ModifyTaskKnife params) {
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

string OperateDeleteZoneStrategy::handler(ModifyTaskZone params) {
    TaskDataBase::instance().operateDeleteZone(params.id, params.zone);
    return "";
}

string OperateModifyZoneStrategy::handler(ModifyTaskZone params) {
    checkZoned(params.zone);
    TaskDataBase::instance().operateModifyZone(params.id, params.zone);
    return "";
}

string ModifyTaskPartitionStrategy::handler(ModifyTaskPartition params) {
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

string OperateDeleteSubregionStrategy::handler(ModifyTaskSubregion params) {
    TaskDataBase::instance().operateDeleteSubregion(params.id, params.subregion);
}

string ModifyTimerNameStrategy::handler(ModifyTimerName params) {
    checkName(params.timer_name);
    TaskDataBase::instance().modifyTimerName(params.id, params.timer_name);
    return "";
}
