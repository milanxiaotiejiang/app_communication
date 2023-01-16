//
// Created by mi on 2022/8/1.
//

#ifndef APP_COMMUNICATION_MATERIALSTRATEGY_H
#define APP_COMMUNICATION_MATERIALSTRATEGY_H

#include "MessageStrategy.h"
#include "model/MaterialStatus.h"

class GetMaterialStrategy : public MessageStrategy<string, VersionSubscribe<MaterialStatus>> {
public:
    VersionSubscribe<MaterialStatus> handler(string params) override;
};


#endif //APP_COMMUNICATION_MATERIALSTRATEGY_H
