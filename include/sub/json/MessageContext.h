//
// Created by lijiang on 2021/12/18.
//

#ifndef APP_COMMUNICATION_MESSAGECONTEXT_H
#define APP_COMMUNICATION_MESSAGECONTEXT_H

#include "MessageStrategy.h"
#include<std_msgs/String.h>
#include <nlohmann/json.hpp>
#include <pub/PubOut.h>
#include <pub/PubInner.h>

class MessageContext {
private:
    MessageBaseStrategy *messageStrategy;
public:
    MessageContext(MessageBaseStrategy *messageStrategy) :
            messageStrategy(messageStrategy) {}

    void startDateProgressing(int source, json &result) {
        messageStrategy->dateProgressing(source, result);
    }
};


#endif //APP_COMMUNICATION_MESSAGECONTEXT_H
