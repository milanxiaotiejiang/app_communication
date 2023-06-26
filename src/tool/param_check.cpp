//
// Created by Looper on 2023/4/3.
//

#include "tool/param_check.h"
#include "BaseThrowable.h"
#include "tool/regex_valid.h"
#include "db/task_model.h"
#include "db/segmentation_data_base.h"
#include "db/task_data_base.h"
#include "schedule/schedule_manager.h"

void checkWorkStatus(const WorkStatus &workStatus) {
    if (workStatus.getSweepStatus() < -1 || workStatus.getSweepStatus() > 4) {
        throw app::exception(make_error_code(error::invalid_sweep_status));
    }
    if (workStatus.getMopStatus() < -1 || workStatus.getMopStatus() > 4) {
        throw app::exception(make_error_code(error::invalid_mop_status));
    }
    if (workStatus.getVacuumStatus() < -1 || workStatus.getVacuumStatus() > 4) {
        throw app::exception(make_error_code(error::invalid_vacuum_status));
    }
    if (workStatus.getPushStatus() < -1 || workStatus.getPushStatus() > 4) {
        throw app::exception(make_error_code(error::invalid_push_status));
    }
    if (workStatus.getAromatherapyStatus() < -1 || workStatus.getAromatherapyStatus() > 4) {
        throw app::exception(make_error_code(error::invalid_aromatherapy_status));
    }
    if (workStatus.getDisinfectStatus() < -1 || workStatus.getDisinfectStatus() > 4) {
        throw app::exception(make_error_code(error::invalid_disinfect_status));
    }
}

void checkName(const std::string &name) {
    if (!is_valid_name(name)) {
        throw app::exception(make_error_code(error::invalid_name));
    }
}

void checkRate(int rate) {
    if (rate < 1 || rate > 9) {
        throw app::exception(make_error_code(error::invalid_rate));
    }
}

void checkMode(int mode) {
    if (mode < static_cast<int>(TaskMode::Zoned) || mode > static_cast<int>(TaskMode::Line)) {
        throw app::exception(make_error_code(error::invalid_mode));
    }
}

void checkSource(const std::string &source) {
    if (source != "App" && source != "Pad" && source != "Cloud") {
        throw app::exception(make_error_code(error::invalid_source));
    }
}

void checkZoned(const ZoneVo &zone) {
    if (zone.getPoints().size() != 4) {
        throw app::exception(make_error_code(error::invalid_zones));
    }
}

void checkZoned(const std::vector<ZoneVo> &zones) {
    if (zones.empty()) {
        throw app::exception(make_error_code(error::invalid_zones));
    } else {
        bool zone_standard = true;
        for (const auto &child: zones) {
            if (child.getPoints().size() != 4) {
                zone_standard = false;
                break;
            }
        }
        if (!zone_standard) {
            throw app::exception(make_error_code(error::invalid_zones));
        }
    }
}

void checkSubregion(const SubregionVo &subregion) {
    try {
        const RoomPo &po = SegmentationDataBase::instance().selectRoomById(subregion.getSubregionValue());
    } catch (...) {
        throw app::exception(make_error_code(error::invalid_subregions));
    }
}

void checkSubregion(const std::vector<SubregionVo> &subregions) {
    if (subregions.empty()) {
        throw app::exception(make_error_code(error::invalid_subregions));
    } else {
        bool sub_pass = true;
        for (const auto &subregion: subregions) {
            try {
                const RoomPo &po = SegmentationDataBase::instance().selectRoomById(subregion.getSubregionValue());
            } catch (...) {
                sub_pass = false;
            }
            if (!sub_pass) {
                break;
            }
        }
        if (!sub_pass) {
            throw app::exception(make_error_code(error::invalid_subregions));
        }
    }
}

void checkSameTimer(const std::string &map_id, const std::string &timer_rule, int selfTimerId) {
    std::string repair_timer_rule;
    try {
        repair_timer_rule = ScheduleManager::fix_cron_expression("0 " + timer_rule);
    } catch (const cron::bad_cronexpr &ex) {
        std::cerr << "Invalid cron expression: " << ex.what() << std::endl;
        throw std::invalid_argument("Invalid timer_rule");
    }

    std::time_t endTime = std::time(nullptr) + 7 * 24 * 60 * 60;
//    std::time_t endTime = std::numeric_limits<std::time_t>::max();

    std::vector<std::time_t> originalPoints = ScheduleManager::cronTimePoints(repair_timer_rule, endTime);

    auto timers = TaskDataBase::instance().loadTimerFoMap(map_id);
    std::vector<std::chrono::system_clock::time_point> timePoints;
    for (const auto &item: timers) {
        if (item.getTimerId() == selfTimerId) {
            continue;
        }
        LOG_IF(INFO, DEBUG_TIMER) << "checkSameTimer" <<
                                  "  originalPoints : " << timer_rule <<
                                  "  targetExpression : " << item.getTimerRule();
        const std::string &targetExpression = ScheduleManager::fix_cron_expression("0 " + item.getTimerRule());
        bool has = ScheduleManager::hasSameTimePoint(originalPoints, targetExpression, endTime);
        if (has) {
            throw std::invalid_argument("存在相同的时间点");
        }
    }
}
