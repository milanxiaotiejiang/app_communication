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
#include "task/RealBlock.h"

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

    enum ElevatorPreState {
        PRE_NONE,
        PRE_CIRCULATION,
        PRE_ELEVATOR,
        PRE_SWITCH_MAP,
        PRE_OVER,
    };

    enum ElevatorPostState {
        POST_NONE,
        POST_CIRCULATION,
        POST_ELEVATOR,
        POST_SWITCH_MAP,
        POST_OVER,
    };

    ros::Subscriber subscriberOdom;
    ros::Subscriber subscriberImu;
    ros::Publisher publisherCmdVel;

    ros::Subscriber subscriberElevatorManager;


    async::ThreadPool pool_;

    std::thread elevator_pre_thread;
    std::thread elevator_post_thread;

    std::mutex pre_mutex_;
    std::condition_variable pre_condition_variable_;
    std::mutex post_mutex_;
    std::condition_variable post_condition_variable_;


    double odom_x;
//    double odom_yaw;
    double imu_yaw;

    std::atomic<double> old_x;
    std::atomic<double> old_yaw;

    double last_angle;

    std::atomic<bool> mainInterrupt;
    std::atomic<ControlCmd> controlCmd;

    std::atomic<ElevatorPreState> preState;
    std::atomic<ElevatorPostState> postState;

    RealBlock preCirculationBlock;
    RealBlock preElevatorBlock;
    RealBlock preSwitchMapBlock;
    RealBlock postCirculationBlock;
    RealBlock postElevatorBlock;
    RealBlock postSwitchMapBlock;

    std::function<void(bool)> callbackElevatorPre;
    std::function<void(bool)> callbackElevatorPost;

private:
    void publishCmd(double x = 0, double z = 0) const;

    void interruptAccessElevators();

    void recordSensorData();

    void subscribeOdomCallback(const nav_msgs::Odometry &msg);

    void subscribeImuCallback(const sensor_msgs::Imu &msg);

    void elevatorManagerSubscribeCallback(const std_msgs::Int32 &flag);

    void movement_controls_func(ControlCommand command);

    [[noreturn]] void elevator_pre_thread_func();

    [[noreturn]] void elevator_post_thread_func();

    void doPreCirculation();

    void doPreElevator();

    void doPreSwitchMap();

    void doPostCirculation();

    void doPostElevator();

    void doPostSwitchMap();

public:
    void initialize(ros::NodeHandle handle);

    void setCallbackElevatorPre(const std::function<void(bool)> &callbackElevatorPre);

    void setCallbackElevatorPost(const std::function<void(bool)> &callbackElevatorPost);

    void enterElevator();

    void exitElevator();

    void printElevator();

    void handlePreFlow(const std::vector<RealBlock> &preFlows);

    void handlePostFlow(const std::vector<RealBlock> &postFlows);

};


#endif //APP_COMMUNICATION_ELEVATOR_CONTROL_H
