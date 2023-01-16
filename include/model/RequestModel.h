//
// Created by zt on 2022/3/8.
//

#ifndef C_T_REQUESTMODEL_H
#define C_T_REQUESTMODEL_H

#include <iostream>
#include "string"
#include "RegisterOperation.h"

using namespace std;

template<class T>
class RequestModel {
private:
    string op;
    string topic;
    T msg;

public:
    RequestModel() = default;

    RequestModel(const string &op, const string &topic, T msg) : op(op), topic(topic), msg(msg) {}

    virtual ~RequestModel() = default;

    const string &getOp() const {
        return op;
    }

    void setOp(const string &op) {
        RequestModel::op = op;
    }

    const string &getTopic() const {
        return topic;
    }

    void setTopic(const string &topic) {
        RequestModel::topic = topic;
    }

    T getMsg() const {
        return msg;
    }

    void setMsg(T msg) {
        RequestModel::msg = msg;
    }

    friend void to_json(json &j, const RequestModel &requestModel) {
        j = json{
                {"op",    requestModel.op},
                {"topic", requestModel.topic},
                {"msg",   requestModel.msg}
        };
    }

    friend void from_json(const json &j, RequestModel &requestModel) {
        j.at("op").get_to(requestModel.op);
        j.at("topic").get_to(requestModel.topic);
        j.at("msg").get_to(requestModel.msg);
    }
};


class AcceptRequestModel {
private:
    string op;
    string topic;

public:
    AcceptRequestModel() = default;

    AcceptRequestModel(const string &op, const string &topic) : op(op), topic(topic) {}

    virtual ~AcceptRequestModel() = default;

    const string &getOp() const {
        return op;
    }

    void setOp(const string &op) {
        AcceptRequestModel::op = op;
    }

    const string &getTopic() const {
        return topic;
    }

    void setTopic(const string &topic) {
        AcceptRequestModel::topic = topic;
    }

    friend void to_json(json &j, const AcceptRequestModel &requestModel) {
        j = json{
                {"op",    requestModel.op},
                {"topic", requestModel.topic},
        };
    }

    friend void from_json(const json &j, AcceptRequestModel &requestModel) {
        j.at("op").get_to(requestModel.op);
        j.at("topic").get_to(requestModel.topic);
    }
};

#endif //C_T_REQUESTMODEL_H
