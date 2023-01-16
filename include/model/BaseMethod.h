//
// Created by lijiang on 2021/12/17.
//

#ifndef APP_COMMUNICATION_BASEMETHOD_H
#define APP_COMMUNICATION_BASEMETHOD_H

#include "string"

#include "nlohmann/json.hpp"

using json = nlohmann::json;

template<class T>
class BaseMethod {
private:
    std::string method;
    int id;
    T params;

public:

    BaseMethod() {}

    BaseMethod(const std::string &method, int id) : method(method), id(id) {}

    BaseMethod(const std::string &method, int id, T params) : method(method), id(id), params(params) {}

    virtual ~BaseMethod() {
    }

    const std::string &getMethod() const {
        return method;
    }

    void setMethod(const std::string &method) {
        BaseMethod::method = method;
    }

    int getId() const {
        return id;
    }

    void setId(int id) {
        BaseMethod::id = id;
    }

    T getParams() const {
        return params;
    }

    void setParams(T params) {
        BaseMethod::params = params;
    }

    friend void to_json(json &j, const BaseMethod<T> &command) {
        j = json{
                {"method", command.method},
                {"id",     command.id},
                {"params", command.params}
        };
    }

    friend void from_json(const json &j, BaseMethod<T> &command) {
        j.at("method").get_to(command.method);
        j.at("id").get_to(command.id);
        j.at("params").get_to(command.params);
    }
};


#endif //APP_COMMUNICATION_BASEMETHOD_H
