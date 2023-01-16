//
// Created by lijiang on 2022/5/26.
//

#include "model/Project.h"

const string &Project::getName() const {
    return name;
}

void Project::setName(const string &name) {
    Project::name = name;
}
