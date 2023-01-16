//
// Created by mi on 2022/8/10.
//

#ifndef APP_COMMUNICATION_NOTICESTRATEGY_H
#define APP_COMMUNICATION_NOTICESTRATEGY_H

#include "MessageStrategy.h"
#include "tool/read_partion_file.h"
#include "tool/write_file.hpp"
#include "model/Notice.h"
#include "manager/NoticeManager.h"

class NoticeListStrategy : public MessageStrategy<string, vector<Notice>> {
public:
    vector<Notice> handler(string params) override;
};


#endif //APP_COMMUNICATION_NOTICESTRATEGY_H
