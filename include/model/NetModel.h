//
// Created by mi on 2022/4/14.
//

#ifndef APP_COMMUNICATION_NETMODEL_H
#define APP_COMMUNICATION_NETMODEL_H

struct NetModel {
    int key;
    std::string value;

    NetModel() {}

    NetModel(int key, const std::string &value) : key(key), value(value) {}
};

#endif //APP_COMMUNICATION_NETMODEL_H
