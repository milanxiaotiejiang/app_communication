//
// Created by lijiang on 2021/12/18.
//

#ifndef APP_COMMUNICATION_ABSTRACTSUBSCRIBE_H
#define APP_COMMUNICATION_ABSTRACTSUBSCRIBE_H


#include <pub/PubInner.h>
#include <pub/PubOut.h>

class AbstractSubscribe {

private:
    PubInner pubInner;
    PubOut pubOut;
};


#endif //APP_COMMUNICATION_ABSTRACTSUBSCRIBE_H
