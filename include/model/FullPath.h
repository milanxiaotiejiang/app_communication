//
// Created by lijiang on 2022/4/1.
//

#ifndef APP_COMMUNICATION_FULLPATH_H
#define APP_COMMUNICATION_FULLPATH_H

#include "vector"
#include "Point.h"

class FullPath {
private:
    std::vector<Point> full_path;
public:
    FullPath() {}

    FullPath(const std::vector<Point> &fullPath) : full_path(fullPath) {}

    virtual ~FullPath() {

    }

    const std::vector<Point> &getFullPath() const {
        return full_path;
    }

    void setFullPath(const std::vector<Point> &fullPath) {
        full_path = fullPath;
    }

    friend void to_json(json &j, const FullPath &b) {
        j = json{
                {"full_path", b.full_path}};
    }

    friend void from_json(const json &j, FullPath &b) {
        j.at("full_path").get_to(b.full_path);
    }
};


#endif //APP_COMMUNICATION_FULLPATH_H
