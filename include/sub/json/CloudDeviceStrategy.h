//
// Created by mi on 2022/8/12.
//

#ifndef APP_COMMUNICATION_CLOUDDEVICESTRATEGY_H
#define APP_COMMUNICATION_CLOUDDEVICESTRATEGY_H

#include "MessageStrategy.h"

class GetDeviceSecretStrategy : public MessageStrategy<string, string> {
public:
    string handler(string method) override;
};


#endif //APP_COMMUNICATION_CLOUDDEVICESTRATEGY_H
