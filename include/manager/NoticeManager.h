//
// Created by lijiang on 2021/12/16.
//

#ifndef C_PTHREAD_NOTICEMANAGER_H
#define C_PTHREAD_NOTICEMANAGER_H

#include <pub/PubOut.h>
#include "string"
#include "model/Notice.h"
#include "pub/PubOut.h"

class NoticeManager {

    NoticeManager() = default;

    NoticeManager(NoticeManager &) = delete;

    NoticeManager &operator=(const NoticeManager &) = delete;


private:

    PubOut *pubOut;
    static NoticeManager *m_instance_ptr;
    int planner_type = 0;

public:
    ~NoticeManager() = default;

    static NoticeManager *get_instance() {
        if (m_instance_ptr == nullptr) {
            m_instance_ptr = new NoticeManager;
        }
        return m_instance_ptr;
    }

    int getPlannerType() const;

    void setPlannerType(int plannerType);

    void sendNotice(int noticeCode, long noticeTime, const string &noticeTitle,
                    const string &noticeMessage, const string &solution);

    PubOut *getPubOut() const;

    void setPubOut(PubOut *pubOut);
};


#endif //C_PTHREAD_NOTICEMANAGER_H
