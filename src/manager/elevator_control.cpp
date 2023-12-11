//
// Created by io on 23-12-11.
//

#include <Eigen/Geometry>
#include <cmath>
#include "manager/elevator_control.h"
#include "glog/logging.h"
#include "simulation.h"
#include "catch2/catch.hpp"
#include "task/manager/NodeWorkModeManager.h"
#include "BaseThrowable.h"

#define SLEEP_TIME 10
#define MOVING_DISTANCE 1.6
#define INEXPLICABLE_MAGIC_NUMBER 0.00456789

void ElevatorControlManager::initialize(ros::NodeHandle handle) {
    stop();
    pool_.setNumOfThreads(2);

    subscriberOdom = handle.subscribe("/odom", 10, &ElevatorControlManager::subscribeOdomCallback, this);
    subscriberImu = handle.subscribe("/imu", 10, &ElevatorControlManager::subscribeImuCallback, this);
    publisherCmdVel = handle.advertise<geometry_msgs::Twist>("/cmd_vel", 1);

    elevator_control_planner_thread = std::thread(&ElevatorControlManager::elevator_control_planner_thread_func, this);
    elevator_control_planner_thread.detach();

    subscriberElevatorManager = handle.subscribe("/elevator_manager", 1,
                                                 &ElevatorControlManager::elevatorManagerSubscribeCallback,
                                                 this);

}

void ElevatorControlManager::subscribeOdomCallback(const nav_msgs::Odometry &odometry) {
    odom_x = odometry.pose.pose.position.x;
//    odom_yaw = tf::getYaw(odometry.pose.pose.orientation);
}

void ElevatorControlManager::subscribeImuCallback(const sensor_msgs::Imu &imu) {
    tf::Quaternion q(imu.orientation.x, imu.orientation.y, imu.orientation.z, imu.orientation.w);
    tf::Matrix3x3 m(q);
    double roll, pitch, yaw;
    m.getRPY(roll, pitch, yaw);
    imu_yaw = yaw;

//    LOG_IF(INFO, DEBUG_ELEVATOR) << "ElevatorControlManager imu_yaw ： " << (imu_yaw * 180.0 / M_PI)
//                                 << " , roll : " << roll
//                                 << " , pitch : " << pitch;
}

void ElevatorControlManager::elevatorManagerSubscribeCallback(const std_msgs::Int32 &flag) {
    try {
        if (flag.data == 0) {
            exitElevator();
        } else if (flag.data == 1) {
            enterElevator();
        } else if (flag.data == 100) {
            if (!NodeWorkModeManager::instance().enterWorkMode(2)) {
                throw app::exception(make_error_code(error::mode_switching_is_not_supported));
            }
        }
    } catch (app::exception const &e) {
        LOG(ERROR) << e.what();
    } catch (const std::exception &e) {
        LOG(ERROR) << e.what();
    } catch (...) {
        LOG(ERROR) << "MessageStrategy other start exception";
    }
}

void ElevatorControlManager::elevator_control_planner_thread_func() {

}

//void ElevatorControlManager::rotate180() {
//    ros::Rate rate(10); // 10Hz
//    double initial_yaw = imu_yaw;
//    double current_yaw = imu_yaw;
//    while (std::abs(current_yaw - initial_yaw) < M_PI) {
//        geometry_msgs::Twist move_cmd;
//        move_cmd.angular.z = 0.5; // Adjust this value to control the rotation speed
//        publisherCmdVel.publish(move_cmd);
//        current_yaw = imu_yaw; // Update current_yaw with the latest imu_yaw value
//        rate.sleep();
//    }
//    // Stop rotation
//    geometry_msgs::Twist move_cmd;
//    move_cmd.angular.z = 0.0;
//    publisherCmdVel.publish(move_cmd);
//}

void ElevatorControlManager::movement_controls_func(ControlCommand command) {
    stop();
    record();

    while (mainInterrupt) {

        int append_sleep_time = 0;

//        printElevator();

        switch (controlCmd) {
            case ControlCmd::NONE:
                publishCmd();
                controlCmd = ControlCmd::MOVE;
                break;
            case ControlCmd::MOVE: {
                double distance_x = std::abs(odom_x - old_x);
                if (distance_x < MOVING_DISTANCE - SLEEP_TIME * INEXPLICABLE_MAGIC_NUMBER) {// 0.0456789
                    publishCmd(0.2, 0);
                } else {
                    controlCmd = ControlCmd::ROTATE;
                }
                break;
            }
            case ControlCmd::ROTATE: {
                auto old_angle = old_yaw * 180.0 / M_PI;
                auto curr_angle = imu_yaw * 180.0 / M_PI;
                auto angle_difference = curr_angle - old_angle;
                if (angle_difference < 0) {
                    angle_difference += 360;
                }
                if (angle_difference > 180) {
                    angle_difference = 360 - angle_difference;
                }
                bool normal_rotate = true;
                if (controlCmd == ControlCmd::ROTATE && angle_difference > 90) {
                    normal_rotate = angle_difference - last_angle > 0;
                    if (!normal_rotate) {
                        angle_difference = 180;
                    }
                }
                if (angle_difference < 180 - SLEEP_TIME * INEXPLICABLE_MAGIC_NUMBER) {// 0.0456789
                    publishCmd(0, 0.2);
                } else {
                    LOG_IF(INFO, DEBUG_ELEVATOR) << "ElevatorControlManager normal_rotate ： " << normal_rotate;
                    if (!normal_rotate) {
                        publishCmd(0, -0.2);
                    }
                    controlCmd = ControlCmd::REACH;
                }
                last_angle = angle_difference;
                break;
            }
            case ControlCmd::REACH: {
                publishCmd(0, 0);
                mainInterrupt = false;

                double distance_x = std::abs(odom_x - old_x);
                auto old_angle = old_yaw * 180.0 / M_PI;
                auto curr_angle = imu_yaw * 180.0 / M_PI;
                auto angle_difference = curr_angle - old_angle;
                if (angle_difference < 0) {
                    angle_difference += 360;
                }
                if (angle_difference > 180) {
                    angle_difference = 360 - angle_difference;
                }

                LOG_IF(INFO, DEBUG_ELEVATOR) << "ElevatorControlManager over ： " << !mainInterrupt
                                             << "， * ： " << (distance_x - MOVING_DISTANCE)
                                             << "， * ： " << (180 - angle_difference);
                break;
            }

        }

        std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_TIME + append_sleep_time));
    }

}

void ElevatorControlManager::printElevator() {
    double distance_x = std::abs(odom_x - old_x);
    double difference_yaw = imu_yaw - old_yaw;

    auto old_angle = old_yaw * 180.0 / M_PI;
    auto curr_angle = imu_yaw * 180.0 / M_PI;

    auto angle_difference = curr_angle - old_angle;
    if (angle_difference < 0) {
        angle_difference += 360;
    }
    if (angle_difference > 180) {
        angle_difference = 360 - angle_difference;
    }
    bool normal_rotate = true;
    if (controlCmd == ROTATE && angle_difference > 90) {
        normal_rotate = angle_difference - last_angle > 0;
        if (!normal_rotate) {
            angle_difference = 180;
        }
    }

    LOG_IF(INFO, DEBUG_ELEVATOR) << "distance_x ： " << distance_x
                                 << "， old_yaw ： " << old_yaw
                                 << "， imu_yaw ： " << imu_yaw
                                 << "， difference_yaw ： " << difference_yaw
                                 << "， old_angle ： " << old_angle
                                 << "， curr_angle ： " << curr_angle
                                 << "， angle_difference ： " << angle_difference;
}

void ElevatorControlManager::publishCmd(double x, double z) const {
    geometry_msgs::Twist move_cmd;
    move_cmd.linear.x = x;
    move_cmd.linear.y = 0.0;
    move_cmd.angular.z = z;
    publisherCmdVel.publish(move_cmd);
}

void ElevatorControlManager::enterElevator() {
    LOG_IF(INFO, DEBUG_ELEVATOR) << "ElevatorControlManager enterElevator ... ";
    pool_.execute([this] {
        movement_controls_func(ControlCommand::ENTER_ELEVATOR);
    });
}

void ElevatorControlManager::exitElevator() {
    LOG_IF(INFO, DEBUG_ELEVATOR) << "ElevatorControlManager exitElevator ... ";
    pool_.execute([this] {
        movement_controls_func(ControlCommand::EXIT_ELEVATOR);
    });
}

void ElevatorControlManager::stop() {
    mainInterrupt = true;
    controlCmd = ControlCmd::NONE;
}

void ElevatorControlManager::record() {
    old_x = odom_x;
    old_yaw = imu_yaw;
}
