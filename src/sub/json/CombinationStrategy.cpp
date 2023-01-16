/**
 * @file CombinationStrategy.cpp
 * @author your name (you@domain.com)
 * @brief 路径组合相关接口
 * @version 0.1
 * @date 2022-03-04
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "sub/json/CombinationStartegy.h"

//添加
///////////////////////////////////////////////////////////////////
string CombinationCombinationAddStartegy::handler(CombinationDetail params) {
    switch (CombinationManager::get_instance()->AddCombination(params)) {
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

//获取全部
//////////////////////////////////////////////////////////////////
vector<CombinationBrief> CombinationCombinationListStrategy::handler(string params) {
    std_msgs::String result;
    CombinationBriefList combination_brief_list_temp;
    auto startegyResult = CombinationManager::get_instance()->GetCombinationBriefList(combination_brief_list_temp);
    if (startegyResult != SUCCESS_) {
        throw app::exception(make_error_code(error::open_file_fail));
    }
    if (combination_brief_list_temp.getCombinationBriefList().size() > 0) {
        return combination_brief_list_temp.getCombinationBriefList();
    } else {
        return vector<CombinationBrief>();
    }
}


vector<CombinationBriefUpgrade> CombinationCombinationListStrategyV2::handler(string params) {
    std_msgs::String result;
    CombinationBriefList combination_brief_list_temp;
    auto startegyResult = CombinationManager::get_instance()->GetCombinationBriefList(combination_brief_list_temp);
    if (startegyResult != SUCCESS_) {
        throw app::exception(make_error_code(error::unknown_command));
    }
    if (combination_brief_list_temp.getCombinationBriefList().size() > 0) {
        vector<CombinationBriefUpgrade> list;
        for (const auto &item: combination_brief_list_temp.getCombinationBriefList()) {
            auto combination = CombinationBriefUpgrade();
            combination.setCombinationId(item.getCombinationID());
            combination.setName(item.getName());
            combination.setRate(item.getRate());
            combination.setPartIdList(item.getPartIDList());
            combination.setCombinationType(item.getCombinationType());
            list.push_back(combination);
        }
        return list;
    } else {
        return vector<CombinationBriefUpgrade>();
    }
}

//获取详情
//////////////////////////////////////////////////////////
CombinationDetail CombinationCombinationDetailsStrategy::handler(string params) {
    std_msgs::String result;                         //回复app
    CombinationBrief combination_brief_temp;
    ViewPartList view_part_list_temp;
    if (ViewPartManager::get_instance()->GetViewPartList(view_part_list_temp) != SUCCESS_) {//访问ViewPartList成功
        throw app::exception(make_error_code(error::cannot_access_view_part_list));
    } else if (CombinationManager::get_instance()->GetCombination(combination_brief_temp, params) !=
               SUCCESS_) {//访问当前组合成功
        throw app::exception(make_error_code(error::unable_to_get_combination));
    } else {
        CombinationDetail combination_detail_temp(combination_brief_temp);
        vector<string> part_not_mached = combination_brief_temp.toDetail(combination_detail_temp,
                                                                         view_part_list_temp);//用于验证是否有未匹配的iewpart
        if (part_not_mached.size() > 0) {
            for (auto &item: part_not_mached) {
                CombinationManager::get_instance()->DelatePartID(item);
            }
            throw app::exception(make_error_code(error::there_are_unmatched_parts));
        } else {//正常
            return combination_detail_temp;
        }
    }
}

CombinationDetailUpgrade CombinationCombinationDetailsStrategyV2::handler(string params) {
    std_msgs::String result;                         //回复app
    cout << "要获取id:" << params << "的详情" << endl;
    CombinationBrief combination_brief_temp;
    ViewPartList view_part_list_temp;
    if (ViewPartManager::get_instance()->GetViewPartList(view_part_list_temp) != SUCCESS_) {//访问ViewPartList成功
        throw app::exception(make_error_code(error::cannot_access_view_part_list));
    } else if (CombinationManager::get_instance()->GetCombination(combination_brief_temp, params) !=
               SUCCESS_) {//访问当前组合成功
        throw app::exception(make_error_code(error::unable_to_get_combination));
    } else {
        CombinationDetail combination_detail_temp(combination_brief_temp);
        vector<string> part_not_mached = combination_brief_temp.toDetail(combination_detail_temp,
                                                                         view_part_list_temp);//用于验证是否有未匹配的iewpart
        if (part_not_mached.size() > 0) {
            for (auto &item: part_not_mached) {
                CombinationManager::get_instance()->DelatePartID(item);
            }
            throw app::exception(make_error_code(error::there_are_unmatched_parts));
        } else {//正常
            vector<ViewPartUpgrade> list;
            for (const auto &item: combination_detail_temp.getViewPartList()) {
                WorkStatusUpgrade ws;
                ws.setSweepStatus(item.getWorkStatus().getSweepStatus());
                ws.setDragStatus(item.getWorkStatus().getMopStatus());
                ws.setAbsorbStatus(item.getWorkStatus().getVacuumStatus());
                ws.setPushStatus(item.getWorkStatus().getPushStatus());
                auto viewPart = ViewPartUpgrade();
                viewPart.setMode(item.getMode());
                viewPart.setName(item.getName());
                viewPart.setPartId(item.getPartID());
                viewPart.setPolygon(item.getPolygon());
                viewPart.setRate(item.getRate());
                viewPart.setTeach(item.getTeach());
                viewPart.setPathId(item.getPathID());
                viewPart.setWorkStatus(ws);
                viewPart.setZoned(item.getZoned());
                list.push_back(viewPart);
            }
            CombinationDetailUpgrade combinationDetailUpgrade(
                    combination_detail_temp.getCombinationID(),
                    combination_detail_temp.getName(),
                    combination_detail_temp.getRate(),
                    list);
            return combinationDetailUpgrade;
        }
    }
}

//删除
//////////////////////////////////////////////////////////////
string CombinationCombinationDeleteStrategy::handler(string params) {
    switch (CombinationManager::get_instance()->DelateCombination(params)) {
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

//更新
////////////////////////////////////////////////////////////////

string CombinationCombinationUpdateStrategy::handler(CombinationDetail params) {
    CombinationBrief combination_brief_temp(params);
    switch (CombinationManager::get_instance()->ResetCombination(combination_brief_temp,
                                                                 combination_brief_temp.getCombinationID())) {
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


string CombinationMainStrategy::handler(string params) {
    switch (CombinationManager::get_instance()->setMainCombination(params)) {
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

string CancelCombinationMainStrategy::handler(string params) {
    switch (CombinationManager::get_instance()->cancelMainCombination(params)) {
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
