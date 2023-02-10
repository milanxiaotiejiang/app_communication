//#ifndef APP_COMMUNICATION_ERROR_CORE_H
//#define APP_COMMUNICATION_ERROR_CORE_H
//
//#include <iostream>
//#include <fstream>
//#include <sstream>
//#include <time.h>
//#include <ctime>
//#include <nlohmann/json.hpp>
//#include <unistd.h>
//#include <pub/PubOut.h>
//#include <pub/PubInner.h>
//#include <std_msgs/String.h>
//#include "yaml-cpp/yaml.h"
//
//using namespace std;
//
//class Error_Core {
//private:
//    ros::NodeHandle handle;
//    PubInner pubInner;
//    PubOut pubOut;
//    ros::Subscriber error_status_;
//    string currFilepath;
//    YAML::Node m_config;
//
//    void subscribeErrCallback(const std_msgs::String &errmsg);
//
//
//public:
////Error_log(ros::NodeHandle handle,PubInner pubInner, PubOut pubOut) ;
//    Error_Core(ros::NodeHandle handle, PubInner pubInner, PubOut pubOut);
//
//
//    virtual ~Error_Core();
//
//
//    void WriteCoreLog(string msg);
//
//
//    void InitCoreLog();
//
//
//private:
//    string sFilepath;
//};
//
//#endif // APP_COMMUNICATION_ERROR_LOG_H
