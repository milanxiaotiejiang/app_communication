//
// Created by mi on 2022/8/10.
//

#include "sub/json/NoticeStrategy.h"
#include "db/path.h"

vector<Notice> NoticeListStrategy::handler(string params) {
    std::string noticeFilePath;
    noticeFilePath.append(path::data_base_config_path());
    noticeFilePath.append("notice.txt");

    std::string content;
    std::unique_ptr<sh::File> noticeFilePtr(new sh::File(noticeFilePath));
    if (noticeFilePtr->open(std::ios::in)) {
        content = noticeFilePtr->readAll();
        noticeFilePtr->close();
    }
    vector<Notice> list;
    if (!content.empty()) {
        auto jdecode = json::parse(content);
        list = jdecode.get<vector<Notice>>();
    }
    return list;
}
