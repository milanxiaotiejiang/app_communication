/*
 * @Author: siyu.zhu
 * @Date: 2022-05-18 05:45:22
 * @Last Modified by: siyu.zhu
 * @Last Modified time: 2022-05-18 06:04:29
 */

#ifndef APP_COMMUNICATION_JsonSubscribeV3_H
#define APP_COMMUNICATION_JsonSubscribeV3_H

#include "boost/thread.hpp"
#include <model/Entrance.h>
#include <ros/ros.h>
#include <std_msgs/String.h>
#include <sub/json/MessageContext.h>
#include <sub/json/MessageStrategy.h>
#include <tool/Switch.h>

#include <nlohmann/json.hpp>
#include <pub/PubInner.h>
#include <pub/PubOut.h>

#include "clean_msgs/cloud_control.h"
#include "clean_msgs/robot_control.h"



using json = nlohmann::json;
using namespace std;
using namespace clean_msgs;

class JsonSubscribeCloud {

private:
    ros::NodeHandle handle;
    PubInner pubInner;
    PubOut pubOut;
    ros::Subscriber sub_json_;
    ros::ServiceServer service;


    std::string m_request;
    std::string m_response;

public:
    JsonSubscribeCloud(ros::NodeHandle handle, PubInner pubInner, PubOut pubOut);

    virtual ~JsonSubscribeCloud();

    // std::string request_;
    // std::string response_;

     bool function(robot_control::Request &req, robot_control::Response &res);

    // void set_request(std::string request_);

    // string get_request();

    // void set_response(std::string response_);

    // string get_response();

   // void subscribeCallback(const std_msgs::String &result);
};


#endif//APP_COMMUNICATION_JsonSubscribeV3_H
