//
// Created by zt on 2022/3/8.
//

#include "model/RegisterOperation.h"

#include <utility>


RegisterOperation::RegisterOperation() = default;

RegisterOperation::RegisterOperation(string robotId) : robotId(std::move(robotId)) {}

RegisterOperation::~RegisterOperation() = default;

const string &RegisterOperation::getRobotId() const {
    return robotId;
}

void RegisterOperation::setRobotId(const string &robotId) {
    RegisterOperation::robotId = robotId;
}

ostream &operator<<(ostream &os, const RegisterOperation &peration) {
    os << "robotId: " << peration.robotId;
    return os;
}
