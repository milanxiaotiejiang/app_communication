//
// Created by lijiang on 2021/12/18.
//

#include "model/CoreError.h"

CoreError::CoreError(int errorCode, const std::string &errorMessage) : error_code(errorCode),
                                                                       error_message(errorMessage) {}

CoreError::CoreError() {}

CoreError::~CoreError() {

}

int CoreError::getErrorCode() const {
    return error_code;
}

void CoreError::setErrorCode(int errorCode) {
    error_code = errorCode;
}

const std::string &CoreError::getErrorMessage() const {
    return error_message;
}

void CoreError::setErrorMessage(const std::string &errorMessage) {
    error_message = errorMessage;
}

bool CoreError::operator==(const CoreError &rhs) const {
    return error_code == rhs.error_code &&
           error_message == rhs.error_message;
}

bool CoreError::operator!=(const CoreError &rhs) const {
    return !(rhs == *this);
}

bool CoreError::operator<(const CoreError &rhs) const {
    if (error_code < rhs.error_code)
        return true;
    if (rhs.error_code < error_code)
        return false;
    return error_message < rhs.error_message;
}

bool CoreError::operator>(const CoreError &rhs) const {
    return rhs < *this;
}

bool CoreError::operator<=(const CoreError &rhs) const {
    return !(rhs < *this);
}

bool CoreError::operator>=(const CoreError &rhs) const {
    return !(*this < rhs);
}

std::ostream &operator<<(std::ostream &os, const CoreError &error) {
    os << "error_code: " << error.error_code << " error_message: " << error.error_message;
    return os;
}
