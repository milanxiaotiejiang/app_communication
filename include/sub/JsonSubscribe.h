//
// Created by lijiang on 2021/12/18.
//

#ifndef APP_COMMUNICATION_JSONSUBSCRIBE_H
#define APP_COMMUNICATION_JSONSUBSCRIBE_H

#include<ros/ros.h>
#include<std_msgs/String.h>
#include <model/Entrance.h>
#include <sub/json/MessageStrategy.h>
#include <tool/Switch.h>
#include <sub/json/MessageContext.h>
#include "boost/thread.hpp"

#include <nlohmann/json.hpp>
#include <pub/PubOut.h>
#include <pub/PubInner.h>

using json = nlohmann::json;

class JsonSubscribe {

private:
    ros::NodeHandle handle;
    ros::Subscriber sub_json_;

    void subscribeCallback(const std_msgs::String &result);

public:
    JsonSubscribe(ros::NodeHandle handle);

    virtual ~JsonSubscribe();

};


#endif //APP_COMMUNICATION_JSONSUBSCRIBE_H
