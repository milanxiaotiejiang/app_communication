//
// Created by mi on 2022/4/20.
//

#ifndef APP_COMMUNICATION_MESSAGEBUS_H
#define APP_COMMUNICATION_MESSAGEBUS_H

#include <string>
#include <functional>
#include <map>
#include "Any.h"
#include "function_traits.h"
#include "NonCopyable.h"

class MessageBus : NonCopyable {
public:
    //register Message
    template<typename F>
    void attach(F &&f, const std::string &strTopic = "") {
        auto func = to_function(std::forward<F>(f));
        add(strTopic, std::move(func));
    }

    //send message
    template<typename R>
    void sendReq(const std::string &strTopic = "") {
        using function_type = std::function<R()>;
        std::string strMsgType = strTopic + typeid(function_type).name();
        auto range = m_map.equal_range(strMsgType);  //range  std::pair<iterator, iterator>
        //std::pair<std::multimap<string, Any>::iterator, std::multimap<string, Any>::iterator> range = m_map.equal_range(strMsgType);
        for (Iterater it = range.first; it != range.second; ++it) {
            auto f = it->second.AnyCast<function_type>();
            f();
        }
    }

    template<typename R, typename... Args>
    void sendReq(Args &&... args, const std::string &strTopic = "") {
        using function_type = std::function<R(Args...)>;
        std::string strMsgType = strTopic + typeid(function_type).name();
        auto range = m_map.equal_range(strMsgType);
        for (Iterater it = range.first; it != range.second; ++it) {
            auto f = it->second.AnyCast<function_type>();
            f(std::forward<Args>(args)...);
        }
    }

    template<typename R, typename... Args>
    void remove(const std::string &strTopic = "") {
        using function_type = std::function<R(Args...)>;

        std::string strMsgType = strTopic + typeid(function_type).name();
        int count = m_map.count(strMsgType);
        auto range = m_map.equal_range(strMsgType);
        m_map.erase(range.first, range.second);
    }

private:
    template<typename F>
    void add(const std::string &strTopic, F &&f) {
        std::string strMsgType = strTopic + typeid(F).name();
        m_map.emplace(std::move(strMsgType), std::forward<F>(f));
    }

private:
    std::multimap<std::string, Any> m_map;
    typedef std::multimap<std::string, Any>::iterator Iterater;

};

#endif //APP_COMMUNICATION_MESSAGEBUS_H
