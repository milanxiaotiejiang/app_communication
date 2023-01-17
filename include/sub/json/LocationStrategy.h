//
// Created by lijiang on 2022/5/25.
//

#ifndef APP_COMMUNICATION_LOCATIONSTRATEGY_H
#define APP_COMMUNICATION_LOCATIONSTRATEGY_H

#include <nlohmann/json.hpp>

#include "pub/PubInner.h"
#include "pub/PubOut.h"

#include "model/BaseResult.h"
#include "model/Location.h"
#include "MessageStrategy.h"

#include "tool/write_file.hpp"

#include "ros/package.h"

using json = nlohmann::json;
using namespace std;


class LocationStrategy : public MessageStrategy<Location, string> {
public:
    string handler(Location params) override;
};

class getLocationStrategy : public MessageStringStrategy {
public:
    void handler() override;
};


#endif //APP_COMMUNICATION_LOCATIONSTRATEGY_H
