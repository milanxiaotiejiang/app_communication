//
// Created by lijiang on 2022/5/26.
//

#include "model/Project.h"

const std::string &Project::getName() const {
    return name;
}

void Project::setName(const std::string &name) {
    Project::name = name;
}
