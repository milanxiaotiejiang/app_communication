//
// Created by Looper on 2023/3/20.
//

#ifndef APP_COMMUNICATION_DBTASKSTRATEGY_H
#define APP_COMMUNICATION_DBTASKSTRATEGY_H

#include "MessageStrategy.h"
#include "model/task.h"

class AddTaskStrategy : public MessageStrategy<TaskVo, long> {
public:
    long handler(TaskVo params) override;
};

class DeleteTaskStrategy : public MessageStrategy<int, string> {
public:
    string handler(int params) override;
};

class ListTaskStrategy : public MessageStrategy<string, vector<TaskVo>> {
public:
    vector<TaskVo> handler(string params) override;
};

class QueryIdTaskStrategy : public MessageStrategy<long, TaskVo> {
public:
    TaskVo handler(long params) override;
};

class AddTimerStrategy : public MessageStrategy<TimerVo, int> {
public:
    int handler(TimerVo params) override;
};

class DeleteTimerStrategy : public MessageStrategy<int, string> {
public:
    string handler(int params) override;
};

class ListTimerStrategy : public MessageStrategy<string, vector<TimerVo>> {
public:
    vector<TimerVo> handler(string params) override;
};

#endif //APP_COMMUNICATION_DBTASKSTRATEGY_H
