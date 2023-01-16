//
// Created by zt on 2022/3/8.
//

#ifndef C_T_REGISTEROPERATION_H
#define C_T_REGISTEROPERATION_H

#include "iostream"
#include "string"

#include "nlohmann/json.hpp"

using json = nlohmann::json;

using namespace std;

class RegisterOperation {
private:
    string robotId;
public:
    RegisterOperation(string robotId);

    RegisterOperation();

    virtual ~RegisterOperation();

    const string &getRobotId() const;

    void setRobotId(const string &robotId);

    friend ostream &operator<<(ostream &os, const RegisterOperation &peration);

    friend void to_json(json &j, const RegisterOperation &operation) {
        j = json{
                {"robotId", operation.robotId}
        };
    }

    friend void from_json(const json &j, RegisterOperation &operation) {
        j.at("robotId").get_to(operation.robotId);
    }
};


#endif //C_T_REGISTEROPERATION_H
