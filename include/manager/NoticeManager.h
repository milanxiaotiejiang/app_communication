//
// Created by lijiang on 2021/12/16.
//

#ifndef C_PTHREAD_NOTICEMANAGER_H
#define C_PTHREAD_NOTICEMANAGER_H

#include "string"
#include "model/Notice.h"
#include "alignment/rrmap2.h"
#include "sub/json/MessageStrategy.h"

class NoticeManager {
private:
    NoticeManager() = default;

    NoticeManager(NoticeManager &) = delete;

    NoticeManager &operator=(const NoticeManager &) = delete;

public:
    ~NoticeManager() = default;

private:

    int planner_type = 0;

public:

    static auto &instance() {
        static NoticeManager obj;
        return obj;
    }

    int getPlannerType() const;

    void setPlannerType(int plannerType);

    void sendNotice(int noticeCode, long noticeTime, const std::string &noticeTitle,
                    const std::string &noticeMessage, const std::string &solution);

    void sendNotice(const int source, const MMapType mapType);
};


#endif //C_PTHREAD_NOTICEMANAGER_H
