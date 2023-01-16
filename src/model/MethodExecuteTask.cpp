//
// Created by lijiang on 2021/12/17.
//

#include "model/MethodExecuteTask.h"

MethodExecuteTask::MethodExecuteTask(int id, const string &method, const Task &params) : id(id), method(method),
                                                                                         params(params) {}

MethodExecuteTask::MethodExecuteTask() {}

MethodExecuteTask::~MethodExecuteTask() {

}
