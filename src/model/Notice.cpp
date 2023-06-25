//
// Created by lijiang on 2021/12/16.
//

#include "model/Notice.h"

Notice::Notice(int noticeCode, long noticeTime, const std::string &noticeTitle, const std::string &noticeMessage,
               const std::string &solution) : notice_code(noticeCode), notice_time(noticeTime), notice_title(noticeTitle),
                                         notice_message(noticeMessage), solution(solution) {}

Notice::Notice() {}

int Notice::getNoticeCode() const {
    return notice_code;
}

void Notice::setNoticeCode(int noticeCode) {
    notice_code = noticeCode;
}

long Notice::getNoticeTime() const {
    return notice_time;
}

void Notice::setNoticeTime(long noticeTime) {
    notice_time = noticeTime;
}

const std::string &Notice::getNoticeTitle() const {
    return notice_title;
}

void Notice::setNoticeTitle(const std::string &noticeTitle) {
    notice_title = noticeTitle;
}

const std::string &Notice::getNoticeMessage() const {
    return notice_message;
}

void Notice::setNoticeMessage(const std::string &noticeMessage) {
    notice_message = noticeMessage;
}

const std::string &Notice::getSolution() const {
    return solution;
}

void Notice::setSolution(const std::string &solution) {
    Notice::solution = solution;
}

std::ostream &operator<<(std::ostream &os, const Notice &notice) {
    os << "notice_code: " << notice.notice_code << " notice_time: " << notice.notice_time << " notice_title: "
       << notice.notice_title << " notice_message: " << notice.notice_message << " solution: " << notice.solution;
    return os;
}
