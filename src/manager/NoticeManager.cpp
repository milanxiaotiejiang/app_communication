//
// Created by lijiang on 2021/12/16.
//

#include <iostream>
#include "glog/logging.h"
#include "manager/NoticeManager.h"

void NoticeManager::sendNotice(int noticeCode, long noticeTime, const string &noticeTitle,
                               const string &noticeMessage, const string &solution) {
    auto notice = Notice(noticeCode, noticeTime, noticeTitle, noticeMessage, solution);
    NoticeManager::pubOut->publishNotice(notice);
}

int NoticeManager::getPlannerType() const {
    return planner_type;
}

void NoticeManager::setPlannerType(int plannerType) {
    planner_type = plannerType;
}

PubOut *NoticeManager::getPubOut() const {
    return pubOut;
}

void NoticeManager::setPubOut(PubOut *pubOut) {
    NoticeManager::pubOut = pubOut;
}
