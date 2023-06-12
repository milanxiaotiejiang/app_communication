//
// Created by Looper on 2023/3/20.
//

#ifndef APP_COMMUNICATION_DBTASKSTRATEGY_H
#define APP_COMMUNICATION_DBTASKSTRATEGY_H

#include "sub/json/MessageStrategy.h"
#include "model/task.h"

class AddTaskStrategy : public MessageStrategy<TaskVo, long> {
public:
    long handler(TaskVo params) override;
};

class DeleteTaskStrategy : public MessageStrategy<long, string> {
public:
    string handler(long params) override;
};

class DeleteMultipleTaskStrategy : public MessageStrategy<std::vector<long>, string> {
public:
    string handler(std::vector<long> params) override;
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

class DeleteMultipleTimerTaskStrategy : public MessageStrategy<std::vector<long>, string> {
public:
    string handler(std::vector<long> params) override;
};

class ListTimerTaskStrategy : public MessageStrategy<string, vector<TimerVo>> {
public:
    vector<TimerVo> handler(string params) override;
};

class ModifyTimerTaskStrategy : public MessageStrategy<TimerVo, string> {
public:
    string handler(TimerVo params) override;
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

class ModifyTaskNameStrategy : public MessageStrategy<ModifyTaskName, string> {
public:
    string handler(ModifyTaskName params) override;
};

class ModifyTaskRateStrategy : public MessageStrategy<ModifyTaskRate, string> {
public:
    string handler(ModifyTaskRate params) override;
};

class ModifyTaskWorkStatusStrategy : public MessageStrategy<ModifyTaskWorkStatus, string> {
public:
    string handler(ModifyTaskWorkStatus params) override;
};

class ModifyTaskKnifeStrategy : public MessageStrategy<ModifyTaskKnife, string> {
public:
    string handler(ModifyTaskKnife params) override;
};

class ModifyCompleteTaskStrategy : public MessageStrategy<TaskVo, TaskVo> {
public:
    TaskVo handler(TaskVo params) override;
};

class OperateAddZoneStrategy : public MessageStrategy<ModifyTaskZone, long> {
public:
    long handler(ModifyTaskZone params) override;
};

class OperateDeleteZoneStrategy : public MessageStrategy<ModifyTaskZone, string> {
public:
    string handler(ModifyTaskZone params) override;
};

class OperateModifyZoneStrategy : public MessageStrategy<ModifyTaskZone, string> {
public:
    string handler(ModifyTaskZone params) override;
};

class ModifyTaskPartitionStrategy : public MessageStrategy<ModifyTaskPartition, string> {
public:
    string handler(ModifyTaskPartition params) override;
};

class OperateAddSubregionStrategy : public MessageStrategy<ModifyTaskSubregion, long> {
public:
    long handler(ModifyTaskSubregion params) override;
};

class OperateDeleteSubregionStrategy : public MessageStrategy<ModifyTaskSubregion, string> {
public:
    string handler(ModifyTaskSubregion params) override;
};

class ModifyTimerNameStrategy : public MessageStrategy<ModifyTimerName, string> {
public:
    string handler(ModifyTimerName params) override;
};

#endif //APP_COMMUNICATION_DBTASKSTRATEGY_H
