//
// Created by Looper on 2023/3/20.
//

#ifndef APP_COMMUNICATION_REGEX_VALID_H
#define APP_COMMUNICATION_REGEX_VALID_H

#include <string>
#include <regex>
#include <catch2/catch.hpp>

int gbk_strlen(const char *str);

bool is_valid_name(const std::string &str);

bool is_valid_crontab(const std::string &expression);

#endif //APP_COMMUNICATION_REGEX_VALID_H
