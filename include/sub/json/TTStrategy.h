//
// Created by lijiang on 2021/12/18.
//

#ifndef APP_COMMUNICATION_TTSTRATEGY_H
#define APP_COMMUNICATION_TTSTRATEGY_H

#include "MessageStrategy.h"
#include <geometry_msgs/Twist.h>
#include "net/base/BaseMethod.h"
#include "net/base/BaseResult.h"
#include <manager/NoticeManager.h>
#include <fstream>

using json = nlohmann::json;

class TTErrorCheck : public MessageStrategy<std::string, std::string> {
public:
    std::string handler(std::string params) override;
};


#endif //APP_COMMUNICATION_TTSTRATEGY_H
