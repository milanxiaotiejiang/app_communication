//
// Created by io on 23-12-11.
//

#ifndef APP_COMMUNICATION_ELEVATOR_CONTROL_H
#define APP_COMMUNICATION_ELEVATOR_CONTROL_H

#include <ros/ros.h>

#include <nav_msgs/Odometry.h>
#include <sensor_msgs/Imu.h>
#include <geometry_msgs/Twist.h>
#include <tf/transform_datatypes.h>

#include <mutex>
#include <condition_variable>
#include <thread>
#include <std_msgs/Int32.h>
#include "future/thread_pool.h"

class ElevatorControlManager {
private:
    ElevatorControlManager() = default;

    ElevatorControlManager(ElevatorControlManager &) = delete;

    ElevatorControlManager &operator=(const ElevatorControlManager &) = delete;

public:
    ~ElevatorControlManager() = default;

    static auto &instance() {
        static ElevatorControlManager obj;
        return obj;
    }

private:
    enum ControlCommand {
        ENTER_ELEVATOR,
        EXIT_ELEVATOR
    };

    enum ControlCmd {
        NONE,
        MOVE,
        ROTATE,
        REACH
    };


    ros::Subscriber subscriberOdom;
    ros::Subscriber subscriberImu;
    ros::Publisher publisherCmdVel;

    ros::Subscriber subscriberElevatorManager;

    std::thread elevator_control_planner_thread;

    double odom_x;
//    double odom_yaw;
    double imu_yaw;

    std::atomic<double> old_x;
    std::atomic<double> old_yaw;

    double last_angle;

    std::atomic<bool> mainInterrupt;
    std::atomic<ControlCmd> controlCmd;

    async::ThreadPool pool_;

    void subscribeOdomCallback(const nav_msgs::Odometry &msg);

    void subscribeImuCallback(const sensor_msgs::Imu &msg);

    void elevatorManagerSubscribeCallback(const std_msgs::Int32 &flag);

    void elevator_control_planner_thread_func();

    void movement_controls_func(ControlCommand command);

public:

    void initialize(ros::NodeHandle handle);

    void enterElevator();

    void exitElevator();

    void stop();

    void record();

    void publishCmd(double x = 0, double z = 0) const;

    void printElevator();
};


#endif //APP_COMMUNICATION_ELEVATOR_CONTROL_H
