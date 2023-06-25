//
// Created by mi on 2022/8/10.
//

#ifndef APP_COMMUNICATION_NOTICESTRATEGY_H
#define APP_COMMUNICATION_NOTICESTRATEGY_H

#include "MessageStrategy.h"
#include "tool/write_file.hpp"
#include "model/Notice.h"
#include "manager/NoticeManager.h"

class NoticeListStrategy : public MessageStrategy<std::string, std::vector<Notice>> {
public:
    std::vector<Notice> handler(std::string params) override;
};


#endif //APP_COMMUNICATION_NOTICESTRATEGY_H
