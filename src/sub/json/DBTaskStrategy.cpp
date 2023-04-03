//
// Created by Looper on 2023/3/20.
//

#include <regex>
#include "sub/json/DBTaskStrategy.h"
#include "db/segmentation_data_base.h"
#include "db/task_data_base.h"
#include "tool/regex_valid.h"
#include "tool/param_check.h"

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
    TaskDataBase::instance().deleteTaskFoId(params);
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
    const string &timerRule = params.getTimerRule();
    if (!is_valid_crontab(timerRule)) {
        throw std::invalid_argument("Invalid timer_rule");
    }
    int rate = params.getRate();
    if (rate < 1 || rate > 4) {
        throw std::invalid_argument("Invalid rate");
    }

    MapPo map = SegmentationDataBase::instance().getDbMap();
    return TaskDataBase::instance().addTimer(map.id, params);
}

string DeleteTimerTaskStrategy::handler(long params) {
    TaskDataBase::instance().deleteTimerForId(params);
    return "";
}

vector<TimerVo> ListTimerTaskStrategy::handler(string params) {
    MapPo map = SegmentationDataBase::instance().getDbMap();
    return TaskDataBase::instance().loadTimerFoMap(map.id);
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
