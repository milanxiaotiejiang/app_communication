//
// Created by lijiang on 2021/12/16.
//

#ifndef C_PTHREAD_NOTICE_H
#define C_PTHREAD_NOTICE_H

#include <ostream>
#include "string"
#include "nlohmann/json.hpp"

using json = nlohmann::json;

class Notice {
private:
    int notice_code;
    long notice_time;
    std::string notice_title;
    std::string notice_message;
    std::string solution;

public:
    Notice();

    Notice(int noticeCode, long noticeTime, const std::string &noticeTitle, const std::string &noticeMessage,
           const std::string &solution);

    int getNoticeCode() const;

    void setNoticeCode(int noticeCode);

    long getNoticeTime() const;

    void setNoticeTime(long noticeTime);

    const std::string &getNoticeTitle() const;

    void setNoticeTitle(const std::string &noticeTitle);

    const std::string &getNoticeMessage() const;

    void setNoticeMessage(const std::string &noticeMessage);

    const std::string &getSolution() const;

    void setSolution(const std::string &solution);

    friend std::ostream &operator<<(std::ostream &os, const Notice &notice);

    friend void to_json(json &j, const Notice &b) {
        j = json{
                {"notice_code",    b.notice_code},
                {"notice_time",    b.notice_time},
                {"notice_title",   b.notice_title},
                {"notice_message", b.notice_message},
                {"solution",       b.solution}
        };
    }

    friend void from_json(const json &j, Notice &b) {
        j.at("notice_code").get_to(b.notice_code);
        j.at("notice_time").get_to(b.notice_time);
        j.at("notice_title").get_to(b.notice_title);
        j.at("notice_message").get_to(b.notice_message);
        j.at("solution").get_to(b.solution);
    }
};

#endif //C_PTHREAD_NOTICE_H
