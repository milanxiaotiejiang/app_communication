#ifndef APP_COMMUNICATION_ERROR_LOG_H
#define APP_COMMUNICATION_ERROR_LOG_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <time.h>
#include <ctime>
#include <nlohmann/json.hpp>
#include <unistd.h>
#include <pub/PubOut.h>
#include <pub/PubInner.h>
#include "yaml-cpp/yaml.h"
using namespace std;

class Error_log {
private:
    
    Error_log() = default;


    Error_log(Error_log &) = delete;

    Error_log &operator=(const Error_log &) = delete;

    static Error_log *m_instance_ptr;
    YAML::Node m_config;

public:
//Error_log(ros::NodeHandle handle,PubInner pubInner, PubOut pubOut) ;
    ~Error_log();

    static Error_log *get_instance() {
        if (m_instance_ptr == nullptr) {
            m_instance_ptr = new Error_log;

        }
        return m_instance_ptr;
    }

    void start() { InitLog(); }

    void WriteLog(int msg);

    void WriteToCloud(int ntCode,long noticeTime);

    void InitLog();


private:
    string sFilepath;
};

#endif // APP_COMMUNICATION_ERROR_LOG_H
