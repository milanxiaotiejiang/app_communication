#ifndef APP_COMMUNICATION_GETCLEANHISTORYSTRATEGY_H
#define APP_COMMUNICATION_GETCLEANHISTORYSTRATEGY_H

#include "MessageStrategy.h"
#include "model/WorkStatus.h"
#include "model/CleanHistory.h"
#include "clean_history/CleanHistoryCenter.h"
#include "ros/package.h"
#include "tool/write_file.hpp"
#include <thread>
#include "model/UpgradeModel.h"


class GetCleanHistoryStrategy : public MessageStrategy<std::string, std::vector<CleanHistory>> {//收到清洁历史
public:
    std::vector<CleanHistory> handler(std::string params) override;
};

class GetCloudCleanHistoryStrategy : public MessageStrategy<std::string, std::vector<CleanHistoryUpgrade>> {//收到清洁历史
public:
    std::vector<CleanHistoryUpgrade> handler(std::string params) override;
};


#endif //APP_COMMUNICATION_GETCLEANHISTORYSTRATEGY_H