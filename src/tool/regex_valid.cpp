//
// Created by Looper on 2023/4/3.
//

#include "tool/regex_valid.h"
#include "schedule/croncpp.h"
#include "iostream"
#include "schedule/schedule_manager.h"

bool is_utf8(const std::string &string) {
    int bytes_in_char = 0;
    unsigned char byte;
    for (auto &chr: string) {
        byte = static_cast<unsigned char>(chr);
        if (bytes_in_char == 0) {
            if (byte >> 7 == 0) {
                continue;
            } else if (byte >> 5 == 0b110) {
                bytes_in_char = 1;
            } else if (byte >> 4 == 0b1110) {
                bytes_in_char = 2;
            } else if (byte >> 3 == 0b11110) {
                bytes_in_char = 3;
            } else {
                return false;
            }
        } else {
            if (byte >> 6 != 0b10) {
                return false;
            }
            --bytes_in_char;
        }
    }
    return bytes_in_char == 0;
}

bool is_valid_name(const std::string &str) {
//    std::regex pattern(R"([ )"); // 匹配3-10个中文、英文字母、数字、下划线
//    return std::regex_match(str, pattern);
//    std::regex pattern(R"([\w\xE4\xB8\x80-\xE9\xBE\xA5]{3,20})"); // 匹配3-10个中文、英文字母、数字、下划线
    std::regex pattern("^[\u4e00-\u9fa5_a-zA-Z0-9]{3,30}$", std::regex_constants::icase);
    return std::regex_match(str, pattern);
}

bool is_valid_crontab(const std::string &expression) {
    try {
        const std::string &cronExpression = ScheduleManager::fix_cron_expression("0 " + expression);
        cron::cronexpr cron_expression = cron::make_cron(cronExpression);
        return true;
    } catch (const cron::bad_cronexpr &ex) {
        std::cerr << "Invalid cron expression: " << ex.what() << std::endl;
        return false;
    }
}

//TEST_CASE() {
//    std::cout << "name " << is_valid_name("sdfgdfg") << std::endl; // 输出 true
//    std::cout << "name " << is_valid_name("sdfgDFg") << std::endl; // 输出 true
//    std::cout << "name " << is_valid_name("划区任务0001") << std::endl; // 输出 true
//    std::cout << "name " << is_valid_name("hello world") << std::endl; // 输出 false
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