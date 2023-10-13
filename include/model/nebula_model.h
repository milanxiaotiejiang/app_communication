//
// Created by io on 23-10-12.
//

#ifndef APP_COMMUNICATION_NEBULA_MODEL_H
#define APP_COMMUNICATION_NEBULA_MODEL_H

#include "string"

#include "nlohmann/json.hpp"

using json = nlohmann::json;

template<class T>
class CloudModel {
private:
    int code;
    std::string message;
    bool success;
    long timestamp;
    T result;

public:

    CloudModel() {}

    virtual ~CloudModel() {}

    int getCode() const {
        return code;
    }

    const std::string &getMessage() const {
        return message;
    }

    bool isSuccess() const {
        return success;
    }

    long getTimestamp() const {
        return timestamp;
    }

    T getResult() const {
        return result;
    }

    friend void to_json(json &j, const CloudModel<T> &model) {
        j = json{
                {"code",      model.code},
                {"message",   model.message},
                {"success",   model.success},
                {"timestamp", model.timestamp},
                {"result",    model.result}
        };
    }

    friend void from_json(const json &j, CloudModel<T> &model) {
        j.at("code").get_to(model.code);
        j.at("message").get_to(model.message);
        j.at("success").get_to(model.success);
        j.at("timestamp").get_to(model.timestamp);
        try {
            j.at("result").get_to(model.result);
        } catch (...) {
            //
        }
    }
};

class CloudToken {
private:
    std::string token;

public:

    CloudToken();

    explicit CloudToken(const std::string &token);

    friend void to_json(json &j, const CloudToken &model) {
        j = json{
                {"token", model.token},
        };
    }

    friend void from_json(const json &j, CloudToken &model) {
        j.at("token").get_to(model.token);
    }

    const std::string &getToken() const;

    void setToken(const std::string &token);
};

#endif //APP_COMMUNICATION_NEBULA_MODEL_H
