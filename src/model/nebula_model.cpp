//
// Created by io on 23-10-12.
//

#include "model/nebula_model.h"

CloudToken::CloudToken() {}

CloudToken::CloudToken(const std::string &token) : token(token) {}

const std::string &CloudToken::getToken() const {
    return token;
}

void CloudToken::setToken(const std::string &token) {
    CloudToken::token = token;
}
