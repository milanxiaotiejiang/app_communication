//
// Created by Looper on 2023/4/3.
//

#include "tool/regex_valid.h"
#include "schedule/croncpp.h"
#include "iostream"

bool is_valid_name(const std::string &str) {
//    std::regex pattern(R"([ )"); // 匹配3-10个中文、英文字母、数字、下划线
//    return std::regex_match(str, pattern);
    std::regex pattern(R"([\w\xE4\xB8\x80-\xE9\xBE\xA5]{3,10})"); // 匹配3-10个中文、英文字母、数字、下划线
    return std::regex_match(str, pattern);
}

bool is_valid_crontab(const std::string &expression) {
    try {
        cron::cronexpr cron_expression = cron::make_cron("0 " + expression);
        return true;
    } catch (const cron::bad_cronexpr &ex) {
        std::cerr << "Invalid cron expression: " << ex.what() << std::endl;
        return false;
    }
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