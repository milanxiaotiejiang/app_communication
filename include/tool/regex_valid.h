//
// Created by Looper on 2023/3/20.
//

#ifndef APP_COMMUNICATION_REGEX_VALID_H
#define APP_COMMUNICATION_REGEX_VALID_H

#include <string>
#include <regex>
#include "schedule/Cron.h"

#include <catch2/catch.hpp>

bool is_valid_name(const std::string &str) {
//    std::regex pattern(R"([ )"); // 匹配3-10个中文、英文字母、数字、下划线
//    return std::regex_match(str, pattern);
    std::regex pattern(R"([\w\xE4\xB8\x80-\xE9\xBE\xA5]{3,10})"); // 匹配3-10个中文、英文字母、数字、下划线
    return std::regex_match(str, pattern);
}

bool is_valid_crontab(const std::string &expression) {
    croncpp::CronData cron_parser;
    cron_parser.parse("0 " + expression);
    return cron_parser.is_valid();
}

//TEST_CASE() {
//    std::cout << "name " << is_valid_name("") << std::endl; // 输出 true
//    std::cout << "name " << is_valid_name("你好") << std::endl; // 输出 true
//    std::cout << "name " << is_valid_name("hello world") << std::endl; // 输出 true
//    std::cout << "name " << is_valid_name("任务1") << std::endl; // 输出 true
//    std::cout << "name " << is_valid_name("任务A") << std::endl; // 输出 true
//    std::cout << "name " << is_valid_name("任务b") << std::endl; // 输出 true
//}
//TEST_CASE() {
//    std::string expression = "0 */5 * * * *";
//    std::cout << is_valid_crontab(expression) << std::endl; // 输出 true
//
//    expression = "0 */5 * * *";
//    std::cout << is_valid_crontab(expression) << std::endl; // 输出 false
//
//    expression = "0 5 * * * * *";
//    std::cout << is_valid_crontab(expression) << std::endl; // 输出 false
//}

#endif //APP_COMMUNICATION_REGEX_VALID_H
