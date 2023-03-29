//
// Created by lijiang on 2021/12/17.
//

#ifndef APP_COMMUNICATION_BASERESULT_H
#define APP_COMMUNICATION_BASERESULT_H

#include "string"

#include "nlohmann/json.hpp"

using json = nlohmann::json;

template<class T>
class BaseResult {

private:
    int id;
    bool result;
    int error_code;
    std::string error_message;
    T params;

public:

    BaseResult(int id) : id(id) {
        error_code = 0;
        result = true;
    }

    BaseResult(int id, T params) : id(id), params(params) {
        error_code = 0;
        result = true;
    }

    BaseResult(int id, int errorCode, const std::string &errorMessage) : id(id), error_code(errorCode),
                                                                         error_message(errorMessage) {
        result = false;
        params = "";
    }

    BaseResult(int id, bool result, int errorCode, const std::string &errorMessage, T params)
            : id(id), result(result), error_code(errorCode), error_message(errorMessage), params(params) {}

    ~BaseResult() {};

    friend void to_json(json &j, const BaseResult<T> &result) {
        j = json{
                {"result",        result.result},
                {"id",            result.id},
                {"error_code",    result.error_code},
                {"error_message", result.error_message},
                {"params",        result.params}
        };
    }

    friend void from_json(const json &j, BaseResult<T> &result) {
        j.at("result").get_to(result.result);
        j.at("id").get_to(result.id);
        j.at("error_code").get_to(result.error_code);
        j.at("error_message").get_to(result.error_message);
        j.at("params").get_to(result.params);
    }
};


#endif //APP_COMMUNICATION_BASERESULT_H
