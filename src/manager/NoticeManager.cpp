//
// Created by lijiang on 2021/12/16.
//

#include <iostream>
#include "manager/NoticeManager.h"
#include "manager/PublishOutManager.h"

void NoticeManager::sendNotice(int noticeCode, long noticeTime, const std::string &noticeTitle,
                               const std::string &noticeMessage, const std::string &solution) {
    auto notice = Notice(noticeCode, noticeTime, noticeTitle, noticeMessage, solution);
    PublishOutManager::instance().publishNotice(notice);
}

int NoticeManager::getPlannerType() const {
    return planner_type;
}

void NoticeManager::setPlannerType(int plannerType) {
    planner_type = plannerType;
}