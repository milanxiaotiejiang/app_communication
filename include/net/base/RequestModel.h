//
// Created by zt on 2022/3/8.
//

#ifndef C_T_REQUESTMODEL_H
#define C_T_REQUESTMODEL_H

#include <iostream>
#include "string"

#include "nlohmann/json.hpp"

using json = nlohmann::json;

template<class T>
class RequestModel {
private:
    std::string op;
    std::string topic;
    T msg;

public:
    RequestModel() = default;

    RequestModel(const std::string &op, const std::string &topic, T msg) : op(op), topic(topic), msg(msg) {}

    virtual ~RequestModel() = default;

    const std::string &getOp() const {
        return op;
    }

    void setOp(const std::string &op) {
        RequestModel::op = op;
    }

    const std::string &getTopic() const {
        return topic;
    }

    void setTopic(const std::string &topic) {
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
    std::string op;
    std::string topic;

public:
    AcceptRequestModel() = default;

    AcceptRequestModel(const std::string &op, const std::string &topic) : op(op), topic(topic) {}

    virtual ~AcceptRequestModel() = default;

    const std::string &getOp() const {
        return op;
    }

    void setOp(const std::string &op) {
        AcceptRequestModel::op = op;
    }

    const std::string &getTopic() const {
        return topic;
    }

    void setTopic(const std::string &topic) {
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
