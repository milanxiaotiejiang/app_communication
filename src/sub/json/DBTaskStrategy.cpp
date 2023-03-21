//
// Created by Looper on 2023/3/20.
//

#include <regex>
#include "sub/json/DBTaskStrategy.h"
#include "db/segmentation_data_base.h"
#include "db/task_data_base.h"
#include "tool/regex_valid.h"

long AddTaskStrategy::handler(TaskVo params) {
    const WorkStatus &workStatus = params.getWorkStatus();
    if (workStatus.getSweepStatus() < -1 || workStatus.getSweepStatus() > 4) {
        throw std::invalid_argument("Invalid sweep_status");
    }
    if (workStatus.getMopStatus() < -1 || workStatus.getMopStatus() > 4) {
        throw std::invalid_argument("Invalid mop_status");
    }
    if (workStatus.getVacuumStatus() < -1 || workStatus.getVacuumStatus() > 4) {
        throw std::invalid_argument("Invalid vacuum_status");
    }
    if (workStatus.getPushStatus() < -1 || workStatus.getPushStatus() > 4) {
        throw std::invalid_argument("Invalid push_status");
    }
    if (workStatus.getAromatherapyStatus() < -1 || workStatus.getAromatherapyStatus() > 4) {
        throw std::invalid_argument("Invalid aromatherapy_status");
    }
    if (workStatus.getDisinfectStatus() < -1 || workStatus.getDisinfectStatus() > 4) {
        throw std::invalid_argument("Invalid disinfect_status");
    }
    int mode = params.getMode();
    if (mode < static_cast<int>(TaskMode::Zoned) || mode > static_cast<int>(TaskMode::Line)) {
        throw std::invalid_argument("Invalid mode");
    }
    const string &source = params.getSource();
    if (source != "App" && source != "Pad" && source != "Cloud") {
        throw std::invalid_argument("Invalid source");
    }
    if (mode == static_cast<int>(TaskMode::Zoned)) {
        vector<std::vector<PointVo>> zones = params.getZones();
        if (zones.empty()) {
            throw std::invalid_argument("Invalid zones");
        } else {
            bool zone_standard = true;
            for (const auto &child: zones) {
                if (child.size() != 4) {
                    zone_standard = false;
                    break;
                }
            }
            if (!zone_standard) {
                throw std::invalid_argument("Invalid zones");
            }
        }
    }
    const string &name = params.getName();
    if (is_valid_name(name)) {
        throw std::invalid_argument("Invalid name");
    }
    int rate = params.getRate();
    if (rate < 1 || rate > 4) {
        throw std::invalid_argument("Invalid rate");
    }

    MapPo map = SegmentationDataBase::instance().getDbMap();
    return TaskDataBase::instance().addTask(map.id, params);
}

string DeleteTaskStrategy::handler(int params) {
    TaskDataBase::instance().deleteTaskFoId(params);
}

vector<TaskVo> ListTaskStrategy::handler(string params) {
    MapPo map = SegmentationDataBase::instance().getDbMap();
    return TaskDataBase::instance().loadTaskFoMap(map.id);
}

TaskVo QueryIdTaskStrategy::handler(long params) {
    return TaskDataBase::instance().loadTaskFoId(params);
}

int AddTimerStrategy::handler(TimerVo params) {
    const TaskVo &taskVo = TaskDataBase::instance().loadTaskFoId(params.getTaskId());
    if (taskVo.getName() != params.getTaskName()) {
        throw std::invalid_argument("Invalid task_name");
    }
    const string &timer_name = params.getTimerName();
    if (is_valid_name(timer_name)) {
        throw std::invalid_argument("Invalid timer_name");
    }
    const string &timerRule = params.getTimerRule();
    if (is_valid_crontab(timerRule)) {
        throw std::invalid_argument("Invalid timer_rule");
    }
    int rate = params.getRate();
    if (rate < 1 || rate > 4) {
        throw std::invalid_argument("Invalid rate");
    }

    MapPo map = SegmentationDataBase::instance().getDbMap();
    TaskDataBase::instance().addTimer(map.id, params);
    return 0;
}

string DeleteTimerStrategy::handler(int params) {
    TaskDataBase::instance().deleteTimerForId(params);
}

vector<TimerVo> ListTimerStrategy::handler(string params) {
    MapPo map = SegmentationDataBase::instance().getDbMap();
    return TaskDataBase::instance().loadTimerFoMap(map.id);
}
