//
// Created by lijiang on 2021/12/20.
//

#ifndef APP_COMMUNICATION_BEFOREJSONSUBSCRIBE_H
#define APP_COMMUNICATION_BEFOREJSONSUBSCRIBE_H

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
#include "tool/OldFunction.hpp"

using json = nlohmann::json;

class BeforeJsonSubscribe {
private:
    ros::NodeHandle handle;
    PubInner pubInner;
    PubOut pubOut;
    ros::Subscriber sub_json_;

    void subscribeCallback(const std_msgs::String &result);

public:
    BeforeJsonSubscribe(ros::NodeHandle handle, PubInner pubInner, PubOut pubOut);

    virtual ~BeforeJsonSubscribe();

};


#endif //APP_COMMUNICATION_BEFOREJSONSUBSCRIBE_H
