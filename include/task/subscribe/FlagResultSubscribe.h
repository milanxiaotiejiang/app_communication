//
// Created by admin1 on 22-11-25.
//

#ifndef APP_COMMUNICATION_FLAGRESULTSUBSCRIBE_H
#define APP_COMMUNICATION_FLAGRESULTSUBSCRIBE_H

#include <ros/ros.h>
#include <std_msgs/Int32.h>

class FlagOutSubscribe {
private:
    ros::Subscriber sub_flag_out_;

    void subscribeCallback(const std_msgs::Int32 &flag_result);

public:
    FlagOutSubscribe(ros::NodeHandle handle);
};


class FlagInSubscribe {
private:
    ros::Subscriber sub_flag_in_;

    void subscribeCallback(const std_msgs::Int32 &flag_result);

public:
    FlagInSubscribe(ros::NodeHandle handle);
};


#endif //APP_COMMUNICATION_FLAGRESULTSUBSCRIBE_H
