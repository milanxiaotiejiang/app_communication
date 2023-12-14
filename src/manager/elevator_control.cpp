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
#define INEXPLICABLE_MAGIC_NUMBER 0.00556789


void ElevatorControlManager::publishCmd(double x, double z) const {
    geometry_msgs::Twist move_cmd;
    move_cmd.linear.x = x;
    move_cmd.linear.y = 0.0;
    move_cmd.angular.z = z;
    publisherCmdVel.publish(move_cmd);
}

void ElevatorControlManager::interruptAccessElevators() {
    mainInterrupt = true;
    controlCmd = ControlCmd::NONE;
}

void ElevatorControlManager::recordSensorData() {
    old_x = odom_x;
    old_yaw = imu_yaw;
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
    try {//elevator_manager
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
    interruptAccessElevators();
    recordSensorData();

    while (mainInterrupt) {

        int append_sleep_time = 0;

        printElevator();

        switch (controlCmd) {
            case ControlCmd::NONE:
                publishCmd();
                controlCmd = ControlCmd::MOVE;
                break;
            case ControlCmd::MOVE: {
                double distance_x = std::abs(odom_x - old_x);
                if (distance_x < MOVING_DISTANCE - SLEEP_TIME * INEXPLICABLE_MAGIC_NUMBER) {// 0.00556789
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
                    normal_rotate = angle_difference - last_angle > -(SLEEP_TIME * INEXPLICABLE_MAGIC_NUMBER);
                    if (!normal_rotate) {
                        angle_difference = 188;
                    }
                }
                if (angle_difference < 180 - 100 * SLEEP_TIME * INEXPLICABLE_MAGIC_NUMBER) {// 0.00556789
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

[[noreturn]] void ElevatorControlManager::elevator_pre_thread_func() {

    while (true) {

        std::unique_lock<std::mutex> lk(pre_mutex_);
        pre_condition_variable_.wait(lk, [this] {
            return preState != ElevatorPreState::PRE_NONE;
        });
        if (preState == ElevatorPreState::PRE_CIRCULATION) {
            LOG_IF(INFO, DEBUG_ELEVATOR) << "ElevatorControlManager PRE_CIRCULATION ... ";
            doPreCirculation();
        } else if (preState == ElevatorPreState::PRE_ELEVATOR) {
            LOG_IF(INFO, DEBUG_ELEVATOR) << "ElevatorControlManager PRE_ELEVATOR ... ";
            doPreElevator();
        } else if (preState == ElevatorPreState::PRE_SWITCH_MAP) {
            LOG_IF(INFO, DEBUG_ELEVATOR) << "ElevatorControlManager PRE_SWITCH_MAP ... ";
            doPreSwitchMap();
        } else if (preState == ElevatorPreState::PRE_OVER) {
            LOG_IF(INFO, DEBUG_ELEVATOR) << "ElevatorControlManager PRE_OVER ... ";
            callbackElevatorPre(true);
        }

        preState = ElevatorPreState::PRE_NONE;
    }

}

[[noreturn]] void ElevatorControlManager::elevator_post_thread_func() {

    while (true) {

        std::unique_lock<std::mutex> lk(post_mutex_);
        post_condition_variable_.wait(lk, [this] {
            return postState != ElevatorPostState::POST_NONE;
        });
        if (postState == ElevatorPostState::POST_CIRCULATION) {
            LOG_IF(INFO, DEBUG_ELEVATOR) << "ElevatorControlManager POST_CIRCULATION ... ";
            doPostCirculation();
        } else if (postState == ElevatorPostState::POST_ELEVATOR) {
            LOG_IF(INFO, DEBUG_ELEVATOR) << "ElevatorControlManager POST_ELEVATOR ... ";
            doPostElevator();
        } else if (postState == ElevatorPostState::POST_SWITCH_MAP) {
            LOG_IF(INFO, DEBUG_ELEVATOR) << "ElevatorControlManager POST_SWITCH_MAP ... ";
            doPostSwitchMap();
        } else if (postState == ElevatorPostState::POST_OVER) {
            LOG_IF(INFO, DEBUG_ELEVATOR) << "ElevatorControlManager POST_OVER ... ";
            callbackElevatorPost(true);
        }

        postState = ElevatorPostState::POST_NONE;
    }
}

void ElevatorControlManager::doPreCirculation() {
    pool_.execute([this]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));

        LOG_IF(INFO, DEBUG_ELEVATOR) << "ElevatorControlManager doPreCirculation 模拟任务执行完成 ... ";

        {
            std::unique_lock<std::mutex> lk(pre_mutex_);
            preState = ElevatorPreState::PRE_ELEVATOR;
        }
        pre_condition_variable_.notify_one();
    });
}

void ElevatorControlManager::doPreElevator() {
    pool_.execute([this]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));

        LOG_IF(INFO, DEBUG_ELEVATOR) << "ElevatorControlManager doPreElevator 模拟任务执行完成 ... ";

        {
            std::unique_lock<std::mutex> lk(pre_mutex_);
            preState = ElevatorPreState::PRE_SWITCH_MAP;
        }
        pre_condition_variable_.notify_one();
    });
}

void ElevatorControlManager::doPreSwitchMap() {
    pool_.execute([this]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));

        LOG_IF(INFO, DEBUG_ELEVATOR) << "ElevatorControlManager doPreSwitchMap 模拟任务执行完成 ... ";

        {
            std::unique_lock<std::mutex> lk(pre_mutex_);
            preState = ElevatorPreState::PRE_OVER;
        }
        pre_condition_variable_.notify_one();
    });
}

void ElevatorControlManager::doPostCirculation() {
    pool_.execute([this]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));

        LOG_IF(INFO, DEBUG_ELEVATOR) << "ElevatorControlManager doPostCirculation 模拟任务执行完成 ... ";

        {
            std::unique_lock<std::mutex> lk(post_mutex_);
            postState = ElevatorPostState::POST_ELEVATOR;
        }
        post_condition_variable_.notify_one();
    });
}

void ElevatorControlManager::doPostElevator() {
    pool_.execute([this]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));

        LOG_IF(INFO, DEBUG_ELEVATOR) << "ElevatorControlManager doPostElevator 模拟任务执行完成 ... ";

        {
            std::unique_lock<std::mutex> lk(post_mutex_);
            postState = ElevatorPostState::POST_SWITCH_MAP;
        }
        post_condition_variable_.notify_one();
    });
}

void ElevatorControlManager::doPostSwitchMap() {
    pool_.execute([this]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));

        LOG_IF(INFO, DEBUG_ELEVATOR) << "ElevatorControlManager doPostSwitchMap 模拟任务执行完成 ... ";

        {
            std::unique_lock<std::mutex> lk(post_mutex_);
            postState = ElevatorPostState::POST_OVER;
        }
        post_condition_variable_.notify_one();
    });
}

void ElevatorControlManager::initialize(ros::NodeHandle handle) {
    interruptAccessElevators();
    pool_.setNumOfThreads(4);

    preState = ElevatorPreState::PRE_NONE;
    postState = ElevatorPostState::POST_NONE;

    subscriberOdom = handle.subscribe("/odom", 10, &ElevatorControlManager::subscribeOdomCallback, this);
    subscriberImu = handle.subscribe(Environment::instance().isRealEnvironment ? "/imu/data" : "/imu",
                                     10, &ElevatorControlManager::subscribeImuCallback, this);
    publisherCmdVel = handle.advertise<geometry_msgs::Twist>("/cmd_vel", 1);

    elevator_pre_thread = std::thread(&ElevatorControlManager::elevator_pre_thread_func, this);
    elevator_pre_thread.detach();
    elevator_post_thread = std::thread(&ElevatorControlManager::elevator_post_thread_func, this);
    elevator_post_thread.detach();

    subscriberElevatorManager = handle.subscribe("/elevator_manager", 1,
                                                 &ElevatorControlManager::elevatorManagerSubscribeCallback,
                                                 this);

}

void ElevatorControlManager::setCallbackElevatorPre(const std::function<void(bool)> &callbackElevatorPre) {
    ElevatorControlManager::callbackElevatorPre = callbackElevatorPre;
}

void ElevatorControlManager::setCallbackElevatorPost(const std::function<void(bool)> &callbackElevatorPost) {
    ElevatorControlManager::callbackElevatorPost = callbackElevatorPost;
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

    LOG_IF(INFO, DEBUG_ELEVATOR) << "distance_x ： " << distance_x
                                 << "， old_yaw ： " << old_yaw
                                 << "， imu_yaw ： " << imu_yaw
                                 << "， difference_yaw ： " << difference_yaw
                                 << "， old_angle ： " << old_angle
                                 << "， curr_angle ： " << curr_angle
                                 << "， angle_difference ： " << angle_difference;
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


void ElevatorControlManager::handlePreFlow(const std::vector<RealBlock> &preFlows) {
    if (preFlows.size() != 3) {
        throw app::exception(make_error_code(error::elevator_pre_flow_error));
    }
    preCirculationBlock = preFlows[0];
    preElevatorBlock = preFlows[1];
    preSwitchMapBlock = preFlows[2];

    //todo 逻辑判断，看看执行哪个流程

    {
        std::unique_lock<std::mutex> lk(pre_mutex_);
        preState = ElevatorPreState::PRE_CIRCULATION;
    }
    pre_condition_variable_.notify_one();
}

void ElevatorControlManager::handlePostFlow(const std::vector<RealBlock> &postFlows) {
    if (postFlows.size() != 3) {
        throw app::exception(make_error_code(error::elevator_post_flow_error));
    }
    postCirculationBlock = postFlows[0];
    postElevatorBlock = postFlows[1];
    postSwitchMapBlock = postFlows[2];

    //todo 逻辑判断，看看执行哪个流程
    {
        std::unique_lock<std::mutex> lk(post_mutex_);
        postState = ElevatorPostState::POST_CIRCULATION;
    }
    post_condition_variable_.notify_one();
}
