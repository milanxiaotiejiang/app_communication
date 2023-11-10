//
// Created by mi on 2022/8/10.
//

#ifndef APP_COMMUNICATION_OTASTRATEGY_H
#define APP_COMMUNICATION_OTASTRATEGY_H

#include "MessageStrategy.h"
#include "tool/write_file.hpp"
#include "model/OtaInfo.h"

class otaStrategy : public MessageStrategy<OtaInfo, std::string> {
public:
    std::string handler(OtaInfo params) override;

    int handleCore(OtaInfo &params);

    int handlePad(OtaInfo &params);

    int handleEcu(OtaInfo &params);
};


#endif //APP_COMMUNICATION_OTASTRATEGY_H
