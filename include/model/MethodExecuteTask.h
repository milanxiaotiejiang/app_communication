//
// Created by lijiang on 2021/12/17.
//

#ifndef APP_COMMUNICATION_METHODEXECUTETASK_H
#define APP_COMMUNICATION_METHODEXECUTETASK_H

#include "string"
#include "Task.h"

#include "nlohmann/json.hpp"

using json = nlohmann::json;

class MethodExecuteTask {
private:
    int id;
    std::string method;
    Task params;
public:

    MethodExecuteTask();

    virtual ~MethodExecuteTask();

    MethodExecuteTask(int id, const string &method, const Task &params);

    friend void to_json(json &j, const MethodExecuteTask &b) {
        j = json{
                {"id",     b.id},
                {"method", b.method},
                {"params", b.params},
        };
    }

    friend void from_json(const json &j, MethodExecuteTask &b) {
        j.at("id").get_to(b.id);
        j.at("method").get_to(b.method);
        j.at("params").get_to(b.params);
    }

};


#endif //APP_COMMUNICATION_METHODEXECUTETASK_H
