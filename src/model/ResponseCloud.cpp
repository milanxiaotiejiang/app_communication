//
// Created by lijiang on 2021/12/16.
//

#include "model/ResponseCloud.h"

    ResponseCloud::ResponseCloud(){}

    ResponseCloud::ResponseCloud(int code_,string message_,std::string result_,int id_):code(code_),
    message(message_),
    result(result_),
    id(id_){}

    int ResponseCloud::getCode() const{
        return code;
    }

    void ResponseCloud::setCode(int noticeCode){
        code = noticeCode;
    }

    string ResponseCloud::getMessage() const{
        return message;

    }

    void ResponseCloud::setMessage(string ss){
        message = ss;
    }

    const string &ResponseCloud::getResult() const{
        return result;
    }

    void ResponseCloud::setResult(const string &re){
        result = re;
    }

    const int &ResponseCloud::getId() const{
        return id;
    }

    void ResponseCloud::setId(const int &i){
        id = i;
    }