//
// Created by Paul Wang on 2022/9/26.
//

#ifndef APP_COMMUNICATION_RESPONSECLOUD_H
#define APP_COMMUNICATION_RESPONSECLOUD_H

#include <ostream>
#include "string"
#include "nlohmann/json.hpp"

using json = nlohmann::json;
using namespace std;

class ResponseCloud {
private:
    int code;
    string message;
    std::string result;
    int id;
    

public:
    ResponseCloud();

    ResponseCloud(int code,string message,std::string result,int id);

    int getCode() const;

    void setCode(int noticeCode);

    string getMessage() const;

    void setMessage(string ss);

    const string &getResult() const;

    void setResult(const string &re);

    const string &getId() const;

    void setId(const int &i);

    
    friend void to_json(json &j, const ResponseCloud &b) {
        j = json{
                {"code",    b.code},
                {"message",    b.message},
                {"result",   b.result},
                {"id", b.id}
        };
    }

    friend void from_json(const json &j, ResponseCloud &b) {
        j.at("code").get_to(b.code);
        j.at("message").get_to(b.message);
        j.at("result").get_to(b.result);
        j.at("id").get_to(b.id);
        
    }
};

#endif //APP_COMMUNICATION_RESPONSECLOUD_H
