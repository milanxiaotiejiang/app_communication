//
// Created by lijiang on 2021/12/18.
//

#ifndef APP_COMMUNICATION_COREERROR_H
#define APP_COMMUNICATION_COREERROR_H

#include <ostream>
#include "string"

class CoreError {
private:
    int error_code;
    std::string error_message;

public:
    CoreError();

    CoreError(int errorCode, const std::string &errorMessage);

    virtual ~CoreError();

    int getErrorCode() const;

    void setErrorCode(int errorCode);

    const std::string &getErrorMessage() const;

    void setErrorMessage(const std::string &errorMessage);

    bool operator==(const CoreError &rhs) const;

    bool operator!=(const CoreError &rhs) const;

    bool operator<(const CoreError &rhs) const;

    bool operator>(const CoreError &rhs) const;

    bool operator<=(const CoreError &rhs) const;

    bool operator>=(const CoreError &rhs) const;

    friend std::ostream &operator<<(std::ostream &os, const CoreError &error);

};


#endif //APP_COMMUNICATION_COREERROR_H
