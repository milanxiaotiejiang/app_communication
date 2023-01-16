//
// Created by mi on 2022/4/20.
//

#ifndef APP_COMMUNICATION_NONCOPYABLE_H
#define APP_COMMUNICATION_NONCOPYABLE_H


class NonCopyable {
protected:
    NonCopyable() = default;

    ~NonCopyable() = default;

    NonCopyable(const NonCopyable &) = delete;

    NonCopyable &operator=(const NonCopyable &) = delete;
};


#endif //APP_COMMUNICATION_NONCOPYABLE_H
