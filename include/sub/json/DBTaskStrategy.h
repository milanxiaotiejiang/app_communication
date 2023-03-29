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

class DeleteTaskStrategy : public MessageStrategy<long, string> {
public:
    string handler(long params) override;
};

class ListTaskStrategy : public MessageStrategy<string, vector<TaskVo>> {
public:
    vector<TaskVo> handler(string params) override;
};

class QueryIdTaskStrategy : public MessageStrategy<long, TaskVo> {
public:
    TaskVo handler(long params) override;
};

class ClearCurrentListTaskStrategy : public MessageStringStrategy {
public:
    void handler() override;
};

class AddTimerTaskStrategy : public MessageStrategy<TimerVo, long> {
public:
    long handler(TimerVo params) override;
};

class DeleteTimerTaskStrategy : public MessageStrategy<long, string> {
public:
    string handler(long params) override;
};

class ListTimerTaskStrategy : public MessageStrategy<string, vector<TimerVo>> {
public:
    vector<TimerVo> handler(string params) override;
};

class BuildPrincipalTaskStrategy : public MessageStrategy<long, TaskVo> {
public:
    TaskVo handler(long params) override;
};

class CancelPrincipalTaskStrategy : public MessageStrategy<long, TaskVo> {
public:
    TaskVo handler(long params) override;
};

class PrincipalTaskStrategy : public MessageStrategy<string, TaskVo> {
public:
    TaskVo handler(string params) override;
};

#endif //APP_COMMUNICATION_DBTASKSTRATEGY_H
