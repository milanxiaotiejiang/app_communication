//
// Created by Looper on 2023/3/7.
//

#ifndef APP_COMMUNICATION_ONCE_CONFIRM_H
#define APP_COMMUNICATION_ONCE_CONFIRM_H

#include "node_chain.h"

class OnceConfirm {
public:
    OnceConfirm() = default;;

    ~OnceConfirm() = default;;

    static bool confirm(NodeChain chain);

};


#endif //APP_COMMUNICATION_ONEC_CONFIRM_H
