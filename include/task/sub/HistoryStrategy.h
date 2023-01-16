//
// Created by Looper on 2022/12/8.
//

#ifndef APP_COMMUNICATION_HISTORYSTRATEGY_H
#define APP_COMMUNICATION_HISTORYSTRATEGY_H

#include "sub/json/MessageStrategy.h"
#include "clean_history/CleanHistoryCenter.h"
#include "clean_history/CleanHistoryVo.h"
#include "model/Page.h"

class GetHistoryListStrategy : public MessageStrategy<Page<std::string>, PageResult<clean_history_db::CleanHistoryVo>> {
public:
    PageResult<clean_history_db::CleanHistoryVo> handler(Page<std::string> params) override;
};


#endif //APP_COMMUNICATION_HISTORYSTRATEGY_H
