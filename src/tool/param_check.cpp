//
// Created by Looper on 2023/4/3.
//

#include "tool/param_check.h"
#include "BaseThrowable.h"
#include "tool/regex_valid.h"
#include "db/task_model.h"
#include "db/segmentation_data_base.h"

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
    if (is_valid_name(name)) {
        throw app::exception(make_error_code(error::invalid_name));
    }
}

void checkRate(int rate) {
    if (rate < 1 || rate > 4) {
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
