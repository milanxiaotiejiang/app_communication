//
// Created by lijiang on 2022/3/9.
//

#ifndef APP_COMMUNICATION_REQUESTDATA_H
#define APP_COMMUNICATION_REQUESTDATA_H

#include "string"
#include "BaseMethod.h"

class RequestData {
public:
    std::string data;

    RequestData(const std::string &data) : data(data) {}

    RequestData() {}

    const std::string &getData() const {
        return data;
    }

    void setData(const std::string &data) {
        RequestData::data = data;
    }

    friend void to_json(json &j, const RequestData &model) {
        j = json{
                {"data", model.data}
        };
    }

    friend void from_json(const json &j, RequestData &requestModel) {
        j.at("data").get_to(requestModel.data);
    }
};


#endif //APP_COMMUNICATION_REQUESTDATA_H
