//
// Created by admin1 on 22-11-22.
//

#include "sub/json/FullCleaningModeStrategy.h"
#include "manager/FullPlanWorkStatusManager.h"
#include "manager/ViewPartManager.h"

//获取全部
//////////////////////////////////////////////////////////////////
std::vector<FullCleanBrief> FullCLeanListStrategy::handler(std::string params) {
    std_msgs::String result;
    FullCleanBriefList full_brief_list_temp;
    auto startegyResult = FullCleanManager::get_instance()->GetFullCleanBriefList(full_brief_list_temp);
    if (startegyResult != SUCCESS_) {
        throw app::exception(make_error_code(error::open_file_fail));
    }
    if (full_brief_list_temp.getFullCleanBriefList().size() > 0) {
        return full_brief_list_temp.getFullCleanBriefList();
    } else {
        return std::vector<FullCleanBrief>();
    }
}

std::string FullCLeaningAddStrategy::handler(FullCleanBrief params) {
    switch (FullCleanManager::get_instance()->AddFullClean(params)) {
        case FAILD_TO_OPEN_FILE_:
            throw app::exception(make_error_code(error::open_file_fail));
        case NAME_DUPLICATE_:
            throw app::exception(make_error_code(error::combined_duplicate_name));
        case EXECUTE_FAILED_:
            throw app::exception(make_error_code(error::add_failed));
        case FAILED_TO_SAVE_FILE_:
            throw app::exception(make_error_code(error::save_failed));
        case SUCCESS_:
            return "";
        default:
            throw app::exception(make_error_code(error::unknown_command));
    }
}

std::string FullCleaningUpdateStrategy::handler(FullCleanBrief params) {
    FullCleanBrief full_clean_brief_temp(params);
    switch (FullCleanManager::get_instance()->ResetFullCLean(full_clean_brief_temp,
                                                             full_clean_brief_temp.getFullPathID())) {
        case FAILD_TO_OPEN_FILE_:
            throw app::exception(make_error_code(error::open_file_fail));
        case EXECUTE_FAILED_:
            throw app::exception(make_error_code(error::operation_failed));
            break;
        case FAILED_TO_SAVE_FILE_:
            throw app::exception(make_error_code(error::save_failed));
        case SUCCESS_:
            return "";
        default:
            throw app::exception(make_error_code(error::unknown_command));
    }
}

std::string FullCLeaningDeleteStrategy::handler(std::string params) {
    switch (FullCleanManager::get_instance()->DeleteFullClean(params)) {
        case FAILD_TO_OPEN_FILE_:
            throw app::exception(make_error_code(error::open_file_fail));
        case EXECUTE_FAILED_:
            throw app::exception(make_error_code(error::operation_failed));
        case FAILED_TO_SAVE_FILE_:
            throw app::exception(make_error_code(error::save_failed));
        case SUCCESS_:
            return "";
        default:
            throw app::exception(make_error_code(error::unknown_command));
    }
}

FullCleanBrief FullCleaningDetailsStrategy::handler(std::string params) {
    std_msgs::String result;
    FullCleanBrief full_clean_brif_temp;
    if (FullCleanManager::get_instance()->GetFullCLeanBrief(full_clean_brif_temp, params) !=
        SUCCESS_) {//访问当前组合成功
        throw app::exception(make_error_code(error::unable_to_get_combination));
    } else {
        return full_clean_brif_temp;
    }
}

std::string FullCleaningMainStrategy::handler(std::string params) {
    switch (FullCleanManager::get_instance()->setMainFullClean(params)) {
        case FAILD_TO_OPEN_FILE_:
            throw app::exception(make_error_code(error::open_file_fail));
        case EXECUTE_FAILED_:
            throw app::exception(make_error_code(error::operation_failed));
        case SUCCESS_:
            return "";
        default:
            throw app::exception(make_error_code(error::unknown_command));
    }
}

std::string CancelFullCleaningMainStrategy::handler(std::string params) {
    switch (FullCleanManager::get_instance()->cancelMainFullCLean(params)) {
        case FAILD_TO_OPEN_FILE_:
            throw app::exception(make_error_code(error::open_file_fail));
        case EXECUTE_FAILED_:
            throw app::exception(make_error_code(error::operation_failed));
        case SUCCESS_:
            return "";
        default:
            throw app::exception(make_error_code(error::unknown_command));
    }
}

