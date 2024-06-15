
#ifndef APP_COMMUNICATION_ZOOROBOTSTATUSSUBSCRIBE_H
#define APP_COMMUNICATION_ZOOROBOTSTATUSSUBSCRIBE_H

#include <ros/ros.h>
#include <model/WorkStatus.h>
#include <std_msgs/Int32.h>
#include "std_msgs/String.h"

class ZooRobotStatusSubscribe {
private:
    ros::NodeHandle handle;

    ros::Subscriber sub_motor_error_, sub_laser_error_, sub_hls_error_, sub_wet_mop_error;

    int last_machine_code_{10006};

    //电机堵转
    void motorErrorCallback(const std_msgs::Int32 &motor_error);

    void wetMopErrorCallback(const std_msgs::Int32 &motor_error);

    //雷达故障
    void laserErrorCallback(const std_msgs::String &laser_error);

    //电机堵转
    void hlsErrorCallback(const std_msgs::Int32 &motor_error);

public:
    ZooRobotStatusSubscribe(ros::NodeHandle handle);

    virtual ~ZooRobotStatusSubscribe();

};


#endif //APP_COMMUNICATION_ZOOROBOTSTATUSSUBSCRIBE_H
