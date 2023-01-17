//
// Created by Looper on 2022/12/8.
//

#include "task/sub/HistoryStrategy.h"

PageResult<clean_history_db::CleanHistoryVo> GetHistoryListStrategy::handler(Page<std::string> params) {
    int page_to_get = params.getPageNo();
    int page_size = params.getPageSize();
    int start_index = (page_to_get - 1) * page_size;
    int end_index = start_index + page_size;
    std::vector<clean_history_db::CleanHistory> clean_histories = clean_history_db::CleanHistoryCenter::instance().getAllCleanHistories();
    int history_size = clean_histories.size();

    PageResult<clean_history_db::CleanHistoryVo> result;
    result.setPageNum(page_to_get);
    result.setPageSize(page_size);
    result.setTotal(history_size / page_size + 1);
    std::vector<clean_history_db::CleanHistoryVo> &result_data = result.getData();
    for (int i = start_index; i < end_index && i < history_size; i++) {
        clean_history_db::CleanHistory &history = clean_histories[i];
        result_data.push_back(clean_history_db::CleanHistoryVo(history.task_id_, history.mode_, history.rate_,
                                                               history.sweep_status_, history.mop_status_,
                                                               history.vacuum_status_, history.push_status_,
                                                               history.aromatherapy_status_, history.disinfect_status_,
                                                               history.launch_people_, history.time_mode_,
                                                               history.launch_time_, history.execute_time_,
                                                               history.clean_time_, history.end_time_,
                                                               history.clean_area_,
                                                               history.current_step_, history.current_frequency_,
                                                               history.total_step_, history.total_frequency_,
                                                               history.history_state_, history.current_frequency_,
                                                               history.urgency_stop_, history.pause_num_,
                                                               history.manual_back_, history.low_power_back_,
                                                               history.force_back_, history.out_station_,
                                                               history.end_sleep_, history.back_base_retries_,
                                                               history.back_base_point_arrived_,
                                                               history.station_arrived_, history.recharge_retries_,
                                                               history.close_mechanism_,
                                                               history.open_mechanism_, history.error_code_,
                                                               history.error_msg_));
    }

    return PageResult<clean_history_db::CleanHistoryVo>();
}
