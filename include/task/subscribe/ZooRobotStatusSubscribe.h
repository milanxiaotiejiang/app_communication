//
// Created by admin1 on 22-11-25.
//

#ifndef APP_COMMUNICATION_ZOOROBOTSTATUSSUBSCRIBE_H
#define APP_COMMUNICATION_ZOOROBOTSTATUSSUBSCRIBE_H

#include <ros/ros.h>
#include "zoo_bringup/robot_status.h"
#include <model/WorkStatus.h>
#include <std_msgs/Int32.h>
#include "std_msgs/String.h"

class ZooRobotStatusSubscribe {
private:
    ros::NodeHandle handle;

    ros::Subscriber sub_robot_status_;

    ros::Subscriber sub_motor_error_, sub_laser_error_, sub_hls_error_;

    int last_machine_code_{10006};

    void subscribeCallback(const zoo_bringup::robot_status &robot_status);

    //电机堵转
    void motorErrorCallback(const std_msgs::Int32ConstPtr &motor_error);

    //雷达故障
    void laserErrorCallback(const std_msgs::StringConstPtr &laser_error);

    //电机堵转
    void hlsErrorCallback(const std_msgs::Int32ConstPtr &motor_error);

public:
    ZooRobotStatusSubscribe(ros::NodeHandle handle);

    virtual ~ZooRobotStatusSubscribe();

    void pubKnob(const zoo_bringup::robot_status &robot_status) const;

    void pubMaterial() const;

};


#endif //APP_COMMUNICATION_ZOOROBOTSTATUSSUBSCRIBE_H
