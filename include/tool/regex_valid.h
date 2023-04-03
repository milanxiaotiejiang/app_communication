//
// Created by Looper on 2023/3/20.
//

#ifndef APP_COMMUNICATION_REGEX_VALID_H
#define APP_COMMUNICATION_REGEX_VALID_H

#include <string>
#include <regex>
#include "schedule/Cron.h"

#include <catch2/catch.hpp>

bool is_valid_name(const std::string &str);

bool is_valid_crontab(const std::string &expression);

#endif //APP_COMMUNICATION_REGEX_VALID_H
